#include "bitutils.h"
#include "bitutils.cpp"
#include "types.h"
#include "board.h"

_move createMove(_ci from, _ci to, _cb isTake, _cb isWhite, _cb isQueen) {
    return static_cast<_move>(isTake | (from << 1) | (to << 6) | (isWhite << 11) | (isQueen << 12));
}

inline _ui isTake(_cmove &move) {
    return move & 1u;
}

inline _ui isWhite(_cmove &move) {
    return (move >> 11) & 1u;
}

inline _ui isQueen(_cmove &move) {
    return move >> 12;
}

const _ui FIRST_HORIZONTAL[] = {
        1, 1, 0, 0, 1,
        0, 0, 0, 0, 1,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0
};

const _ui LAST_HORIZONTAL[] = {
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 1, 0, 0,
        0, 0, 1, 0, 0,
        1, 1
};

const _ui UN_ROTATED_CELLS[] = {
        9, 16,
        4, 10, 17, 23,
        1, 5, 11, 18, 24, 28,
        0, 2, 6, 12, 19, 25, 29, 31,
        3, 7, 13, 20, 26, 30,
        8, 14, 21, 27,
        15, 22,
};

const _ui ROTATED_CELLS[] = {
        12,
        6, 13, 20,
        2, 7, 14, 21, 26,
        0, 3, 8, 15, 22, 27, 30,
        1, 4, 9, 16, 23, 28, 31,
        5, 10, 17, 24, 29,
        11, 18, 25,
        19
};

const _ui SHIFT[] = {
        0,
        1, 1, 1,
        4, 4, 4, 4, 4,
        9, 9, 9, 9, 9, 9, 9,
        16, 16, 16, 16, 16, 16, 16,
        23, 23, 23, 23, 23,
        28, 28, 28,
        31
};

const _ui SHIFT_90[] = {
        0, 0,
        2, 2, 2, 2,
        6, 6, 6, 6, 6, 6,
        12, 12, 12, 12, 12, 12, 12, 12,
        20, 20, 20, 20, 20, 20,
        26, 26, 26, 26,
        30, 30

};

const _ui MASK[] = {
        0b1,
        0b111, 0b111, 0b111,
        0b11111, 0b11111, 0b11111, 0b11111, 0b11111,
        0b1111111, 0b1111111, 0b1111111, 0b1111111, 0b1111111, 0b1111111, 0b1111111,
        0b1111111, 0b1111111, 0b1111111, 0b1111111, 0b1111111, 0b1111111, 0b1111111,
        0b11111, 0b11111, 0b11111, 0b11111, 0b11111,
        0b111, 0b111, 0b111,
        0b1,
};

const _ui MASK_90[] = {
        0b11, 0b11,
        0b1111, 0b1111, 0b1111, 0b1111,
        0b111111, 0b111111, 0b111111, 0b111111, 0b111111, 0b111111,
        0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
        0b111111, 0b111111, 0b111111, 0b111111, 0b111111, 0b111111,
        0b1111, 0b1111, 0b1111, 0b1111,
        0b11, 0b11
};


_moves **movesW[32];
_moves **movesB[32];
_moves **movesQueen[32];
_moves **movesW90[32];
_moves **movesB90[32];
_moves **movesQueen90[32];

static const int MOVES_SET_POWER = 2 * 2 * 2 * 32 * 32;

_ui takeMasks[MOVES_SET_POWER];
_ui moveBitMasks[MOVES_SET_POWER];
_ui moveBitMasksQueen[MOVES_SET_POWER];
_ui takeMasks90[MOVES_SET_POWER];
_ui moveBitMasks90[MOVES_SET_POWER];

const int dirs[] = {1, -1};

_moves
genMoves(_ci &posOnBoard, _ci &pos, _ci &length, _board &enemy, _board &all, _cb &isQueen, _cb &white,
         const bool &rotated) {
    _moves moves;
    bool taking = false;
    for (const auto &dir : dirs) {
        if (isQueen) {
            bool take = false;
            for (int i = 1; i < 8; ++i) {
                int currentPos = pos + i * dir;
                if (currentPos < 0 || currentPos >= length)
                    break;
                _ui unsignedCurrentPos = static_cast<_ui>(currentPos);
                _ui forward = unsignedCurrentPos + dir;
                if (!getBit(all, unsignedCurrentPos)) {
                    if (!taking || take) {
                        _ci to = posOnBoard - pos + unsignedCurrentPos;
                        moves.push_back(createMove(
                                rotated ? UN_ROTATED_CELLS[posOnBoard] : posOnBoard,
                                rotated ? UN_ROTATED_CELLS[to] : to,
                                taking,
                                white,
                                isQueen
                        ));
                    }
                } else if (take)
                    break;
                else if (getBit(enemy, unsignedCurrentPos) && forward >= 0 && forward < length &&
                         !getBit(all, forward)) {
                    if (!taking)
                        moves.clear();
                    taking = true;
                    take = true;
                }
            }
        } else {
            int forward = pos + dir;
            if (forward < 0 || forward >= length)
                continue;
            _ui unsignedForward = static_cast<_ui>(forward);
            if (!taking && dir == 1 == white && !getBit(all, unsignedForward)) {
                _ci to = posOnBoard - pos + forward;
                moves.push_back(createMove(
                        rotated ? UN_ROTATED_CELLS[posOnBoard] : posOnBoard,
                        rotated ? UN_ROTATED_CELLS[to] : to,
                        taking,
                        white,
                        isQueen
                ));
                continue;
            }
            int forward2 = forward + dir;
            if (forward2 < 0 || forward2 >= length)
                continue;
            _ui unsignedForward2 = static_cast<_ui>(forward2);
            if (getBit(enemy, unsignedForward) && !getBit(all, unsignedForward2)) {
                if (!taking)
                    moves.clear();
                taking = true;

                int to = posOnBoard - pos + unsignedForward2;
                moves.push_back(createMove(
                        rotated ? UN_ROTATED_CELLS[posOnBoard] : posOnBoard,
                        rotated ? UN_ROTATED_CELLS[to] : to,
                        taking,
                        white,
                        isQueen
                ));
            }
        }
    }


    return moves;
}

void genMoves() {
    for (_ui cell = 0; cell < 32; ++cell) {
        _ui length = getLowestBit(MASK[cell] + 1);
        _ui lengthPower = 1u << length;
        movesQueen[cell] = new _moves *[lengthPower];
        movesB[cell] = new _moves *[lengthPower];
        movesW[cell] = new _moves *[lengthPower];
        for (int i = 0; i < lengthPower; ++i) {
            movesQueen[cell][i] = new _moves[lengthPower];
            movesB[cell][i] = new _moves[lengthPower];
            movesW[cell][i] = new _moves[lengthPower];
        }
        for (_ui enemy = 0; enemy < lengthPower; ++enemy) {
            for (_ui all = 0; all < lengthPower; ++all) {
                _ui all0 = all | enemy;
                _ui pos = cell - SHIFT[cell];
                movesQueen[cell][enemy][all] = genMoves(cell, pos, length, enemy, all0, true, true, false);
                movesW[cell][enemy][all] = genMoves(cell, pos, length, enemy, all0, false, true, false);
                movesB[cell][enemy][all] = genMoves(cell, pos, length, enemy, all0, false, false, false);
            }
        }

        _ui rotatedCell = ROTATED_CELLS[cell];
        length = getLowestBit(MASK_90[rotatedCell] + 1);
        lengthPower = 1u << length;
        movesQueen90[rotatedCell] = new _moves *[lengthPower];
        movesB90[rotatedCell] = new _moves *[lengthPower];
        movesW90[rotatedCell] = new _moves *[lengthPower];
        for (int i = 0; i < lengthPower; ++i) {
            movesQueen90[rotatedCell][i] = new _moves[lengthPower];
            movesB90[rotatedCell][i] = new _moves[lengthPower];
            movesW90[rotatedCell][i] = new _moves[lengthPower];
        }

        for (_ui enemy = 0; enemy < lengthPower; ++enemy) {
            for (_ui all = 0; all < lengthPower; ++all) {
/*
                 cell = 22;
                 rotatedCell = ROTATED_CELLS[cell];
                 enemy = 0;
                 all = 0;
                 length = 2;*/

                _ui all0 = all | enemy;
                _ui pos = rotatedCell - SHIFT_90[rotatedCell];


                movesQueen90[rotatedCell][enemy][all] = genMoves(rotatedCell, pos, length,
                                                                 enemy, all0, true, true, true);
                movesW90[rotatedCell][enemy][all] = genMoves(rotatedCell, pos, length,
                                                             enemy, all0, false, true, true);
                movesB90[rotatedCell][enemy][all] = genMoves(rotatedCell, pos, length,
                                                             enemy, all0, false, false, true);
            }
        }
    }
}

void genMovesMask() {
    for (_ui white = 0; white < 2; ++white)
        for (_ui queen = 0; queen < 2; ++queen)
            for (_ui from = 0; from < 32; ++from)
                for (_ui to = 0; to < 32; ++to)
                    for (_ui isTake = 0; isTake < 2; ++isTake) {
                        if (from == to)
                            break;
                        _ui rotatedFrom = ROTATED_CELLS[from];
                        _ui rotatedTo = ROTATED_CELLS[to];
                        _move move = createMove(from, to,
                                                static_cast<const bool &>(isTake),
                                                static_cast<_cb &>(white),
                                                static_cast<_cb>(queen));
                        if (SHIFT[from] == SHIFT[to]) {
                            _ui take = 0u;
                            for (int cell = (to < from ? to : from); cell <= (to > from ? to : from); ++cell) {
                                take |= 1 << cell;
                            }
                            takeMasks[move] = ~take;
                        } else if (SHIFT_90[rotatedFrom] == SHIFT_90[rotatedTo]) {
                            _ui take = 0u;

                            for (int rotatedCell = (rotatedTo < rotatedFrom ? rotatedTo : rotatedFrom);
                                 rotatedCell <= (rotatedTo > rotatedFrom ? rotatedTo : rotatedFrom);
                                 ++rotatedCell) {
                                take |= 1 << UN_ROTATED_CELLS[rotatedCell];
                            }
                            takeMasks[move] = ~take;
                        } else {
                            continue;
                        }
                        if (queen) {
                            moveBitMasks[move] = 0;
                            moveBitMasksQueen[move] = (1u << from) | (1u << to);

                            moveBitMasks90[move] = (1u << ROTATED_CELLS[from]) | (1u << ROTATED_CELLS[to]);
                        } else if ((white && LAST_HORIZONTAL[to]) || (!white && FIRST_HORIZONTAL[to])) {
                            moveBitMasks[move] = 1u << from;
                            moveBitMasksQueen[move] = 1u << to;
                        } else {
                            moveBitMasksQueen[move] = 0;
                            moveBitMasks[move] = (1u << from) | (1u << to);
                        }

                        //rotated
                        for (_ui i = 0; i < 32; ++i) {
                            if (getBit(takeMasks[move], i))
                                setBitAssign(takeMasks90[move], ROTATED_CELLS[i]);
                            if (getBit(moveBitMasks[move], i))
                                setBitAssign(moveBitMasks90[move], ROTATED_CELLS[i]);
                        }
                    }
}

void gen() {
    genMoves();
    genMovesMask();
}

inline _moves __getMove(
        _cboard our_c,
        _cboard enemy_c,
        _cboard our_q,
        _cboard enemy_q,
        _cboard our90,
        _cboard enemy90,
        _cb isWhiteMove,
        _ui cell
) {
    _moves moves;
    _ui enemy = enemy_c | enemy_q;
    _ui our = our_c | our_q;
    _ui all = enemy | our;
    _ui all90 = enemy90 | our90;
    _ui queen = getBit(our_q, cell);
    _ui rotatedCell = ROTATED_CELLS[cell];
    bool take = false;
    for (_ui rotated = 0; rotated < 2; ++rotated) {
        _moves generatedMoves = rotated ?
                                (queen ? movesQueen90 : (isWhiteMove ? movesW90
                                                                     : movesB90))[rotatedCell][
                                        (enemy90 >> SHIFT_90[rotatedCell]) & MASK_90[rotatedCell]][
                                        (all90 >> SHIFT_90[rotatedCell]) & MASK_90[rotatedCell]] :
                                (queen ? movesQueen : (isWhiteMove ? movesW : movesB))[cell][
                                        (enemy >> SHIFT[cell]) & MASK[cell]][(all >> SHIFT[cell]) &
                                                                             MASK[cell]];
        if (!generatedMoves.empty() && isTake(generatedMoves[0])) {
            if (!take)
                moves.clear();
            take = true;
        } else if (take)
            continue;
        for (const _move move : generatedMoves) {
            moves.push_back(move);
        }
    }
    return moves;
}

_moves getMove(
        _cboard wc,
        _cboard bc,
        _cboard wq,
        _cboard bq,
        _cboard w90,
        _cboard b90,
        _cb isWhiteMove,
        _ui cell
) {
    return isWhiteMove ?
           __getMove(wc, bc, wq, bq, w90, b90, isWhiteMove, cell) :
           __getMove(bc, wq, bq, wq, b90, w90, isWhiteMove, cell);
}

inline _moves __getMoves(
        _cboard &our_c,
        _cboard &enemy_c,
        _cboard &our_q,
        _cboard &enemy_q,
        _cboard &our90,
        _cboard &enemy90,
        _cb &isWhiteMove
) {
    _moves moves;
    _ui enemy = enemy_c | enemy_q;
    _ui our = our_c | our_q;
    _ui all = enemy | our;
    _ui all90 = enemy90 | our90;
    bool take = false;
    for (_ui queen = 0; queen < 2; ++queen) {
        _board ourIter = queen ? our_q : our_c;
        while (ourIter) {
            _ui cell = getLowestBit(ourIter);
            zeroLowestBitAssign(ourIter);
            _ui rotatedCell = ROTATED_CELLS[cell];
            for (_ui rotated = 0; rotated < 2; ++rotated) {
                _moves generatedMoves = rotated ?
                                        (queen ? movesQueen90 : (isWhiteMove ? movesW90
                                                                             : movesB90))[rotatedCell][
                                                (enemy90 >> SHIFT_90[rotatedCell]) & MASK_90[rotatedCell]][
                                                (all90 >> SHIFT_90[rotatedCell]) & MASK_90[rotatedCell]] :
                                        (queen ? movesQueen : (isWhiteMove ? movesW : movesB))[cell][
                                                (enemy >> SHIFT[cell]) & MASK[cell]][(all >> SHIFT[cell]) &
                                                                                     MASK[cell]];
                if (!generatedMoves.empty() && isTake(generatedMoves[0])) {
                    if (!take)
                        moves.clear();
                    take = true;
                } else if (take)
                    continue;
                for (const _move move : generatedMoves) {
                    moves.push_back(move);
                }
            }
        }
    }
    return moves;
}

_moves getMoves(
        _cboard wc,
        _cboard bc,
        _cboard wq,
        _cboard bq,
        _cboard w90,
        _cboard b90,
        _cb isWhiteMove
) {
    return isWhiteMove ?
           __getMoves(wc, bc, wq, bq, w90, b90, isWhiteMove) :
           __getMoves(bc, wc, bq, wq, b90, w90, isWhiteMove);
}

inline void __makeMove(
        _board &our_c,
        _board &enemy_c,
        _board &our_q,
        _board &enemy_q,
        _board &our90,
        _board &enemy90,
        _cmove &move
) {
    _board takeMask = takeMasks[move];

    _ui i = moveBitMasks[move];
    _ui i1 = moveBitMasks90[move];
    _ui i2 = takeMasks90[move];

    our_c ^= i;
    enemy_c &= takeMask;
    our_q ^= moveBitMasksQueen[move];
    enemy_q &= takeMask;
    our90 ^= i1;
    enemy90 &= i2;
}

void makeMove(
        _board &wc,
        _board &bc,
        _board &wq,
        _board &bq,
        _board &w90,
        _board &b90,
        _cmove move,
        _cb isWhiteMove
) {
    if (isWhiteMove)
        __makeMove(wc, bc, wq, bq, w90, b90, move);
    else
        __makeMove(bc, wc, bq, wq, b90, w90, move);
}

_board rotateBoard(_board board) {
    _board rotated = 0;
    for (_ui cell = 0; cell < 32; ++cell) {
        if (getBit(board, cell))
            rotated = setBit(rotated, ROTATED_CELLS[cell]);
    }

    return rotated;
}

