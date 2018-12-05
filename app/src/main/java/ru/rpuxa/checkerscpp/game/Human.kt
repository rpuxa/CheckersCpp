package ru.rpuxa.checkerscpp.game

import ru.rpuxa.checkerscpp.game.board.Move

class Human(private val humanController: HumanController) : Player {

    init {
        humanController.human = this
    }

    private lateinit var executor: CommandExecutor

    override fun onMove(executor: CommandExecutor, game: Game) {
        this.executor = executor
        humanController.canMove = true
    }

    fun makeMove(move: Move) {
        humanController.canMove = false
        executor.move(move)
    }
}