#ifndef PERFIL_H_
#define PERFIL_H_

#include <cstring>

class Perfil {
public:
	Perfil();
	virtual ~Perfil();

	/*!
	 *	\brief Determina o fator de segregação
	 * + A - Sexo Masculino e Fumante
	 * + B - Sexo Masculino e não Fumante
	 * + C - Sexo Feminino e Fumante
	 * + D - Sexo Feminino e não Fumante
	 *	\return void
	*/
	void determinaFatorDeSegregacao();

	int getFumante() const {
		return fumante;
	}

	void setFumante(int fumante) {
		this->fumante = fumante;
	}

	int getGrupo() const {
		return grupo;
	}

	void setGrupo(int grupo) {
		this->grupo = grupo;
	}

	int getIdade() const {
		return idade;
	}

	void setIdade(int idade) {
		this->idade = idade;
	}

	int getSexo() const {
		return sexo;
	}

	void setSexo(int sexo) {
		this->sexo = sexo;
	}

	int getRegime() const {
		return regime;
	}

	void setRegime(int regime) {
		this->regime = regime;
	}

	int getVigencia() const {
		return vigencia;
	}

	void setVigencia(int vigencia) {
		this->vigencia = vigencia;
	}

	int getSegregacao() const {
		return segregacao;
	}

	enum tempo { VIGENCIA_MINIMA = 365, IDADE_MAXIMA = 65, IDADE_MAXIMA_APICP = 71 };
    enum sexo_tipo { FEMININO = 1, MASCULINO };
    enum { SIM = 1, NAO };
    enum regime_contrato { CLT = 1, PJ };
    enum fator_segregacao { FATOR_A = 1, FATOR_B, FATOR_C, FATOR_D };

private:
    int fumante; 			/*!< Cliente é fumante? SIM ou NAO */
    int grupo;				/*!< Grupo DIT conforme a profissão recebida. A, B, blank */
	int idade;				/*!< Idade do cliente em anos. */
	int regime;				/*!< Regime de contrato. CLT ou PJ */
    int sexo;				/*!< Sexo do cliente. FEMININO OU MASCULINO */
    int vigencia;			/*!< Vigência do contrato. */
    int segregacao;			/*!< Fator de segregacao. */
};

#endif /* PERFIL_H_ */
