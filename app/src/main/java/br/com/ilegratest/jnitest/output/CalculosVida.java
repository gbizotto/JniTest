package br.com.ilegratest.jnitest.output;

/**
 * Created by gabrielabizotto on 12/12/16.
 */

public class CalculosVida {
    private String premioTotal;

    /**
     *
     * @return
     * The premioTotal
     */
    public String getPremioTotal() {
        return premioTotal;
    }

    /**
     *
     * @param premioTotal
     * The premioTotal
     */
    public void setPremioTotal(String premioTotal) {
        this.premioTotal = premioTotal;
    }

    @Override
    public String toString() {
        return "CalculosVida{" +
                "premioTotal='" + premioTotal + '\'' +
                '}';
    }
}
