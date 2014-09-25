package com.xoba.ngaro.inf;

public interface IMemory {

	public int size();

	public int get(int pc);

	public void set(int pc, int value);

	public void clear();

}