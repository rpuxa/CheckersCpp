package ru.rpuxa.checkerscpp.game

import ru.rpuxa.checkerscpp.game.board.Move

class Human(private val humanController: HumanController) : Player {

    init {
        humanController.human = this
    }

    private lateinit var executor: CommandExecutor
    private var lock = true

    override fun onMove(executor: CommandExecutor, game: Game) {
        this.executor = executor
        humanController.canMove = true
        humanController.lastMove = null
        humanController.isHumanColorWhite = game.isTurnWhite
        lock = true
        while (lock)
            Thread.sleep(50)
    }

    fun makeMove(move: Move, multiTake: Boolean) {
        if (!multiTake)
            humanController.canMove = false
        executor.move(move, multiTake)
        if (!multiTake)
            lock = false
    }
}