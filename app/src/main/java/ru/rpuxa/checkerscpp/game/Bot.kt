package ru.rpuxa.checkerscpp.game

import android.util.Log
import ru.rpuxa.checkerscpp.natives.NativeEngine

class Bot : Player {
    override fun onMove(executor: CommandExecutor, game: Game) {
        val startTime = System.currentTimeMillis()
        val moves = NativeEngine.getBestMove(game.position, game.isTurnWhite, 6)
        val time = System.currentTimeMillis() - startTime
        Log.i("Bot", "$time ms")
        for ((index, move) in moves.withIndex())
            executor.move(
                move, index != moves.lastIndex
            )
    }
}