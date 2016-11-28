package br.com.ilegratest.jnitest;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        String text = "";

        TextView textView1 = (TextView) findViewById(R.id.txt1);
        textView1.setText(text);
    }

    static {
        System.loadLibrary("jni-test");
    }

    public native String testaEnvioObjeto(Fulano fulano);

    public native void testString(String input);

    public native int testInt(Fulano fulano);

    public native Fulano testaBuscaObjeto(int age);

    public native String testaEnvioObjetoString(Fulano fulano);

    public native String enviaFulano(Fulano fulano, String name);

    public native void listaProdutosIndicados();
}
