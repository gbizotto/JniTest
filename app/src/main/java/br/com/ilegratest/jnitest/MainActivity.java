package br.com.ilegratest.jnitest;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        ((TextView) findViewById(R.id.jni_msgView)).setText(getMsgFromJni());

        ((TextView) findViewById(R.id.jni_msgView_2)).setText(String.valueOf(getIntFromJni()));

        ((TextView) findViewById(R.id.jni_msgView_3)).setText(String.valueOf(getSumFromJni(25,78)));
    }

    static {
        System.loadLibrary("jni-test");
    }

    public native String getMsgFromJni();
    public native int getIntFromJni();
    public native int getSumFromJni(int num1, int num2);
}