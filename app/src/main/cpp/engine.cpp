/*
//
// Created by Григорий on 02.12.2018.
//

#include "types.h"
#include "board.h"

using namespace types;

short alphaBeta(
        _board wc,
        _board bc,
        _board wq,
        _board bq,
        _board w90,
        _board b90,
        _cb isWhiteMove,
        short alpha,
        short beta,
        short depth
) {
    if (depth <= 0)
        return 0; // todo


    for (const _move move : getMoves(wc, bc, wq, bq, w90, b90, isWhiteMove)) {
        _board copy_wc = wc;
        _board copy_bc = bc;
        _board copy_wq = wq;
        _board copy_bq = bq;
        _board copy_w90 = w90;
        _board copy_b90 = b90;

        makeMove(copy_wc, copy_bc, copy_wq, copy_bq, copy_w90, copy_b90, move, isWhiteMove);

        short result = -alphaBeta(copy_wc, copy_bc, copy_wq, copy_bq, copy_w90, copy_b90)
    }
}*/
