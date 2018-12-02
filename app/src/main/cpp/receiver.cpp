#include <jni.h>
#include "board.h"

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