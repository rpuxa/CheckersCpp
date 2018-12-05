@file:Suppress("INTEGER_OVERFLOW")

package ru.rpuxa.checkerscpp.views

import android.content.Context
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.util.AttributeSet
import android.view.MotionEvent
import android.view.View
import org.jetbrains.anko.runOnUiThread
import ru.rpuxa.checkerscpp.game.GameVisualizer
import ru.rpuxa.checkerscpp.game.Human
import ru.rpuxa.checkerscpp.game.HumanController
import ru.rpuxa.checkerscpp.game.board.Move
import ru.rpuxa.checkerscpp.game.board.Position
import ru.rpuxa.checkerscpp.game.board.Position.Companion.BOARD_RANGE
import ru.rpuxa.checkerscpp.game.board.WhiteQueen
import ru.rpuxa.checkerscpp.natives.NativeEngine
import kotlin.math.min

private const val BLACK_CELLS_COLOR = 0xb48861 or (0xff00_00_0 * 16)
private const val WHITE_CELLS_COLOR = 0xefd9b4 or (0xff00_00_0 * 16)


class BoardView(context: Context, attrs: AttributeSet) :
    View(context, attrs),
    GameVisualizer,
    HumanController {

    private var position: Position? = null
    private var moves: Array<Move>? = null
    override lateinit var human: Human


    init {
        position = Position().apply {
            board[1][1] = WhiteQueen
        }
    }

    override fun onMeasure(widthMeasureSpec: Int, heightMeasureSpec: Int) {
        val min = min(widthMeasureSpec, heightMeasureSpec)
        setMeasuredDimension(min, min)
    }

    private val paint = Paint()

    override fun onDraw(canvas: Canvas) {
        val cellSize = width / 8f

        for (x in BOARD_RANGE)
            for (y in BOARD_RANGE) {
                paint.color = if ((x + y) % 2 == 0) WHITE_CELLS_COLOR else BLACK_CELLS_COLOR
                val canvasX1 = x * cellSize
                val canvasY1 = y * cellSize
                val canvasX2 = canvasX1 + cellSize
                val canvasY2 = canvasY1 + cellSize

                canvas.drawRect(canvasX1, canvasY1, canvasX2, canvasY2, paint)
            }

        paint.color = Color.GREEN

        val moves = this.moves
        if (moves != null)
            for (move in moves) {
                val canvasX1 = move.to.x * cellSize + cellSize / 2
                val canvasY1 = (7 - move.to.y) * cellSize + cellSize / 2

                canvas.drawCircle(canvasX1, canvasY1, cellSize / 4, paint)
            }

    }


    override fun onTouchEvent(event: MotionEvent): Boolean {
        val cellSize = width / 8f

        if (event.action == MotionEvent.ACTION_DOWN) {
            val x = (event.x / cellSize).toInt()
            val y = 7 - (event.y / cellSize).toInt()

            if ((x + y) % 2 == 0) {
                moves = NativeEngine.getMoves(position!!, x, y)
                invalidate()
            }
        }

        return true
    }

    override fun onGameStarted() {
    }

    override fun onMove(move: Move, currentPosition: Position) {
        moves = null
        position = currentPosition
        context.runOnUiThread {
            invalidate()
        }
    }

}