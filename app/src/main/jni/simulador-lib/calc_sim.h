#ifndef CALC_SIM_H_
#define CALC_SIM_H_

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <cstdlib>
#include <fstream>

#include "jsoncpp/dist/json/json.h"
#include "Tabela.h"
#include "Perfil.h"
#include "Profissao.h"
#include "Produto.h"

using namespace std;

enum { SIM = 1, NAO };
enum { VI = 31, APIP, VMM, VMS, APICP };

/******************************************************************************************/
/***************************** M�todos principais da Lib***********************************/
/******************************************************************************************/
/*!
 *  Recebe arquivo JSON com dados do perfil e devolve JSON ordenado com os produtos recomendados.
 *  \nL� o arquivo JSON, instancia o objeto Perfil e grava no JSON de sa�da.
 */
void listaProdutosIndicados(const char* input_file, const char* output_file);

/*!
 *  Recebe arquivo JSON com dados do perfil, JSON com dados do produto e devolve JSON com o valor do pr�mio.
 *  \nL� os arquivos JSON, instancia os objetos necess�rios, calcula o pr�mio e grava no JSON de sa�da.
 */
double gravaValorPremio(const char* perfil_input_file, const char* produto_input_file, const char* output_file);

/******************************************************************************************/

Perfil inicializaPerfil();

Json::Value retornaValueJSON(const char* arquivo);

void criaPerfil(const Json::Value& val, Perfil& perfil);

Produto inicializaProduto();

void criaProduto(const Json::Value& val, Produto& produto);

double calculaImprimePremio(const Produto &produto, const Perfil &perfil);

double calculaPremio(const Produto &produto, const Perfil &perfil);

void montaJSONPremio(double premio,const char* output_file);

#endif /* CALC_SIM_H_ */
