package ru.rpuxa.checkerscpp

import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import kotlinx.android.synthetic.main.activity_main.*
import ru.rpuxa.checkerscpp.game.Bot
import ru.rpuxa.checkerscpp.game.Game
import ru.rpuxa.checkerscpp.game.Human
import ru.rpuxa.checkerscpp.game.board.Position
import ru.rpuxa.checkerscpp.natives.NativeEngine

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        NativeEngine.prepareEngine()

        val bot = Bot()
        val human = Human(game_board)

        Game(
            Position.createStartPosition(),
            true,
            human,
            bot,
            game_board
        ).start()
    }

}
