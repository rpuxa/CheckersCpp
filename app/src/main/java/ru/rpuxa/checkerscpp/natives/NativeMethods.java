package ru.rpuxa.checkerscpp.natives;


public class NativeMethods {

    public static native void prepareEngine();

    public static native void getBestMove(
            int whiteCheckers,
            int blackCheckers,
            int whiteQueens,
            int blackQueens,
            boolean isWhiteMove,
            short analyzeDepth,
            short[] movesArray
    );

    public static native void getAvailableMoves(
            int whiteCheckers,
            int blackCheckers,
            int whiteQueens,
            int blackQueens,
            boolean isTurnWhite,
            short[] movesArray
    );

    public static native void makeMove(
            int whiteCheckers,
            int blackCheckers,
            int whiteQueens,
            int blackQueens,
            short move,
            int[] changedPosition
    );

    static {
        System.loadLibrary("receiver");
    }
}