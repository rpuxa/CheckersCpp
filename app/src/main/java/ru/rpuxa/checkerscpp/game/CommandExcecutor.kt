package ru.rpuxa.checkerscpp.game

import ru.rpuxa.checkerscpp.game.board.Move

interface CommandExcecutor {
    fun move(move: Move)

    fun resign()

    fun offerDraw()


}