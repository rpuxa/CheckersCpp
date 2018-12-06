package ru.rpuxa.checkerscpp.natives

import ru.rpuxa.checkerscpp.game.board.*
import ru.rpuxa.checkerscpp.game.board.Position.Companion.BOARD_RANGE
import ru.rpuxa.checkerscpp.natives.NativeMove.Companion.END_MOVES_FLAG
import java.util.*
import kotlin.concurrent.thread

const val MAX_MOVES_COUNT = 200

object NativeEngine {


    private lateinit var mainThread: Thread

    fun prepareEngine() {
        mainThread = thread {
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

    fun getMoves(position: Position, x: Int, y: Int): Pair<Array<Move>, Boolean> {
        val moves = ShortArray(MAX_MOVES_COUNT)
        getNativeMoves(position, x, y, moves)

        val movesList = ArrayList<Move>()

        var multiTake = false

        label@
        for (move in moves) {
            if (move == END_MOVES_FLAG)
                break
            val nativeMove = NativeMove(move)
            if (nativeMove.from.x != x || nativeMove.from.y != y)
                continue
            if (nativeMove.isTake) {
                val secondaryPosition = position.clone()
                makeMove(secondaryPosition, nativeMove)
                val secondaryMoves = ShortArray(MAX_MOVES_COUNT)
                getNativeMoves(secondaryPosition, nativeMove.to.x, nativeMove.to.y, secondaryMoves)
                val nativeMove1 = NativeMove(secondaryMoves[0])
                if (secondaryMoves[0] != END_MOVES_FLAG && nativeMove1.isTake && !nativeMove1.isOpposite(nativeMove)) {
                    if (!multiTake)
                        movesList.clear()
                    multiTake = true
                }

                for (secondaryMove in secondaryMoves) {
                    if (secondaryMove == END_MOVES_FLAG)
                        break
                    val nativeSecondaryMove = NativeMove(secondaryMove)
                    val opposite = nativeSecondaryMove.isOpposite(nativeMove)
                    if (multiTake && (!nativeSecondaryMove.isTake || opposite))
                        continue@label
                }
            }

            movesList.add(nativeMove)
        }

        return movesList.toTypedArray() to multiTake
    }

    private fun getNativeMoves(
        position: Position,
        x: Int,
        y: Int,
        movesArray: ShortArray
    ) {
        val (whiteCheckers, blackCheckers, whiteQueens, blackQueens) =
                position.toNative()

        val white = position.board[x][y].isWhite
        task {
            NativeMethods.getAvailableMoves(
                whiteCheckers, blackCheckers, whiteQueens,
                blackQueens, white, movesArray
            )
        }
    }

    fun getBestMove(position: Position, isTurnWhite: Boolean, depth: Int): Array<Move> {
        val (whiteCheckers, blackCheckers, whiteQueens, blackQueens) =
                position.toNative()
        val bestMove = ShortArray(MAX_MOVES_COUNT)
        task {
            NativeMethods.getBestMove(
                whiteCheckers, blackCheckers, whiteQueens,
                blackQueens, isTurnWhite, depth.toShort(),
                bestMove
            )
        }

        val movesList = ArrayList<Move>()

        for (move in bestMove) {
            if (move == END_MOVES_FLAG)
                break
            movesList.add(NativeMove(move))
        }

        return movesList.toTypedArray()
    }

    fun makeMove(position: Position, move: Move) {
        val (whiteCheckers, blackCheckers, whiteQueens, blackQueens) =
                position.toNative()
        val nativeMove =
            (BITS[move.from.x][move.from.y] shl 1) or
                    (BITS[move.to.x][move.to.y] shl 6) or
                    ((if (position.board[move.from.x][move.from.y].isWhite) 1 else 0) shl 11) or
                    ((if (position.board[move.from.x][move.from.y].isQueen) 1 else 0) shl 12)
        val pos = IntArray(4)
        task {
            NativeMethods.makeMove(whiteCheckers, blackCheckers, whiteQueens, blackQueens, nativeMove.toShort(), pos)
        }

        position.setFromNative(pos)
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