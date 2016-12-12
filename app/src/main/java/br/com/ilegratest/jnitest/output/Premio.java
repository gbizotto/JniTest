package br.com.ilegratest.jnitest.output;

/**
 * Created by gabrielabizotto on 12/12/16.
 */

public class Premio {
    private CalculosVida calculosVida;

    /**
     *
     * @return
     * The calculosVida
     */
    public CalculosVida getCalculosVida() {
        return calculosVida;
    }

    /**
     *
     * @param calculosVida
     * The calculosVida
     */
    public void setCalculosVida(CalculosVida calculosVida) {
        this.calculosVida = calculosVida;
    }

    @Override
    public String toString() {
        return "Premio{" +
                "calculosVida=" + calculosVida +
                '}';
    }
}
