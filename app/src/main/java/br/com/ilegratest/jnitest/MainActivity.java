package br.com.ilegratest.jnitest;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import com.google.gson.Gson;

import br.com.ilegratest.jnitest.input.Perfil;
import br.com.ilegratest.jnitest.input.Produto;
import br.com.ilegratest.jnitest.output.Planos;
import br.com.ilegratest.jnitest.output.Premio;

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

        String retorno = "";

        TextView textView1 = (TextView) findViewById(R.id.txt1);

        Perfil perfil = new Perfil();
        perfil.setCodigoProfissao(126);
        perfil.setDataNascimento("08/05/1982");
        perfil.setFumante(2);
        perfil.setProfCLT("S");
        perfil.setVigenciaInicial(365);
        perfil.setSexo(2);

        Gson gson = new Gson();
        String perfilString = gson.toJson(perfil);
        Log.v(MainActivity.class.getSimpleName(), "perfilString = " + perfilString);


        Produto produto = new Produto();
        produto.setAssistFuneralFamiliar("N");
        produto.setCobAssistenciaFuneral(5000);
        produto.setCobInvalidezAcidente(50000);
        produto.setCobMorte(50000);
        produto.setCobMorteAcidental(25000);
        produto.setCodOperacao(10);
        produto.setDominio(33);

        String produtoString = gson.toJson(produto);

        retorno = enviaJson(perfilString);

        Planos planos = gson.fromJson(retorno, Planos.class);
        Log.v(MainActivity.class.getSimpleName(), "------");
        Log.v(MainActivity.class.getSimpleName(), planos.toString());

        Log.v(MainActivity.class.getSimpleName(), retorno);

        textView1.setText(retorno);

        TextView textView2 = (TextView) findViewById(R.id.txt2);
        String premio = gravaPremio(perfilString,produtoString);

        Premio premio1= gson.fromJson(premio, Premio.class);

        Log.v(MainActivity.class.getSimpleName(), "------");
        Log.v(MainActivity.class.getSimpleName(), premio1.toString());

        textView2.setText(premio1.getCalculosVida().getPremioTotal());

        Log.v(MainActivity.class.getSimpleName(), premio);
    }


    public native String enviaJson(String json);

    public native String gravaPremio(String jsonPerfil, String jsonProduto);
}
