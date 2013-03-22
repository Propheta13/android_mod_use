package com.propheta13.amoduse;

public class IoctlWrapper {
	public static native String getKData();
	
	public static String getData() {
		return getKData();
	}
	
	static {
        System.loadLibrary("ioctlwrap");
    }
}
