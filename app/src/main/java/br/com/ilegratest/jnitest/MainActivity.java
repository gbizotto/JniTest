package br.com.ilegratest.jnitest;

import android.os.Build;
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
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP){
            String[] v = Build.SUPPORTED_ABIS;
            for (int i=0;i<v.length;i++){
                text += v[i] + "\n";
            }
        }



        TextView textView1 = (TextView) findViewById(R.id.txt1);
        textView1.setText(text);


        String json = " {\n" +
                " \t\"Perfil\" :\n" +
                " \t{\n" +
                "\t\t\"fumante\": 2,\n" +
                "\t\t\"idd\": 54,\n" +
                "\t\t\"codigoProfissao\": 1177,\n" +
                "\t\t\"profCLT\": \"S\",\n" +
                "\t\t\"sexo\": 1,\n" +
                "\t\t\"vigenciaInicial\": 365\n" +
                "\t}\n" +
                "}";


        text = enviaJson(json);

        textView1.setText(text);
    }



    public native String testaEnvioObjeto(Fulano fulano);

    public native void testString(String input);

    public native int testInt(Fulano fulano);

    public native Fulano testaBuscaObjeto(int age);

    public native String testaEnvioObjetoString(Fulano fulano);

    public native String enviaFulano(Fulano fulano, String name);

    public native String enviaJson(String json);
}
