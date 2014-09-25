package com.xoba.ngaro;

import java.io.File;
import java.io.IOException;
import java.io.RandomAccessFile;

import com.xoba.ngaro.inf.IMemory;
import com.xoba.ngaro.inf.IOManager;
import com.xoba.ngaro.inf.IReplIOManager;
import com.xoba.ngaro.inf.IStack;

public class NGaroVM {

	private int ip;
	private final IMemory memory;

	private final IMemory ports = new Memory(12);

	private final IStack data, address;

	private final IOManager im;
	private final IReplIOManager rm;

	public static interface ISaveImageController {
		public File getSavedImageFile();
	}

	private final ISaveImageController sic;

	public NGaroVM(int dataStackSize, int addressStackSize, IMemory m, IOManager im, IReplIOManager rm,
			ISaveImageController sic) throws IOException {
		this.data = new Stack(dataStackSize);
		this.address = new Stack(addressStackSize);
		this.memory = m;
		this.im = im;
		this.rm = rm;
		this.sic = sic;
	}

	public static final int VM_NOP = 0;
	public static final int VM_LIT = 1;
	public static final int VM_DUP = 2;
	public static final int VM_DROP = 3;
	public static final int VM_SWAP = 4;
	public static final int VM_PUSH = 5;
	public static final int VM_POP = 6;
	public static final int VM_LOOP = 7;
	public static final int VM_JUMP = 8;
	public static final int VM_RETURN = 9;
	public static final int VM_LT_JUMP = 10;
	public static final int VM_GT_JUMP = 11;
	public static final int VM_NE_JUMP = 12;
	public static final int VM_EQ_JUMP = 13;
	public static final int VM_FETCH = 14;
	public static final int VM_STORE = 15;
	public static final int VM_ADD = 16;
	public static final int VM_SUB = 17;
	public static final int VM_MUL = 18;
	public static final int VM_DIVMOD = 19;
	public static final int VM_AND = 20;
	public static final int VM_OR = 21;
	public static final int VM_XOR = 22;
	public static final int VM_SHL = 23;
	public static final int VM_SHR = 24;
	public static final int VM_ZERO_EXIT = 25;
	public static final int VM_INC = 26;
	public static final int VM_DEC = 27;
	public static final int VM_IN = 28;
	public static final int VM_OUT = 29;
	public static final int VM_WAIT = 30;

	private void saveImage(File f) {
		try {
			RandomAccessFile out = new RandomAccessFile(f, "rw");
			try {
				for (int i = 0; i < memory.size(); i++) {
					out.writeInt(Memory.switchEndian(memory.get(i)));
				}
			} finally {
				out.close();
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private String rxGetString() {
		int name = data.pop();
		StringBuffer buf = new StringBuffer();
		int i = 0;
		boolean done = false;
		while (!done) {
			int j = memory.get(name + i);
			if (j == 0) {
				done = true;
			} else {
				buf.append((char) j);
				i++;
			}
		}
		return buf.toString();
	}

	private void handleDevices() {

		if (ports.get(0) == 1) {
			return;
		}

		if (ports.get(0) == 0 && ports.get(1) == 1) {
			final byte[] b = { 0, 0, 0 };
			try {
				b[0] = (byte) rm.read();
			} catch (Exception e) {
				System.err.println(e);
			}
			ports.set(1, b[0]);
			ports.set(0, 1);
		}

		if (ports.get(2) == 1) {
			int x = data.pop();
			if (x < 0) {
				for (char c = 0; c < 300; c++)
					rm.write('\n');
			} else
				rm.write((char) x);
			ports.set(2, 0);
			ports.set(0, 1);
		}

		switch (ports.get(4)) {

		case 0: {
			ports.set(4, 0);
			break;
		}

		case 1: {
			saveImage(sic.getSavedImageFile());
			ports.set(0, 1);
			ports.set(4, 0);
			break;
		}

		case 2: {
			try {
				rm.includeFile(rxGetString());
			} catch (Exception e) {
				throw new RuntimeException(e);
			}
			ports.set(4, 0);
			break;
		}

		case -1: {
			int mode = data.pop();
			String filename = rxGetString();
			ports.set(4, im.rxOpenFile(mode, filename));
			break;
		}

		case -2: {
			int slot = data.pop();
			ports.set(4, im.rxReadFile(slot));
			break;
		}

		case -3: {
			int slot = data.pop();
			int c = data.pop();
			ports.set(4, im.rxWriteFile(slot, c));
			break;
		}

		case -4: {
			int slot = data.pop();
			ports.set(4, im.rxCloseFile(slot));
			break;
		}
		case -5: {
			int slot = data.pop();
			ports.set(4, im.rxGetFilePosition(slot));
			break;
		}

		case -6: {
			int slot = data.pop();
			int pos = data.pop();
			ports.set(4, im.rxSetFilePosition(slot, pos));
			break;
		}

		case -7: {
			int slot = data.pop();
			ports.set(4, im.rxGetFileSize(slot));
			break;
		}

		case -8: {
			String filename = rxGetString();
			ports.set(4, im.rxDeleteFile(filename));
			break;
		}

		default:
			ports.set(4, 0);

		}

		switch (ports.get(5)) {

		case -1:
			ports.set(5, memory.size());
			ports.set(0, 1);
			break;
		case -5:
			ports.set(5, data.getDepth());
			ports.set(0, 1);
			break;
		case -6:
			ports.set(5, address.getDepth());
			ports.set(0, 1);
			break;
		case -8:
			ports.set(5, (int) (System.currentTimeMillis() / 1000L));
			ports.set(0, 1);
			break;
		case -9:
			ip = memory.size();
			ports.set(5, 0);
			ports.set(0, 1);
			break;
		case -13:
			ports.set(5, 32);
			ports.set(0, 1);
			break;
		case -14:
			ports.set(5, 1);
			ports.set(0, 1);
			break;
		case -15:
			ports.set(5, -1);
			ports.set(0, 1);
			break;

		case -2:
		case -3:
		case -4:
		case -7:
		case -10:
		case -11:
		case -12:
			ports.set(5, 0);
			ports.set(0, 1);
			break;
		}

	}

	/**
	 * Process a single opcode
	 */
	private void process() {

		switch (memory.get(ip)) {

		case VM_NOP: {
			break;
		}

		case VM_LIT: {
			data.push(memory.get(++ip));
			break;
		}

		case VM_DUP: {
			data.push(data.peek());
			break;
		}

		case VM_DROP: {
			data.pop();
			break;
		}

		case VM_SWAP: {
			int x = data.pop();
			int y = data.pop();
			data.push(x);
			data.push(y);
			break;
		}

		case VM_PUSH: {
			address.push(data.pop());
			break;
		}

		case VM_POP: {
			data.push(address.pop());
			break;
		}

		case VM_LOOP: {
			data.push(data.pop() - 1);
			ip++;
			if (data.peek() != 0 && data.peek() > -1)
				ip = memory.get(ip) - 1;
			else
				data.drop(1);
			break;
		}

		case VM_JUMP: {
			ip++;
			ip = memory.get(ip) - 1;
			if (memory.get(ip + 1) == 0)
				ip++;
			if (memory.get(ip + 1) == 0)
				ip++;
			break;
		}

		case VM_RETURN: {
			ip = address.pop();
			if (memory.get(ip + 1) == 0)
				ip++;
			if (memory.get(ip + 1) == 0)
				ip++;
			break;
		}

		case VM_LT_JUMP: {
			ip++;
			if (data.pop() < data.pop())
				ip = memory.get(ip) - 1;
			break;
		}

		case VM_GT_JUMP: {
			ip++;
			if (data.pop() > data.pop())
				ip = memory.get(ip) - 1;
			break;
		}

		case VM_NE_JUMP: {
			ip++;
			if (data.pop() != data.pop())
				ip = memory.get(ip) - 1;
			break;
		}

		case VM_EQ_JUMP: {
			ip++;
			if (data.pop() == data.pop())
				ip = memory.get(ip) - 1;
			break;
		}

		case VM_FETCH: {
			int x = data.pop();
			data.push(memory.get(x));
			break;
		}

		case VM_STORE: {
			memory.set(data.pop(), data.pop());
			break;
		}

		case VM_ADD: {
			int y = data.pop();
			int x = data.pop();
			data.push(x + y);
			break;
		}

		case VM_SUB: {
			int y = data.pop();
			int x = data.pop();
			data.push(x - y);
			break;
		}

		case VM_MUL: {
			int y = data.pop();
			int x = data.pop();
			data.push(x * y);
			break;
		}

		case VM_DIVMOD: {
			final int x = data.pop();
			final int y = data.pop();
			data.push(y % x);
			data.push(y / x);
			break;
		}

		case VM_AND: {
			int y = data.pop();
			int x = data.pop();
			data.push(x & y);
			break;
		}

		case VM_OR: {
			int y = data.pop();
			int x = data.pop();
			data.push(x | y);
			break;
		}

		case VM_XOR: {
			int y = data.pop();
			int x = data.pop();
			data.push(x ^ y);
			break;
		}

		case VM_SHL: {
			int y = data.pop();
			int x = data.pop();
			data.push(x << y);
			break;
		}

		case VM_SHR: {
			int y = data.pop();
			int x = data.pop();
			data.push(x >>= y);
			break;
		}

		case VM_ZERO_EXIT: {
			if (data.peek() == 0) {
				data.drop(1);
				ip = address.pop();
			}
			break;
		}

		case VM_INC: {
			data.push(data.pop() + 1);
			break;
		}

		case VM_DEC: {
			data.push(data.pop() - 1);
			break;
		}

		case VM_IN: {
			final int x = data.pop();
			data.push(ports.get(x));
			ports.set(x, 0);
			break;
		}

		case VM_OUT: {
			ports.set(0, 0);
			int x = data.pop();
			int y = data.pop();
			ports.set(x, y);
			break;
		}

		case VM_WAIT: {
			handleDevices();
			break;
		}

		default: {
			address.push(ip);
			ip = memory.get(ip) - 1;
			if (memory.get(ip + 1) == 0)
				ip++;
			if (memory.get(ip + 1) == 0)
				ip++;
			break;
		}

		}
	}

	public void run() {
		for (ip = 0; ip < memory.size(); ip++) {
			process();
		}
	}

	private static void copy(IMemory source, IMemory target, int p) {
		for (int i = 0; i < source.size(); i++) {
			target.set(p + i, source.get(i));
		}
	}

	public static IMemory initializeMemory(File imageFile, boolean littleEndian, int totalMemorySize)
			throws IOException {
		IMemory mem = new Memory(totalMemorySize);
		IMemory image = Memory.load(imageFile, true);
		copy(image, mem, 0);
		return mem;
	}

}
