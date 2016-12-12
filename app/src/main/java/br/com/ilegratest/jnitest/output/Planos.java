package br.com.ilegratest.jnitest.output;

import java.util.List;

/**
 * Created by gabrielabizotto on 12/12/16.
 */

public class Planos {
    private List<Produto> produtos = null;

    /**
     *
     * @return
     * The produtos
     */
    public List<Produto> getProdutos() {
        return produtos;
    }

    /**
     *
     * @param produtos
     * The produtos
     */
    public void setProdutos(List<Produto> produtos) {
        this.produtos = produtos;
    }

    @Override
    public String toString() {
        return "Planos{" +
                "produtos=" + produtos +
                '}';
    }
}
