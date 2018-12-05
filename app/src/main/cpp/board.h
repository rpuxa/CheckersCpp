//
// Created by Григорий on 01.12.2018.
//

#ifndef CHECKERSCPP_BOARD_H
#define CHECKERSCPP_BOARD_H

#endif //CHECKERSCPP_BOARD_H

#include "types.h"

using namespace types;

void gen();

_moves getMoves(
        _cboard wc,
        _cboard bc,
        _cboard wq,
        _cboard bq,
        _cboard w90,
        _cboard b90,
        _cb isWhiteMove
);

void makeMove(
        _board &wc,
        _board &bc,
        _board &wq,
        _board &bq,
        _board &w90,
        _board &b90,
        _cmove move,
        _cb isWhiteMove
);

_moves getMove(
        _cboard wc,
        _cboard bc,
        _cboard wq,
        _cboard bq,
        _cboard w90,
        _cboard b90,
        _cb isWhiteMove,
        _ui cell
);

_board rotateBoard(_board board);