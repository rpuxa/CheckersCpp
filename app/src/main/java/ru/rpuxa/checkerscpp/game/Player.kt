package ru.rpuxa.checkerscpp.game

interface Player {

    fun onMove(executor: CommandExecutor, game: Game)
}