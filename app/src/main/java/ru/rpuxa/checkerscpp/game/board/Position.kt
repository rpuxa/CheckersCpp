package ru.rpuxa.checkerscpp.game.board

class Position {
    val board: Array<Array<Figure>> =
        Array(BOARD_SIZE) {
            Array<Figure>(
                BOARD_SIZE
            ) { NullFigure }
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