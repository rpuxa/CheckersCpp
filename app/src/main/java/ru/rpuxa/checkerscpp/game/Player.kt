package ru.rpuxa.checkerscpp.game

interface Player {

    fun CommandExcecutor.onMove(game: Game)
}