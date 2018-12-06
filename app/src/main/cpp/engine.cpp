#include <vector>
#include "types.h"
#include "board.h"
#include "evaluate.h"
#include "evaluate.cpp"


using namespace types;
using namespace std;

const short WHITE_LOSING = -30000;
const short BLACK_LOSING = 30000;


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
        short depth,
        short maxDepth,
        _moves *takeMoves
) {
    if (depth >= maxDepth)
        return eval(wc, bc, wq, bq);

    const _moves moves = ((takeMoves == nullptr) ? getMoves(wc, bc, wq, bq, w90, b90, isWhiteMove) : *takeMoves);
    delete takeMoves;

    if (moves.empty())
        return isWhiteMove ? WHITE_LOSING + depth : BLACK_LOSING - depth;

    for (const _move move : moves) {
        _board copy_wc = wc;
        _board copy_bc = bc;
        _board copy_wq = wq;
        _board copy_bq = bq;
        _board copy_w90 = w90;
        _board copy_b90 = b90;

        makeMove(copy_wc, copy_bc, copy_wq, copy_bq, copy_w90, copy_b90, move, isWhiteMove);

        _moves *takes;
        bool turn;
        bool empty;
        if (isTake(move)) {
            takes = new _moves;
            *takes = getTakes(copy_wc, copy_bc, copy_wq, copy_bq, copy_w90, copy_b90, isWhiteMove, move);
            empty = (*takes).empty();
            turn = empty != isWhiteMove;
            if (empty)
                delete takes;
        } else {
            turn = !isWhiteMove;
            empty = true;
        }
        short result = alphaBeta(
                copy_wc, copy_bc, copy_wq,
                copy_bq, copy_w90, copy_b90,
                turn,
                alpha,
                beta,
                empty ? depth + 1 : depth,
                maxDepth,
                empty ? nullptr : takes
        );

        if (isWhiteMove) {
            if (result > alpha) {
                alpha = result;
            }
        } else {
            if (result < beta) {
                beta = result;
            }
        }
        if (alpha >= beta)
            return alpha;
    }

    return isWhiteMove ? alpha : beta;
}


_move __getBestMove(
        _board wc,
        _board bc,
        _board wq,
        _board bq,
        _board w90,
        _board b90,
        _cb isWhiteMove,
        short depth
) {
    const _moves moves = getMoves(wc, bc, wq, bq, w90, b90, isWhiteMove);
    _move bestMove = static_cast<_move>(~0);
    short bestScore = isWhiteMove ? WHITE_LOSING : BLACK_LOSING;

    for (_move move : moves) {
        //<editor-fold desc="Dublicated code from alphaBeta()">
        _board copy_wc = wc;
        _board copy_bc = bc;
        _board copy_wq = wq;
        _board copy_bq = bq;
        _board copy_w90 = w90;
        _board copy_b90 = b90;

        makeMove(copy_wc, copy_bc, copy_wq, copy_bq, copy_w90, copy_b90, move, isWhiteMove);

        _moves *takes;
        bool turn;
        bool empty;
        if (isTake(move)) {
            takes = new _moves;
            *takes = getTakes(copy_wc, copy_bc, copy_wq, copy_bq, copy_w90, copy_b90, isWhiteMove, move);
            empty = (*takes).empty();
            turn = empty != isWhiteMove;
            if (empty)
                delete takes;
        } else {
            turn = !isWhiteMove;
            empty = true;
        }
        short result = alphaBeta(
                copy_wc, copy_bc, copy_wq, copy_bq, copy_w90, copy_b90,
                turn, WHITE_LOSING, BLACK_LOSING, 0, depth,
                empty ? nullptr : takes
        );
        //</editor-fold>

        if (result > bestScore == isWhiteMove) {
            bestScore = result;
            bestMove = move;
        }
    }

    return bestMove;
}

_moves getBestMove(
        _board wc,
        _board bc,
        _board wq,
        _board bq,
        _board w90,
        _board b90,
        _cb isWhiteMove,
        short depth
) {
    _moves moves = getMoves(wc, bc, wq, bq, w90, b90, isWhiteMove);
    if (moves.size() == 1)
        return moves;
    _moves move;
    move.push_back(__getBestMove(wc, bc, wq, bq, w90, b90, isWhiteMove, depth));
    return move;
}