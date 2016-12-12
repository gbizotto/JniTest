#ifndef CALC_SIM_H_
#define CALC_SIM_H_

#include <ctime>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <math.h>
#include <cstdlib>
#include <fstream>
#include <ostream>
#include <stdexcept>

#include "jsoncpp/dist/json/json.h"

#include "Tabela.h"
#include "Perfil.h"
#include "Profissao.h"
#include "Produto.h"
#include "Investidor.h"
#include "Previdencia.h"

using namespace std;

enum {
	SIM = 1, 	/*!< Respostas SIM*/
	NAO		 	/*!< Respostas NAO*/
};

enum {
	VI = 31,	/*!< Vida Individual*/
	APIP,		/*!< Acidentes Pessoais Individual Plus*/
	VMM,		/*!< Vida Mais Mulher*/
	VMS,		/*!< Vida Mais Simples*/
	APICP		/*!< Acidentes Pessoais Individual Curto Prazo*/
};

enum {
	RUBIPLUS = 1,	/*!< Plano Rubi Plus*/
	RUBIPREMIUM,	/*!< Plano Rubi Premium*/
	DIAMANTE,		/*!< Plano Diamante*/
	DIAMANTEPLUS	/*!< Plano Diamante Plus*/
};

enum {
	RENDA_FIXA = 1,		/*!< Renda Fixa*/
	MULTIMERCADO,		/*!< Multimercado*/
	RENDA_FIXA_CP,		/*!< Renda Fixa Cr�dito Privado*/
	RENDA_FIXA_INF,		/*!< Renda Fixa Infla��o*/
	SOBERANO			/*!< Soberano*/
};

enum {
	PGBL = 1,
	VGBL
};

enum {
	MEDICO = 111,
	DENTISTA = 1177
};

enum cod_erros {
					SEM_ERRO = 0,
					VIGENCIA_MENOR = 101,
					VIGENCIA_MAIOR,
					IDADE_MENOR,
					IDADE_MAIOR,
					PROFISSAO_INVALIDA,
					COB_MORTE,
					COB_MAP,
					COB_IPA,
					COB_CANCER,
					COB_AED,
					COB_DMHO,
					COB_DG,
					COB_DIT,
					MAJORACAO_PROFISSAO_DOBRO,
					COB_DIT_INVALIDA,
					FRANQUIA_DIT_INVALIDA,
					CONTRIBUICAO_MINIMA,
					APORTE_MINIMO,
					PRAZO_RMT,
					IDADE_MENORES,
					IDADE_CONJUGE_MIN,
					IDADE_CONJUGE_MAX,
					PRAZO_RENDA_CONJUGE,
					VALOR_MAX_PENSOES,
					IDADE_PARTICIPANTE_MENOR,
					IDADE_PARTICIPANTE_MAIOR,
};

/******************************************************************************************/
/************************ M�todos principais PREVID�NCIA***********************************/
/******************************************************************************************/
/*!
 *  Retorna c�digos de opera��o dispon�veis.
 */
std::string retornaCodigosOperacaoPrevidencia();
/*!
 *  Recebe string JSON com dados do perfil e com os valores contratados e retorna resultados dos c�lculos de previd�ncia.
 */
std::string calculaPrevidencia(const std::string input_string_investidor,const std::string input_string_previdencia);
/*!
 *  Recebe string JSON com dados do perfil do investidor e com valores aferidos e retorna o perfil do investidor,
 *  regime tribut�rio, plano, modalidade e investimentos recomendados.
 */
std::string calculaPerfilInvestidor(const std::string input_string_investidor,const std::string input_string_previdencia);
/******************************************************************************************/
std::string calculaPremiosRiscoVida(Investidor& investidor,Previdencia& previdencia);
std::string calculaPremiosRiscoPrevidencia(Investidor& investidor,Previdencia& previdencia);
double calculaValorFuturo(const Investidor &investidor, const Previdencia &previdencia);
double calculaRMT(double &valor_futuro, const Investidor &investidor, const Previdencia &previdencia);
double calculaRMV(double &valor_futuro, const Investidor &investidor);
int criaInvestidor(const Json::Value& val, Investidor& investidor);
int criaPrevidencia(const Json::Value& val, Previdencia& previdencia);
/******************************************************************************************/

inline int calculaIdade(const Perfil &perfil) {
	time_t data_atual;
	std::string data_str;
	struct tm* inicio_vigencia;
	struct tm data_nascimento;

	time(&data_atual);
	inicio_vigencia = localtime (&data_atual);

	data_str = perfil.getData_nascimento();

	std::istringstream buffer(data_str.substr(0,2));
	buffer >> data_nascimento.tm_mday;
	std::istringstream buffer1(data_str.substr(3,2));
	buffer1 >> data_nascimento.tm_mon; data_nascimento.tm_mon -= 1;
	std::istringstream buffer2(data_str.substr(6,4));
	buffer2 >> data_nascimento.tm_year; data_nascimento.tm_year -= 1900;

	int idade = inicio_vigencia->tm_year - data_nascimento.tm_year;
	if((data_nascimento.tm_mon == inicio_vigencia->tm_mon && data_nascimento.tm_mday > inicio_vigencia->tm_mday) ||
		data_nascimento.tm_mon > inicio_vigencia->tm_mon) {
			idade -= 1;
	}
	return idade;
}

/******************************************************************************************/
/***************************** M�todos principais VIDA*************************************/
/******************************************************************************************/
/*!
 *  Recebe string JSON com dados do perfil e devolve string JSON ordenado com os produtos recomendados.
 */
std::string listaProdutosIndicados_(const std::string input_string);
/*!
 *  Recebe string JSON com dados do produto e devolve string JSON ordenado com os c�digos de opera��o dispon�veis.
 */
std::string retornaCodigosOperacaoVida(const std::string input_string);
/*!
 *  Recebe string JSON com dados do perfil e string JSON com dados do produto e devolve string JSON com o valor do pr�mio total.
 */
std::string gravaValorPremio_(const std::string input_string_perfil,const std::string input_string_produto);

/******************************************************************************************/
//void listaProdutosIndicados(const char* input_file, const char* output_file);
//double gravaValorPremio(const char* perfil_input_file, const char* produto_input_file, const char* output_file);
/******************************************************************************************/

std::string returnStringFromJson(const Json::Value& input_val);

Json::Value returnJsonFromString(const std::string& input_val);

Json::Value retornaValueJSON(const char* arquivo);

void criaPerfil(const Json::Value& val, Perfil& perfil);

int criaProduto(const Json::Value& val, Produto& produto);

double calculaImprimePremio(const Produto &produto, const Perfil &perfil);

double calculaPremio(const Produto &produto, const Perfil &perfil);

void montaJSONPremio(double premio,const char* output_file);

#endif /* CALC_SIM_H_ */
