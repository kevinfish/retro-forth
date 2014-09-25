package com.xoba.ngaro;

import java.io.File;
import java.io.RandomAccessFile;
import java.util.HashMap;
import java.util.Map;
import java.util.Random;

import com.xoba.ngaro.inf.IOManager;

public class InputManager implements IOManager {

	private final Map<Integer, RandomAccessFile> randomAccessFiles = new HashMap<Integer, RandomAccessFile>();
	private final Map<Integer, File> files = new HashMap<Integer, File>();

	private int findOpenSlot() {
		Random random = new Random();
		while (true) {
			int n = 1 + random.nextInt(Integer.MAX_VALUE - 10);
			if (!randomAccessFiles.containsKey(n)) {
				return n;
			}
		}
	}

	private RandomAccessFile create(File f, int mode) {
		try {

			RandomAccessFile raf = null;
			switch (mode) {

			case 0: { // reading
				if (f.exists()) {
					raf = new RandomAccessFile(f, "r");
				}
				break;
			}

			case 1: { // writing
				raf = new RandomAccessFile(f, "rw");
				raf.setLength(0);
				break;
			}

			case 2: { // append
				raf = new RandomAccessFile(f, "rw");
				raf.seek(raf.length());
				break;
			}

			case 3: { // mod

				if (f.exists()) {
					raf = new RandomAccessFile(f, "rw");
				}
				break;

			}

			default: {
				throw new IllegalStateException("illegal mode " + mode);
			}

			}

			return raf;

		} catch (Exception e) {
			throw new RuntimeException(e);
		}

	}

	@Override
	public int rxOpenFile(int mode, String filename) {

		int slot = findOpenSlot();

		File f = new File(filename);

		RandomAccessFile raf = create(f, mode);

		if (raf == null) {
			return 0;
		} else {
			files.put(slot, f);
			randomAccessFiles.put(slot, raf);
			return slot;
		}

	}

	@Override
	public int rxReadFile(int slot) {
		try {
			int c = randomAccessFiles.get(slot).read();
			if (c < 0) {
				return 0;
			} else {
				return c;
			}
		} catch (Exception e) {
			return 0;
		}
	}

	@Override
	public int rxWriteFile(int slot, int c) {
		try {
			randomAccessFiles.get(slot).write(c);
			return 1;
		} catch (Exception e) {
			return 0;
		}
	}

	@Override
	public int rxCloseFile(int slot) {
		try {
			try {
				randomAccessFiles.get(slot).close();
				return 0;
			} finally {
				randomAccessFiles.remove(slot);
				files.remove(slot);
			}
		} catch (Exception e) {
			throw new RuntimeException(e);
		}
	}

	@Override
	public int rxGetFilePosition(int slot) {
		try {
			return (int) randomAccessFiles.get(slot).getFilePointer();
		} catch (Exception e) {
			return -1;
		}
	}

	@Override
	public int rxSetFilePosition(int slot, int pos) {
		try {
			randomAccessFiles.get(slot).seek(pos);
			return 0;
		} catch (Exception e) {
			return -1;
		}
	}

	@Override
	public int rxGetFileSize(int slot) {
		try {
			return (int) randomAccessFiles.get(slot).length();
		} catch (Exception e) {
			return -1;
		}
	}

	@Override
	public int rxDeleteFile(String filename) {
		File f = new File(filename);
		if (f.delete()) {
			return -1;
		} else {
			return 0;
		}
	}

}