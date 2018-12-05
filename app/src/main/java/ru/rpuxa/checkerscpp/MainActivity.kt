package ru.rpuxa.checkerscpp

import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import ru.rpuxa.checkerscpp.natives.NativeEngine

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        NativeEngine
        setContentView(R.layout.activity_main)
    }

}
