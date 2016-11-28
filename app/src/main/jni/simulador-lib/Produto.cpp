//! Define um produto com alguns de seus atributos e calcula os prêmios
/*!
  + VMS Vida Mais Simples
  + Acidentes Pessoais Individual Curto Prazo
  + Vida Mais Mulher
  + Acidentes Pessoais Individual Plus
  + Vida Individual
*/

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include "Produto.h"
#include "math.h"

using namespace std;

//! Construtor
Produto::Produto() :
	cobertura_morte(0.0),
	cobertura_map(0.0),
	cobertura_ipa(0.0),
	funeral_familia(0),
	cobertura_aff(0.0),
	cobertura_aed(0.0),
	diaria_it(0.0),
	despesas_medicas(0.0),
	doencas_graves(0.0),
	majoracao(0),
	franquia_reduzida(0),
	operacao(0),
	tipo_doenca_grave(0),
	IOF(0.0),
	premio(0.0),
	tipo(0)
{

}

//! Destrutor
Produto::~Produto() {

}
