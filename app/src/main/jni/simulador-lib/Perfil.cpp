#include "Perfil.h"

Perfil::Perfil() : fumante(0), grupo(0), idade(0), regime(0), sexo(0), vigencia(0), segregacao(0) {

}

Perfil::~Perfil() {

}

void Perfil::determinaFatorDeSegregacao() {
	if(this->sexo == MASCULINO) {
		if(this->fumante == SIM) {
			this->segregacao = FATOR_A;
		}
		else
			this->segregacao = FATOR_B;
	}
	else if(sexo == FEMININO) {
		if(this->fumante == SIM) {
			this->segregacao = FATOR_C;
		}
		else
			this->segregacao = FATOR_D;
	}
}
