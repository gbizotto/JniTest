package br.com.ilegratest.jnitest;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    static {
        try {
            System.loadLibrary("jni-test");

            Log.v(MainActivity.class.getSimpleName(), "conseguiu carregar lib");

        }catch(UnsatisfiedLinkError e) {
            Log.e(MainActivity.class.getSimpleName(), "Native code library failed to load.\n" + e.getMessage(), e);
        }
        catch(Exception e) {
            Log.e(MainActivity.class.getSimpleName(), "Exception: " + e.getMessage(), e);
        }

    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        String text = "";

        TextView textView1 = (TextView) findViewById(R.id.txt1);

        String json = " {\n" +
                " \t\"Perfil\" :\n" +
                " \t{\n" +
                "\t\t\"fumante\": 2,\n" +
                "\t\t\"idd\": 54,\n" +
                "\t\t\"codigoProfissao\": 1177,\n" +
                "\t\t\"profCLT\": \"S\",\n" +
                "\t\t\"sexo\": 0,\n" +
                "\t\t\"vigenciaInicial\": 365\n" +
                "\t}\n" +
                "}";


        text = enviaJson(json);

        textView1.setText(text);
    }


    public native String enviaJson(String json);
}
