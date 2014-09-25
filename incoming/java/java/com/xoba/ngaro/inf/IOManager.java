package com.xoba.ngaro.inf;

public interface IOManager {

	/**
	 * returns an opaque handle for the file
	 */
	public int rxOpenFile(int mode, String name);

	public int rxReadFile(int handle);

	public int rxWriteFile(int handle, int c);

	public int rxCloseFile(int handle);

	public int rxGetFilePosition(int handle);

	public int rxSetFilePosition(int handle, int pos);

	public int rxGetFileSize(int handle);

	public int rxDeleteFile(String name);

}