package com.xoba.ngaro.inf;

import java.io.IOException;

public interface IReplIOManager {

	public void includeFile(String name);

	/**
	 * same semantics as java.io.Inputstream.read()
	 * 
	 * @return
	 * @throws IOException
	 */
	public int read();

	public void write(int c);

}
