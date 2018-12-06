package ru.rpuxa.checkerscpp.game

import ru.rpuxa.checkerscpp.game.board.Move
import ru.rpuxa.checkerscpp.game.board.Position
import ru.rpuxa.checkerscpp.natives.NativeEngine
import kotlin.concurrent.thread

class Game(
    val position: Position,
    isTurnWhite: Boolean,
    private val whitePlayer: Player,
    private val blackPlayer: Player,
    private var gameVisualizer: GameVisualizer
) {
    var isTurnWhite: Boolean = isTurnWhite
        private set

    var gameEnd = false

    fun start() {
        val whitePlayerExecutor = PlayerExecutor(whitePlayer)
        val blackPlayerExecutor = PlayerExecutor(blackPlayer)
        gameVisualizer.position = position
        thread {
            while (!gameEnd) {
                if (isTurnWhite)
                    whitePlayer.onMove(whitePlayerExecutor, this)
                else
                    blackPlayer.onMove(blackPlayerExecutor, this)
            }
        }
    }

    private inner class PlayerExecutor(val sender: Player) : CommandExecutor {
        override fun move(move: Move, multiTake: Boolean) {
            if (whitePlayer === sender != isTurnWhite)
                throw IllegalStateException("Move out of turn")
            NativeEngine.makeMove(position, move)
            gameVisualizer.onMove(move, position)
            if (!multiTake)
                isTurnWhite = !isTurnWhite
        }

        override fun resign() {
        }

        override fun offerDraw() {
        }
    }
}