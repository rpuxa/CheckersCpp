package ru.rpuxa.checkerscpp.game

import ru.rpuxa.checkerscpp.game.board.Position

class Game(
    val position: Position,
    isTurnWhite: Boolean,
    val whitePlayer: Player,
    val blackPlayer: Player
) {
    var isTurnWhite: Boolean = isTurnWhite
        private set

    fun start() {

    }
}