package ru.rpuxa.checkerscpp.game.board

import ru.rpuxa.checkerscpp.natives.NativeMove.Companion.X
import ru.rpuxa.checkerscpp.natives.NativeMove.Companion.Y
import ru.rpuxa.checkerscpp.natives.checkBit

class Position {
    val board: Array<Array<Figure>> =
        Array(BOARD_SIZE) {
            Array<Figure>(
                BOARD_SIZE
            ) { NullFigure }
        }

    fun setFromNative(
        whiteCheckers: Int,
        blackCheckers: Int,
        blackQueens: Int,
        whiteQueens: Int
    ) {
        val types = arrayOf(whiteCheckers, blackCheckers, whiteQueens, blackQueens)
        for (type in 0 until 4) {
            for (bit in 0 until 32) {
                board[X[bit]][Y[bit]] =
                        if (types[type] checkBit bit) {
                            when (type) {
                                0 -> WhiteChecker
                                1 -> BlackChecker
                                2 -> WhiteQueen
                                3 -> BlackQueen
                                else -> throw IllegalStateException()
                            }
                        } else {
                            NullFigure
                        }
            }
        }
    }

    companion object {
        const val BOARD_SIZE = 8
        @JvmField
        val BOARD_RANGE = 0 until BOARD_SIZE

        fun createStartPosition(): Position {
            val position = Position()
            for (x in BOARD_RANGE)
                for (y in BOARD_RANGE)
                    position.board[x][y] = when (y) {
                        in 0..2 -> WhiteChecker
                        in 5..7 -> BlackChecker
                        else -> NullFigure
                    }

            return position
        }
    }

}