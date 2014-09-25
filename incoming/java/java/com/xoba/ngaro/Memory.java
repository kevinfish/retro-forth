package com.xoba.ngaro;

import java.io.File;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.util.Arrays;

import com.xoba.ngaro.inf.IMemory;

public class Memory implements IMemory {

	private final int n;
	private final int[] memory;

	public static IMemory load(File f, boolean littleEndian) throws IOException {
		RandomAccessFile in = new RandomAccessFile(f, "r");
		try {
			int n = (int) (in.length() / 4);
			IMemory mem = new Memory(n);
			for (int i = 0; i < n; i++) {
				int v = in.readInt();
				mem.set(i, littleEndian ? Memory.switchEndian(v) : v);
			}
			return mem;
		} finally {
			in.close();
		}
	}

	public Memory(int n) {
		this.n = n;
		this.memory = new int[this.n];
	}

	@Override
	public int get(int pc) {
		return memory[pc];
	}

	@Override
	public void set(int pc, int value) {
		memory[pc] = value;
	}

	@Override
	public int size() {
		return n;
	}

	@Override
	public void clear() {
		for (int i = 0; i < n; i++) {
			memory[i] = 0;
		}
	}

	@Override
	public boolean equals(Object o) {
		if (o instanceof IMemory) {
			IMemory m = (IMemory) o;
			if (m.size() != this.size()) {
				return false;
			}
			for (int i = 0; i < n; i++) {
				if (this.get(i) != m.get(i)) {
					return false;
				}
			}
			return true;
		} else {
			return false;
		}

	}

	@Override
	public int hashCode() {
		return Arrays.hashCode(memory);
	}

	public static int switchEndian(int value) {
		int b1 = value & 0xff;
		int b2 = (value >> 8) & 0xff;
		int b3 = (value >> 16) & 0xff;
		int b4 = (value >> 24) & 0xff;
		return b1 << 24 | b2 << 16 | b3 << 8 | b4;
	}

}