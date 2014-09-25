package com.xoba.ngaro.inf;

public interface IStack {

	public int peek();

	public int pop();

	public void push(int v);

	public int getDepth();

	public void drop(int i);

	public int peek2();

}