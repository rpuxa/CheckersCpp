package ru.rpuxa.checkerscpp.game

import ru.rpuxa.checkerscpp.natives.NativeEngine

class Bot : Player {
    override fun onMove(executor: CommandExecutor, game: Game) {
        executor.move(
            NativeEngine.getBestMove(game.position, game.isTurnWhite, 0)
        )
    }
}