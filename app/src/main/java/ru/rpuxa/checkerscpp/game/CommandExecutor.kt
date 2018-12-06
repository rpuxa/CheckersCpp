package ru.rpuxa.checkerscpp.game

import ru.rpuxa.checkerscpp.game.board.Move

interface CommandExecutor {
    fun move(move: Move, multiTake: Boolean)

    fun resign()

    fun offerDraw()


}