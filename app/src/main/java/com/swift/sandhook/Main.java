package com.swift.sandhook;

public class Main {

    static {
        System.loadLibrary("sandhook");
    }

    public static native void test();

}
