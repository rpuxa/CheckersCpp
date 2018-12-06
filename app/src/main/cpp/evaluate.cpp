//
// Created by Григорий on 02.12.2018.
//

#include "types.h"
#include "bitutils.h"

const short CHECKER_SCORE = 100;
const short QUEEN_SCORE = 300;
const short BIG_WAY_SCORE = 7;
const short CELLS_SCORE_COEFFICIENT = 5;
const short DIFFERENCE_FLANK_PENALTY = -8;

const _ui BIG_WAY_MASK = 0b10100010000010000001000001000101;
const _ui RIGHT_FLANK_MASK = 0b11110111100011110000111000010000;
const _ui LEFT_FLANK_MASK = ~RIGHT_FLANK_MASK;

const _ui CELLS_SCORE[] = {
        19,
        15, 16, 13,
        12, 12, 9, 8, 7,
        17, 14, 8, 6, 1, 0, 1,
        18, 11, 7, 1, 1, 0, 0,
        11, 3, 1, 0, 0,
        4, 0, 0,
        0
};

const _ui CELLS_SCORE_ROTATED[] = {
        0,
        0, 0, 4,
        0, 0, 1, 3, 11,
        0, 0, 1, 1, 7, 11, 18,
        1, 0, 1, 6, 8, 14, 17,
        7, 8, 9, 12, 12,
        13, 16, 15,
        19
};


short eval(
        _board wc,
        _board bc,
        _board wq,
        _board bq
) {
    _ui whiteCheckersCount = bitCount(wc);
    _ui whiteQueensCount = bitCount(wq);
    _ui blackCheckersCount = bitCount(bc);
    _ui blackQueensCount = bitCount(bq);

    _ui whiteOnBigWay = wq & BIG_WAY_MASK;
    _ui blackOnBigWay = bq & BIG_WAY_MASK;

    //Draw
    if (
            (
                    whiteQueensCount == 1 &&
                    whiteCheckersCount == 0 &&
                    blackQueensCount &&
                    blackCheckersCount + blackQueensCount < 3
            ) || (
                    blackQueensCount == 1 &&
                    blackCheckersCount == 0 &&
                    whiteQueensCount &&
                    whiteCheckersCount + whiteQueensCount < 3
            )
            )
        return 0;
    if (
            (
                    whiteQueensCount == 1 &&
                    whiteCheckersCount == 0 &&
                    blackQueensCount &&
                    blackCheckersCount + blackQueensCount == 3 &&
                    whiteOnBigWay
            ) || (
                    blackQueensCount == 1 &&
                    blackCheckersCount == 0 &&
                    whiteQueensCount >= 1 &&
                    whiteCheckersCount + whiteQueensCount == 3 &&
                    blackOnBigWay
            )
            )
        return 0;

    auto score = static_cast<short>(
            whiteCheckersCount * CHECKER_SCORE +
            whiteQueensCount * QUEEN_SCORE -
            blackCheckersCount * CHECKER_SCORE -
            blackQueensCount * QUEEN_SCORE
    );

    if (whiteOnBigWay)
        score += BIG_WAY_SCORE;
    if (blackOnBigWay)
        score -= BIG_WAY_SCORE;

    //сделать какой то combination

    int left = bitCount(wc & LEFT_FLANK_MASK);
    int right = bitCount(wc & RIGHT_FLANK_MASK);
    int difference = left - right;
    if (difference > 0)
        score += difference * DIFFERENCE_FLANK_PENALTY;
    else
        score -= difference * DIFFERENCE_FLANK_PENALTY;

    left = bitCount(bc & LEFT_FLANK_MASK);
    right = bitCount(bc & RIGHT_FLANK_MASK);
    difference = left - right;
    if (difference > 0)
        score -= difference * DIFFERENCE_FLANK_PENALTY;
    else
        score += difference * DIFFERENCE_FLANK_PENALTY;

    short cellScore = 0;
    while (wc) {
        _ui cell = getLowestBit(wc);
        zeroLowestBitAssign(wc);

        cellScore -= CELLS_SCORE[cell];
    }
    
    while (bc) {
        _ui cell = getLowestBit(bc);
        zeroLowestBitAssign(bc);
        
        cellScore += CELLS_SCORE_ROTATED[cell];
    }

    return score + cellScore / CELLS_SCORE_COEFFICIENT;
}

