package ru.rpuxa.checkerscpp.views

import android.content.Context
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.util.AttributeSet
import android.view.SurfaceHolder
import android.view.SurfaceView
import ru.rpuxa.checkerscpp.game.board.Move
import ru.rpuxa.checkerscpp.game.board.Position
import ru.rpuxa.checkerscpp.game.board.Position.Companion.BOARD_RANGE
import ru.rpuxa.checkerscpp.game.board.WhiteQueen
import ru.rpuxa.checkerscpp.natives.NativeEngine
import kotlin.concurrent.thread
import kotlin.math.min

private const val BLACK_CELLS_COLOR = Color.RED
private const val WHITE_CELLS_COLOR = Color.BLUE


class BoardView(context: Context, attrs: AttributeSet) :
    SurfaceView(context, attrs),
    SurfaceHolder.Callback {

    private val lock = Object()
    private lateinit var drawingThread: Thread
    var position: Position? = null
    private var moves: Array<Move>? = null

    init {
        holder.addCallback(this)
        position = Position().apply {
            board[3][3] = WhiteQueen
        }
        moves = NativeEngine.getMoves(position!!, 3, 3)
    }


    override fun surfaceCreated(holder: SurfaceHolder?) {
        drawingThread = thread {
            try {
                while (!drawingThread.isInterrupted) {
                    synchronized(lock) {
                        val canvas = holder!!.lockCanvas(null)
                        synchronized(holder) {
                            drawCanvas(canvas)
                        }
                        holder.unlockCanvasAndPost(canvas)
                        lock.wait()
                    }
                }
            } catch (e: InterruptedException) {
            }
        }

    }

    override fun onMeasure(widthMeasureSpec: Int, heightMeasureSpec: Int) {
        val min = min(widthMeasureSpec, heightMeasureSpec)
        setMeasuredDimension(min, min)
    }

    private val paint = Paint()

    private fun drawCanvas(canvas: Canvas) {
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
                val canvasY1 = move.to.y * cellSize + cellSize / 2

                canvas.drawCircle(canvasX1, canvasY1, cellSize / 4, paint)
            }

    }

    fun update() {
        lock.notify()
    }

    override fun surfaceChanged(holder: SurfaceHolder?, format: Int, width: Int, height: Int) {
    }

    override fun surfaceDestroyed(holder: SurfaceHolder?) {
        drawingThread.interrupt()
    }
}