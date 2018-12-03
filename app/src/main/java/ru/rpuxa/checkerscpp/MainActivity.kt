package ru.rpuxa.checkerscpp

import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import ru.rpuxa.checkerscpp.game.board.Position

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val position = Position()
    }

}
