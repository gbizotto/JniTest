#include "Perfil.h"

Perfil::Perfil() : fumante(0), grupo(0), idade(0), regime(0), sexo(0), vigencia(0), segregacao(0), profissao(0) {

}

Perfil::~Perfil() {

}

void Perfil::determinaFatorDeSegregacao() {
	if(getSexo() == MASCULINO) {
		if(getFumante() == SIM) {
			this->segregacao = FATOR_A;
		}
		else
			this->segregacao = FATOR_B;
	}
	else if(getSexo() == FEMININO) {
		if(getFumante() == SIM) {
			this->segregacao = FATOR_C;
		}
		else
			this->segregacao = FATOR_D;
	}
}
