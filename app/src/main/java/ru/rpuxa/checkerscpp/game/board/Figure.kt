package ru.rpuxa.checkerscpp.game.board

sealed class Figure(val isWhite: Boolean)

open class Checker(isWhite: Boolean) : Figure(isWhite)

object WhiteChecker : Checker(true)

object BlackChecker : Checker(false)

open class Queen(isWhite: Boolean) : Figure(isWhite)

object WhiteQueen : Queen(true)

object BlackQueen : Queen(false)

object NullFigure : Figure(false)