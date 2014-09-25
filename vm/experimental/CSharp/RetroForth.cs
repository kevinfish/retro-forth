
/*****************************************************************************
 * Ngaro for Mono / .NET
 *
 * Copyright (c) 2009 - 2011, Simon Waite and Charles Childers
 *
 * Please compile with `gmcs` as `mcs` seems to have a
 * simple Console.in implementation.
 *****************************************************************************/

using System;
using System.IO;
using System.Text;
using System.Threading;
using System.Net.Sockets;
using System.Net;
using System.Collections.Generic;

namespace Retro.Ngaro {
  public class VM {
    //List<TcpClient>   clients = new List<TcpClient>();
    //List<TcpListener> servers = new List<TcpListener>(); 
    TcpClient[] clients = new TcpClient[1024];
    TcpListener[] servers = new TcpListener[1024];

    /* Registers */
    int sp, rsp, ip, shrink;
    int[] data, address, ports, memory;
    string request;

    /* Input Stack  */
    byte[] [] inputs;
    int[] lengths;
    int[] offset;
    int isp;
    static readonly int MAX_OPEN_FILES = 8;
    static readonly int MAX_REQUEST_LENGTH = 1024;
    FileStream[] files = {null,null,null,null,null,null,null,null};

    /* Opcodes recognized by the VM */
    enum OpCodes {
      VM_NOP,      VM_LIT,        VM_DUP,
      VM_DROP,     VM_SWAP,       VM_PUSH,
      VM_POP,      VM_LOOP,       VM_JUMP,
      VM_RETURN,   VM_GT_JUMP,    VM_LT_JUMP,
      VM_NE_JUMP,  VM_EQ_JUMP,    VM_FETCH,
      VM_STORE,    VM_ADD,        VM_SUB,
      VM_MUL,      VM_DIVMOD,     VM_AND,
      VM_OR,       VM_XOR,        VM_SHL,
      VM_SHR,      VM_ZERO_EXIT,  VM_INC,
      VM_DEC,      VM_IN,         VM_OUT,
      VM_WAIT
    }

    void rxSetTextColor(int color, int arg)  {
        ConsoleColor col = ConsoleColor.White;
        bool valid = true;
        switch (color) {
            case 0: col = ConsoleColor.Black;   break;
            case 1: col = ConsoleColor.Red;     break;
            case 2: col = ConsoleColor.Green;   break;
            case 3: col = ConsoleColor.Yellow;  break;
            case 4: col = ConsoleColor.Blue;    break;
            case 5: col = ConsoleColor.Magenta; break;
            case 6: col = ConsoleColor.Cyan;    break;
            case 7: col = ConsoleColor.White;   break;
            default: valid = false; break;
        }
        if (valid) {
            if (arg == 1) Console.ForegroundColor = col;
            else          Console.BackgroundColor = col;
        }
    }

    void rxGetString(int starting) {
        int i = 0;
        char[] requestTmp = new char[MAX_REQUEST_LENGTH];
        while (memory[starting] > 0 && i < MAX_REQUEST_LENGTH) {
            requestTmp[i++] = (char)memory[starting++];
        }
        request = new string(requestTmp);
        request = request.TrimEnd('\0');
    }

    int rxGetFileHandle() {
        int i;
        for (i = 1; i < MAX_OPEN_FILES; i++) {
            if (files[i] == null) return i;
        }
        return 0;
    }

    int rxOpenFile() {
        int slot = rxGetFileHandle();
        int mode = data[sp--];
        int name = data[sp--];
        rxGetString(name);
        if (slot > 0) {
            if (mode == 0) {
                try {
                    files[slot] = File.Open(request, FileMode.Open, FileAccess.Read);
                }
                catch {
                    files[slot] = null;
                    slot = 0;
                }
            }
            if (mode == 1) files[slot] = File.Open(request, FileMode.OpenOrCreate,FileAccess.Write);
            if (mode == 2) files[slot] = File.Open(request, FileMode.Append,FileAccess.Write);
            if (mode == 3) {
                try {
                    files[slot] = File.Open(request, FileMode.Open, FileAccess.ReadWrite);
                }
                catch {
                    files[slot] = null;
                    slot = 0;
                }
            }
        }
        return slot;
    }

    int rxReadFile() {
        int c = files[data[sp--]].ReadByte();
        return c == -1 ? 0 : c;
    }

    int rxWriteFile() {
        int slot = data[sp--];
        int c = data[sp--];
        files[slot].WriteByte((byte)c);
        return 1;
    }

    int rxCloseFile() {
        int tos = data[sp--];
        files[tos].Close();
        files[tos] = null;
        return 0;
    }

    int rxGetFilePosition() {
        int slot = data[sp--];
        return (int)files[slot].Position;
    }

    int rxSetFilePosition() {
        int slot = data[sp--];
        int pos = data[sp--];
        return (int)files[slot].Seek(pos, SeekOrigin.Begin);
    }

    int rxGetFileSize() {
        int slot = data[sp--];
        return (int)files[slot].Length;
    }

    int rxDeleteFile() {
        int name = data[sp--];
        rxGetString(name);
        if(File.Exists(request)) {
            try   { File.Delete(request); }
            catch { return 0; }
            return -1;
        }
        return 0;
    }


    /* Initialize the VM */
    public VM() {
      sp = 0;
      rsp = 0;
      ip = 0;
      data    = new int[128];
      address = new int[1024];
      ports   = new int[30];
      memory  = new int[1000000];

      inputs  = new byte[12][];
      lengths = new int[12];
      isp = 0;
      offset = new int[12];

      loadImage();

      if (memory[0] == 0) {
        Console.Write("Sorry, unable to find retroImage\n");
        Environment.Exit(0);
      }
    }


    /* Load the 'retroImage' into memory */
    public void loadImage() {
      int i = 0;
      if (!File.Exists("retroImage"))
        return;

      BinaryReader binReader = new BinaryReader(File.Open("retroImage", FileMode.Open));
      FileInfo f = new FileInfo("retroImage");
      long s = f.Length / 4;
      try {
        while (i < s) { memory[i] = binReader.ReadInt32(); i++; }
      }
      catch(EndOfStreamException e) {
        Console.WriteLine("{0} caught and ignored." , e.GetType().Name);
      }
      finally {
        binReader.Close();
      }
    }

    public int rxSockConnect() {
        int port = data[sp--];
        rxGetString(data[sp--]);
        string address = request;
        int ndx = 0;
        for(int x = 0;x < 1024;x++){
        	if (clients[x] == null) {
        		ndx = x;
        		break;
        	}
        }
        clients[ndx] = new TcpClient(address, port);
        return ndx;
    }

    public int rxSockListen() {
    	int port = data[sp--];
    	int ndx = 0;
        for(int x = 0;x < 1024;x++){
        	if (servers[x] == null) {
        		ndx = x;
        		break;
        	}
        }
    	servers[ndx] = new TcpListener(IPAddress.Any, port);
    	servers[ndx].Start();
    	return ndx;
    }
    
    public int rxSockStop() {
    	int sock = data[sp--];
    	servers[sock].Stop();
    	servers[sock] = null;
    	return -1;
    }

    public int rxSockAccept() {
    	int sock = data[sp--];
    	int ndx = 0;
        for(int x = 0;x < 1024;x++){
        	if (clients[x] == null) {
        		ndx = x;
        		break;
        	}
        }
    	clients[ndx] = servers[sock].AcceptTcpClient();
    	return ndx;
    }
    
    public int rxSockClientAvailable() {
    	int sock = data[sp--];
    	return servers[sock].Pending() ? -1 : 0;
    }

    public int rxSockReceive() {
        int sockBuffer = data[sp--];
        int sockSize   = data[sp--];
        int sockIndex  = data[sp--];
        byte[] buffer = new byte[sockSize];
        try{
	        int bytesRead = clients[sockIndex].GetStream().Read(buffer, 0, sockSize);
	        for (int x = 0; x < bytesRead; x++) {
	            memory[sockBuffer + x] = buffer[x];
	        }
	        return bytesRead;
        }
        catch {
        	return -1;
        }
    }
    
    public int rxSockReadReady() {
    	int sock = data[sp--];
    	return clients[sock].Available > 0 ? -1 : 0;
    }

    public int rxSockWrite() {
        int sockBuffer = data[sp--];
        int buffSize = data[sp--];
        int sockIndex = data[sp--];
        byte[] buffer = new byte[buffSize];
        for (int x = 0; x < buffSize; x++) {
            buffer[x] = (byte)memory[sockBuffer + x];
        }
        try {
	        clients[sockIndex].GetStream().Write(buffer, 0, buffSize);
	        return buffSize;
        }
        catch {
        	return -1;
        }
    }

    public int rxSockClose() {
        int sockIndex = data[sp--];
        clients[sockIndex].Close();
        clients[sockIndex] = null;
        return -1;
    }

    /* Save the image */
    public void saveImage() {
      int i = 0, j = 1000000;
      BinaryWriter binWriter = new BinaryWriter(File.Open("retroImage", FileMode.Create));
      try {
        if (shrink != 0)
          j = memory[3];
        while (i < j) { binWriter.Write(memory[i]); i++; }
      }
      catch(EndOfStreamException e) {
        Console.WriteLine("{0} caught and ignored." , e.GetType().Name);
      }
      finally {
        binWriter.Close();
      }
    }


    /* Read a key */
    public int read_key() {
      int a = 0;

      /* Check to see if we need to move to the next input source */
      if (isp > 0 && offset[isp] == (lengths[isp])) {
	offset[isp] = 0;
        isp--;
      }

      if (isp > 0) {      /* Read from a file */
        a = (int)inputs[isp][offset[isp]++];
      }
      else {              /* Read from Console */
        ConsoleKeyInfo cki = Console.ReadKey(true);
        a = (int)cki.KeyChar;
      }
      return a;
    }


    /* Handle I/O device emulation */
    public void HandleDevices() {
      if (ports[0] == 1)
        return;

      ports[0] = 1;

      /* Read from input source */
      if (ports[1] == 1) {
        int a = read_key();
        ports[1] = a;
      }

      /* Write to display */
      if (ports[2] == 1) {
        char c = (char)data[sp];
        if (data[sp] < 0)
          Console.Clear();
        else
            Console.Write(c);
        sp--;
        ports[2] = 0;
      }


      switch(ports[4]) {
          /* File Operations: Save Image */
          case 1: {
            saveImage();
            ports[4] = 0;
            break;
          }

          /* File Operations: Add to Input Stack */
          case 2: {
            ports[4] = 0;
            int i = 0;
            char[] s = new char[1024];
            int name = data[sp]; sp--;
            while(memory[name] != 0) {
              s[i] = (char)memory[name];
              i++; name++;
            }
            isp++;
            inputs[isp] = System.IO.File.ReadAllBytes(new String(s, 0, i));
            lengths[isp] = inputs[isp].Length;
            offset[isp] = 0;
          }
          break;

          case -1: ports[4] = rxOpenFile();        break;
          case -2: ports[4] = rxReadFile();        break;
          case -3: ports[4] = rxWriteFile();       break;
          case -4: ports[4] = rxCloseFile();       break;
          case -5: ports[4] = rxGetFilePosition(); break;
          case -6: ports[4] = rxSetFilePosition(); break;
          case -7: ports[4] = rxGetFileSize();     break;
          case -8: ports[4] = rxDeleteFile();      break;
          default: ports[4] = 0;                   break;
    }

      /* Capabilities */
      if (ports[5] == -1) ports[5] = 1000000;
      if (ports[5] == -2) ports[5] = 0;
      if (ports[5] == -3) ports[5] = 0;
      if (ports[5] == -4) ports[5] = 0;
      if (ports[5] == -5) ports[5] = sp;
      if (ports[5] == -6) ports[5] = rsp;
      if (ports[5] == -7) ports[5] = 0;
      if (ports[5] == -8) {
      	TimeSpan ts = DateTime.UtcNow - new DateTime(1970, 1, 1);
      	int unixTime = (int)ts.TotalSeconds;
        int milliseconds = ts.Milliseconds;
        ports[5] = unixTime;
        ports[26] = milliseconds;
      }
      if (ports[5] == -9) {
        ports[5] = 0;
        ip = 1000000;
      }
      if (ports[5] == -10) {
        char[] s = new char[1024];
        int name = data[sp]; sp--;
        int i = 0;
        while(memory[name] != 0) {
          s[i] = (char)memory[name];
          i++; name++;
        }
        byte[] array = Encoding.ASCII.GetBytes(Environment.GetEnvironmentVariable(new String(s,0,i)));
        name = data[sp]; sp--;
        foreach (byte element in array) {
          memory[name] = (int)element;
          name++;
        }
        memory[name] = 0;
        ports[5] = 0;
      }

      if (ports[5] == -11) ports[5] = Console.WindowWidth;
      if (ports[5] == -12) ports[5] = Console.WindowHeight;
      if (ports[5] == -15) ports[5] = -1;

      int tos;
      int nos;
      switch (ports[8])
      {
          case 1:
              tos = data[sp--];
              nos = data[sp--];
              Console.SetCursorPosition(tos, nos);
              break;
          case 2:
              tos = data[sp--];
              rxSetTextColor(tos, 1);
              break;
          case 3:
              tos = data[sp--];
              rxSetTextColor(tos, 2);
              break;
          default:
              break;
      }
      ports[8] = 0;
      switch(ports[20]) {
          case 1: // KeyAvailable
              ports[20] = Console.KeyAvailable ? -1 : 0;
              break;
          case 2: // GetKey
              ports[20] = (int)Console.ReadKey(true).KeyChar;
              break;
          default:
              ports[20] = 0;
              break;
      }
      if (ports[21] == 1) {
          Thread.Sleep(data[sp--]);
          ports[21] = 0;
      }
      switch(ports[22]) {
          case 1:  ports[22] = rxSockConnect();         break;
          case 2:  ports[22] = rxSockListen();          break;
          case 3:  ports[22] = rxSockAccept();          break;
          case 4:  ports[22] = rxSockReceive();         break;
          case 5:  ports[22] = rxSockWrite();           break;
          case 6:  ports[22] = rxSockClose();           break;
          case 7:  ports[22] = rxSockStop();            break;
          case 8:  ports[22] = rxSockClientAvailable(); break;
          case 9:  ports[22] = rxSockReadReady();       break;
          default: ports[22] = 0;                       break;
      }
    }


    /* Process the current opcode */
    public void Process() {
      int x, y;

      switch((OpCodes)memory[ip]) {
        case OpCodes.VM_NOP:
          break;
        case OpCodes.VM_LIT:
          sp++; ip++; data[sp] = memory[ip];
          break;
        case OpCodes.VM_DUP:
          sp++; data[sp] = data[sp-1];
          break;
        case OpCodes.VM_DROP:
          data[sp] = 0; sp--;
          break;
        case OpCodes.VM_SWAP:
          x = data[sp];
          y = data[sp-1];
          data[sp] = y;
          data[sp-1] = x;
          break;
        case OpCodes.VM_PUSH:
          rsp++;
          address[rsp] = data[sp];
          sp--;
          break;
        case OpCodes.VM_POP:
          sp++;
          data[sp] = address[rsp];
          rsp--;
          break;
        case OpCodes.VM_LOOP:
          data[sp]--;
          if (data[sp] != 0 && data[sp] > -1)
            ip = memory[ip + 1] - 1;
          else {
            ip++;
            sp--;
          }
          break;
        case OpCodes.VM_JUMP:
          ip++;
          ip = memory[ip] - 1;
          if (memory[ip+1] == 0)
            ip++;
          if (memory[ip+1] == 0)
            ip++;
          break;
        case OpCodes.VM_RETURN:
          ip = address[rsp]; rsp--;
          break;
        case OpCodes.VM_GT_JUMP:
          ip++;
          if (data[sp-1] > data[sp])
            ip = memory[ip] - 1;
          sp = sp - 2;
          break;
        case OpCodes.VM_LT_JUMP:
          ip++;
          if (data[sp-1] < data[sp])
            ip = memory[ip] - 1;
          sp = sp - 2;
          break;
        case OpCodes.VM_NE_JUMP:
          ip++;
          if (data[sp-1] != data[sp])
            ip = memory[ip] - 1;
          sp = sp - 2;
          break;
        case OpCodes.VM_EQ_JUMP:
          ip++;
          if (data[sp-1] == data[sp])
            ip = memory[ip] - 1;
          sp = sp - 2;
          break;
        case OpCodes.VM_FETCH:
          x = data[sp];
          data[sp] = memory[x];
          break;
        case OpCodes.VM_STORE:
          memory[data[sp]] = data[sp-1];
          sp = sp - 2;
          break;
        case OpCodes.VM_ADD:
          data[sp-1] += data[sp]; data[sp] = 0; sp--;
          break;
        case OpCodes.VM_SUB:
          data[sp-1] -= data[sp]; data[sp] = 0; sp--;
          break;
        case OpCodes.VM_MUL:
          data[sp-1] *= data[sp]; data[sp] = 0; sp--;
          break;
        case OpCodes.VM_DIVMOD:
          x = data[sp];
          y = data[sp-1];
          data[sp] = y / x;
          data[sp-1] = y % x;
          break;
        case OpCodes.VM_AND:
          x = data[sp];
          y = data[sp-1];
          sp--;
          data[sp] = x & y;
          break;
        case OpCodes.VM_OR:
          x = data[sp];
          y = data[sp-1];
          sp--;
          data[sp] = x | y;
          break;
        case OpCodes.VM_XOR:
          x = data[sp];
          y = data[sp-1];
          sp--;
          data[sp] = x ^ y;
          break;
        case OpCodes.VM_SHL:
          x = data[sp];
          y = data[sp-1];
          sp--;
          data[sp] = y << x;
          break;
        case OpCodes.VM_SHR:
          x = data[sp];
          y = data[sp-1];
          sp--;
          data[sp] = y >>= x;
          break;
        case OpCodes.VM_ZERO_EXIT:
          if (data[sp] == 0) {
            sp--;
            ip = address[rsp]; rsp--;
          }
          break;
        case OpCodes.VM_INC:
          data[sp]++;
          break;
        case OpCodes.VM_DEC:
          data[sp]--;
          break;
        case OpCodes.VM_IN:
          x = data[sp];
          data[sp] = ports[x];
          ports[x] = 0;
          break;
        case OpCodes.VM_OUT:
          ports[data[sp]] = data[sp-1];
          sp = sp - 2;
          break;
        case OpCodes.VM_WAIT:
          HandleDevices();
          break;
        default:
          rsp++;
          address[rsp] = ip;
          ip = memory[ip] - 1;
          if (memory[ip+1] == 0)
            ip++;
          if (memory[ip+1] == 0)
            ip++;
          break;
      }
    }


    /* Process the image until the IP reaches the end of memory */
    public void Execute() {
      for (; ip < 1000000; ip++)
         Process();
    }

    /* Main entry point */
    /* Calls all the other stuff and process the command line */
    public static void Main(string [] args) {
      VM vm = new VM();
      vm.shrink = 0;

      for (int i = 0; i < args.Length; i++) {
        if (args[i] == "--shrink")
          vm.shrink = 1;
        if (args[i] == "--about") {
          Console.Write("Retro Language  [VM: C#, .NET]\n\n");
          Environment.Exit(0);
        }
        if (args[i] == "--with") {
          i++;
          vm.isp++;
          vm.inputs[vm.isp] = System.IO.File.ReadAllBytes(args[i]);
          vm.lengths[vm.isp] = vm.inputs[vm.isp].Length;
        }
      }
      vm.Execute();
    }
  }
}
