package ru.rpuxa.checkerscpp.engine

import ru.rpuxa.checkerscpp.game.board.Move
import ru.rpuxa.checkerscpp.game.board.Position

interface Engine {

    fun getMoves(position: Position, x: Int, y: Int): Array<Move>
}