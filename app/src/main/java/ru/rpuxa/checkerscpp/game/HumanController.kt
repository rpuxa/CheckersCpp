package ru.rpuxa.checkerscpp.game

import ru.rpuxa.checkerscpp.game.board.Move

interface HumanController {
    var human: Human
    var canMove: Boolean
    var isHumanColorWhite: Boolean?
    var lastMove: Move?
}