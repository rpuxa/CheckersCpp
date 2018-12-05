package ru.rpuxa.checkerscpp.game

import ru.rpuxa.checkerscpp.game.board.Move
import ru.rpuxa.checkerscpp.game.board.Position

interface GameVisualizer {

    fun onGameStarted()

    fun onMove(move: Move, currentPosition: Position)
}