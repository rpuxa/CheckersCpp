#include <jni.h>
#include "board.h"
#include "board.cpp"
#include "bitutils.h"

extern "C"
JNIEXPORT void JNICALL
Java_ru_rpuxa_checkerscpp_natives_NativeMethods_prepareEngine(JNIEnv *env, jclass type) {
    gen();
}

extern "C"
JNIEXPORT jshort JNICALL
Java_ru_rpuxa_checkerscpp_natives_NativeMethods_getBestMove(JNIEnv *env, jclass type, jint whiteCheckers,
                                                            jint blackCheckers, jint whiteQueens, jint blackQueens,
                                                            jboolean isWhiteMove, jint analyzeDepth) {

    return 19;
}

const _move END_MOVES_FLAG = ~static_cast<const _move>(0);

extern "C"
JNIEXPORT void JNICALL
Java_ru_rpuxa_checkerscpp_natives_NativeMethods_getAvailableMoves(JNIEnv *env, jclass type, jint whiteCheckers,
                                                                  jint blackCheckers, jint whiteQueens,
                                                                  jint blackQueens, jint cell,
                                                                  jshortArray movesArray_) {
    jshort *movesArray = env->GetShortArrayElements(movesArray_, 0);

    _board wc = static_cast<_board>(whiteCheckers);
    _board bc = static_cast<_board>(blackCheckers);
    _board wq = static_cast<_board>(whiteQueens);
    _board bq = static_cast<_board>(blackQueens);
    _board w = wc | wq;
    _board b = bc | bq;
    bool isWhiteTurn = static_cast<bool>(getBit(w, static_cast<_ci>(cell)));
    const _moves moves = getMove(
            wc,
            bc,
            wq,
            bq,
            rotateBoard(w),
            rotateBoard(b),
            isWhiteTurn,
            static_cast<_ui>(cell)
    );

    for (int i = 0; i < moves.size(); ++i) {
        movesArray[i] = moves[i];
    }

    movesArray[moves.size()] = END_MOVES_FLAG;

    env->ReleaseShortArrayElements(movesArray_, movesArray, 0);
}

extern "C"
JNIEXPORT void JNICALL
Java_ru_rpuxa_checkerscpp_natives_NativeMethods_makeMove(JNIEnv *env, jclass type, jint whiteCheckers,
                                                         jint blackCheckers, jint whiteQueens, jint blackQueens,
                                                         jshort move, jintArray changedPosition_) {
    jint *changedPosition = env->GetIntArrayElements(changedPosition_, 0);

    _board wc = static_cast<_board>(whiteCheckers);
    _board bc = static_cast<_board>(blackCheckers);
    _board wq = static_cast<_board>(whiteQueens);
    _board bq = static_cast<_board>(blackQueens);
    _board w = wc | wq;
    _move m = static_cast<_move>(move);

    _ui tmp1, tmp2;

    bool isWhiteMove = static_cast<bool>(getBit(w, static_cast<_ci>((move >> 1) & 0b11111)));

    makeMove(wc, bc, wq, bq, tmp1, tmp2, m, isWhiteMove);

    changedPosition[0] = whiteCheckers;
    changedPosition[1] = blackCheckers;
    changedPosition[2] = whiteQueens;
    changedPosition[3] = blackQueens;

    env->ReleaseIntArrayElements(changedPosition_, changedPosition, 0);
}