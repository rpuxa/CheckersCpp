package ru.rpuxa.checkerscpp.natives;


class NativeMethods {

    public static native void prepareEngine();

    public static native short getBestMove(
            int whiteCheckers,
            int blackCheckers,
            int whiteQueens,
            int blackQueens,
            boolean isWhiteMove,
            int analyzeDepth
    );

    static {
        System.loadLibrary("receiver");
    }
}