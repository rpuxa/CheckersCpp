package ru.rpuxa.checkerscpp.natives

import ru.rpuxa.checkerscpp.engine.Engine
import ru.rpuxa.checkerscpp.game.board.*
import ru.rpuxa.checkerscpp.game.board.Position.Companion.BOARD_RANGE
import ru.rpuxa.checkerscpp.natives.NativeMove.Companion.END_MOVES_FLAG
import java.util.*
import kotlin.concurrent.thread

object NativeEngine : Engine {

    private val mainThread = thread {
        NativeMethods.prepareEngine()

        try {
            while (!Thread.currentThread().isInterrupted) {
                while (queue.isEmpty())
                    Thread.sleep(50)

                val task = queue.first
                task()
            }
        } catch (e: InterruptedException) {
        }
    }

    private val queue = ArrayDeque<() -> Unit>()

    private fun task(runnable: () -> Unit) {
        queue.addLast(runnable)
    }

    override fun getMoves(position: Position, x: Int, y: Int): Array<Move> {
        var whiteCheckers = 0
        var blackCheckers = 0
        var blackQueens = 0
        var whiteQueens = 0

        for (i in BOARD_RANGE)
            for (j in BOARD_RANGE) {
                val bit = BITS[i][j]
                when (position.board[i][j]) {
                    WhiteChecker -> whiteCheckers = whiteCheckers setBit bit
                    BlackChecker -> blackCheckers = blackCheckers setBit bit
                    WhiteQueen -> whiteQueens = whiteQueens setBit bit
                    BlackQueen -> blackQueens = blackQueens setBit bit
                }
            }

        val moves = ShortArray(200)
        val lock = Object()

        task {
            NativeMethods.getAvailableMoves(whiteCheckers, blackCheckers, whiteQueens, blackQueens, BITS[x][y], moves)
            lock.notify()
        }

        lock.wait()

        val movesList = ArrayList<Move>()

        for (move in moves) {
            if (move == END_MOVES_FLAG)
                break
            movesList.add(NativeMove(move))
        }

        return movesList.toTypedArray()
    }

    private val BITS = arrayOf(
        arrayOf(0, -1, 3, -1, 8, -1, 15, -1),
        arrayOf(-1, 2, -1, 7, -1, 14, -1, 22),
        arrayOf(1, -1, 6, -1, 13, -1, 21, -1),
        arrayOf(-1, 5, -1, 12, -1, 20, -1, 27),
        arrayOf(4, -1, 11, -1, 19, -1, 26, -1),
        arrayOf(-1, 10, -1, 18, -1, 25, -1, 30),
        arrayOf(9, -1, 17, -1, 24, -1, 29, -1),
        arrayOf(-1, 16, -1, 23, -1, 28, -1, 31)
    )
}