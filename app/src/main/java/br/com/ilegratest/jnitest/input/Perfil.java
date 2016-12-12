package br.com.ilegratest.jnitest.input;

/**
 * Created by gabrielabizotto on 12/12/16.
 */

public class Perfil {

    private String dataNascimento;
    private Integer sexo;
    private Integer fumante;
    private Integer codigoProfissao;
    private String profCLT;
    private Integer vigenciaInicial;

    /**
     *
     * @return
     * The dataNascimento
     */
    public String getDataNascimento() {
        return dataNascimento;
    }

    /**
     *
     * @param dataNascimento
     * The dataNascimento
     */
    public void setDataNascimento(String dataNascimento) {
        this.dataNascimento = dataNascimento;
    }

    /**
     *
     * @return
     * The sexo
     */
    public Integer getSexo() {
        return sexo;
    }

    /**
     *
     * @param sexo
     * The sexo
     */
    public void setSexo(Integer sexo) {
        this.sexo = sexo;
    }

    /**
     *
     * @return
     * The fumante
     */
    public Integer getFumante() {
        return fumante;
    }

    /**
     *
     * @param fumante
     * The fumante
     */
    public void setFumante(Integer fumante) {
        this.fumante = fumante;
    }

    /**
     *
     * @return
     * The codigoProfissao
     */
    public Integer getCodigoProfissao() {
        return codigoProfissao;
    }

    /**
     *
     * @param codigoProfissao
     * The codigoProfissao
     */
    public void setCodigoProfissao(Integer codigoProfissao) {
        this.codigoProfissao = codigoProfissao;
    }

    /**
     *
     * @return
     * The profCLT
     */
    public String getProfCLT() {
        return profCLT;
    }

    /**
     *
     * @param profCLT
     * The profCLT
     */
    public void setProfCLT(String profCLT) {
        this.profCLT = profCLT;
    }

    /**
     *
     * @return
     * The vigenciaInicial
     */
    public Integer getVigenciaInicial() {
        return vigenciaInicial;
    }

    /**
     *
     * @param vigenciaInicial
     * The vigenciaInicial
     */
    public void setVigenciaInicial(Integer vigenciaInicial) {
        this.vigenciaInicial = vigenciaInicial;
    }

}
