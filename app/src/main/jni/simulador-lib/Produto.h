#ifndef PRODUTO_H_
#define PRODUTO_H_

//#include "Tabela.h"

class Produto {
public:
	Produto();
	virtual ~Produto();

	double getCobertura_morte() const {
		return cobertura_morte;
	}

	void setCobertura_morte(double cobertura_morte) {
		this->cobertura_morte = cobertura_morte;
	}

	double getCobertura_map() const {
		return cobertura_map;
	}

	void setCobertura_map(double cobertura_map) {
		this->cobertura_map = cobertura_map;
	}

	double getCobertura_ipa() const {
		return cobertura_ipa;
	}

	void setCobertura_ipa(double cobertura_ipa) {
		this->cobertura_ipa = cobertura_ipa;
	}

	int getFuneral_familia() const {
		return funeral_familia;
	}

	void setFuneral_familia(int funeral_familia) {
		this->funeral_familia = funeral_familia;
	}

	double getCobertura_aff() const {
		return cobertura_aff;
	}

	void setCobertura_aff(double cobertura_aff) {
		this->cobertura_aff = cobertura_aff;
	}

	double getCobertura_aed() const {
		return cobertura_aed;
	}

	void setCobertura_aed(double cobertura_aed) {
		this->cobertura_aed = cobertura_aed;
	}

	double getDiaria_it() const {
		return diaria_it;
	}

	void setDiaria_it(int diaria_it) {
		this->diaria_it = diaria_it;
	}

	double getDespesas_medicas() const {
		return despesas_medicas;
	}

	void setDespesas_medicas(double despesas_medicas) {
		this->despesas_medicas = despesas_medicas;
	}

	double getDoencas_graves() const {
		return doencas_graves;
	}

	void setDoencas_graves(double doencas_graves) {
		this->doencas_graves = doencas_graves;
	}

	int getMajoracao() const {
		return majoracao;
	}

	void setMajoracao(int majoracao) {
		this->majoracao = majoracao;
	}

	double getIof() const {
		return IOF;
	}

	void setIof(double iof) {
		IOF = double(iof / 100.0);
	}

	double getPremio() const {
		return premio;
	}

	void setPremio(double premio) {
		this->premio = premio;
	}

	int getTipo() const {
		return tipo;
	}

	void setTipo(int tipo) {
		this->tipo = tipo;
	}

	int getFranquia_reduzida() const {
		return franquia_reduzida;
	}

	void setFranquia_reduzida(int franquia_reduzida) {
		this->franquia_reduzida = franquia_reduzida;
	}

	int getOperacao() const {
		return operacao;
	}

	void setOperacao(int operacao) {
		this->operacao = operacao;
	}

	int getTipo_doenca_grave() const {
		return tipo_doenca_grave;
	}

	void setTipo_doenca_grave(int tipo_doenca_grave) {
		this->tipo_doenca_grave = tipo_doenca_grave;
	}

    //enum segregacao { A = 1, B, C, D };
    //enum { SIM = 1, NAO };

private:
    /*
    Vida Mais Simples (VMS)
    Acidentes Pessoais Individual Prazo Curto (APICP)
    Vida Mais Mulher (VMM)
    Acidentes Pessoais Individual Plus (APIP)
    Vida Individual (VI)
    */
    double cobertura_morte;		/*!< Cobertura por morte natural */
    double cobertura_map;		/*!< Morte acidental (DOBRO?) */
    double cobertura_ipa;		/*!< Invalidez por acidente */
    int funeral_familia;		/*!< Assistência funeral familiar? SIM ou NAO */
    double cobertura_aff;		/*!< Cobertura assistência funeral */
    double cobertura_aed;		/*!< Antecipação especial por doença */
    double diaria_it;			/*!< Diária por incapacidade temporária */
    double despesas_medicas;	/*!< Despesas médico hospitalares */
    double doencas_graves;		/*!< Despesas médico hospitalares */
    int majoracao;				/*!< Majoração SIM = cobertura_ipa */
    int franquia_reduzida;		/*!< Franquia reduzida? SIM ou NAO */
    int operacao;				/*!< Código da operação. */
    int tipo_doenca_grave;		/*!< Tipo doença grave 6 ou 10 tipos. */

    double IOF;
    double premio;
    int tipo;
};

#endif /* PRODUTO_H_ */
