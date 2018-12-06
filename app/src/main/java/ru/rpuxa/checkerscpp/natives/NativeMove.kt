package ru.rpuxa.checkerscpp.natives

import ru.rpuxa.checkerscpp.game.board.Cell
import ru.rpuxa.checkerscpp.game.board.Move
import ru.rpuxa.checkerscpp.game.board.x
import kotlin.experimental.inv

class NativeMove(move: Short) : Move {

    override val from: Cell
    override val to: Cell
    val isTake: Boolean

    init {
        val intMove = move.toInt()
        val fromInt = getFrom(intMove)
        val toInt = getTo(intMove)

        from = X[fromInt] x Y[fromInt]
        to = X[toInt] x Y[toInt]

        isTake = intMove checkBit 0
    }

    fun isOpposite(other: Move): Boolean {
        val x1 = to.x - from.x
        val y1 = to.y - from.y

        val x2 = other.to.x - other.from.x
        val y2 = other.to.y - other.from.y

        return x1 * x2 + y1 * y2 < 0
    }

    companion object {

        @JvmField
        val END_MOVES_FLAG: Short = 0.toShort().inv()

        val X = arrayOf(
            0,
            2, 1, 0,
            4, 3, 2, 1, 0,
            6, 5, 4, 3, 2, 1, 0,
            7, 6, 5, 4, 3, 2, 1,
            7, 6, 5, 4, 3,
            7, 6, 5,
            7
        )

        val Y = arrayOf(
            0,
            0, 1, 2,
            0, 1, 2, 3, 4,
            0, 1, 2, 3, 4, 5, 6,
            1, 2, 3, 4, 5, 6, 7,
            3, 4, 5, 6, 7,
            5, 6, 7,
            7
        )

        private fun getFrom(move: Int) =
            (move ushr 1) and 0b11_111

        private fun getTo(move: Int) =
            (move ushr 6) and 0b11_111

    }
}
