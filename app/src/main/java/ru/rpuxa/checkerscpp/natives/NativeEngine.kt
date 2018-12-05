package ru.rpuxa.checkerscpp.natives

import ru.rpuxa.checkerscpp.game.board.*
import ru.rpuxa.checkerscpp.game.board.Position.Companion.BOARD_RANGE
import ru.rpuxa.checkerscpp.natives.NativeMove.Companion.END_MOVES_FLAG
import java.util.*
import kotlin.concurrent.thread

object NativeEngine {

    private val mainThread = thread {
        NativeMethods.prepareEngine()

        try {
            while (!Thread.currentThread().isInterrupted) {
                while (queue.isEmpty())
                    Thread.sleep(50)

                val task = queue.pollFirst()
                task()
            }
        } catch (e: InterruptedException) {
        }
    }

    private val queue = ArrayDeque<() -> Unit>()

    private fun task(runnable: () -> Unit) {
        var wait = true

        queue.addLast {
            runnable()
            wait = false
        }
        while (wait)
            Thread.sleep(1)
    }

    fun getMoves(position: Position, x: Int, y: Int): Array<Move> {
        val (whiteCheckers, blackCheckers, blackQueens, whiteQueens) =
                position.toNative()


        val moves = ShortArray(200)

        task {
            NativeMethods.getAvailableMoves(whiteCheckers, blackCheckers, whiteQueens, blackQueens, BITS[x][y], moves)
        }

        val movesList = ArrayList<Move>()

        for (move in moves) {
            if (move == END_MOVES_FLAG)
                break
            movesList.add(NativeMove(move))
        }

        return movesList.toTypedArray()
    }

    fun makeMove(position: Position, move: Move) {
        val (whiteCheckers, blackCheckers, whiteQueens, blackQueens) =
                position.toNative()
        val nativeMove = (BITS[move.from.x][move.from.y] shl 1) or
                (BITS[move.to.x][move.to.y] shl 6)
        val pos = IntArray(4)
        task {
            NativeMethods.makeMove(whiteCheckers, blackCheckers, whiteQueens, blackQueens, nativeMove.toShort(), pos)
        }

        val (wc, bc, wq, bq) = pos
        position.setFromNative(wc, bc, wq, bq)
    }

    private fun Position.toNative(): IntArray {
        var whiteCheckers = 0
        var blackCheckers = 0
        var blackQueens = 0
        var whiteQueens = 0

        for (i in BOARD_RANGE)
            for (j in BOARD_RANGE) {
                val bit = BITS[i][j]
                when (board[i][j]) {
                    WhiteChecker -> whiteCheckers = whiteCheckers setBit bit
                    BlackChecker -> blackCheckers = blackCheckers setBit bit
                    WhiteQueen -> whiteQueens = whiteQueens setBit bit
                    BlackQueen -> blackQueens = blackQueens setBit bit
                }
            }

        return intArrayOf(whiteCheckers, blackCheckers, whiteQueens, blackQueens)
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