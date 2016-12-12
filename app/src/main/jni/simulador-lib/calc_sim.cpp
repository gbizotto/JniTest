#include "calc_sim.h"

const char* Investimentos[] = {
		"Renda Fixa",
		"Multimercado",
		"Renda Fixa Crédito Privado",
		"Renda Fixa Inflacao",
		"Soberano"
};

const char* Planos[] = {
		"Rubi Plus",
		"Rubi Premium",
		"Diamante",
		"Diamante Plus"
};

const char* Codigos_Operacao[] = {
	/*1*/"1A",
	/*2*/"1B",
	/*3*/"1C",
	/*4*/"5A",
	/*5*/"5B",
	/*6*/"5C",
	/*7*/"10A",
	/*8*/"10B",
	/*9*/"10C",
	/*10*/"15A",
	/*11*/"15B",
	/*12*/"15C",
	/*13*/"20A",
	/*14*/"20B",
	/*15*/"20C",
	/*16*/"25A",
	/*17*/"25B",
	/*18*/"25C",
	/*19*/"30A",
	/*20*/"30B",
	/*21*/"35A",
	/*22*/"35B",
	/*23*/"40A",
	/*24*/"45A",
	/*25*/"50A",
	/*26*/"30C",
	/*27*/"35A",
	/*28*/"35C",
	/*29*/"615A",
	/*30*/"620A",
	/*31*/"625A",
	/*32*/"630A",
	/*33*/"635A",
	/*34*/"640A",
};

const char* Erros[] = {
/*00*/		"Sucesso",
/*01*/		"Vigência menor que a mínima estabelecida",
/*02*/		"Vigência maior que a máxima estabelecida",
/*03*/		"Idade menor que a mínima estabelecida",
/*04*/		"Idade maior que a máxima estabelecida",
/*05*/		"Profissão inválida",
/*06*/		"Valor de cobertura morte natural inválido",
/*07*/		"Valor de cobertura morte acidental inválido",
/*08*/		"Valor de cobertura indenização por acidente inválido",
/*09*/		"Valor de cobertura câncer inválido",
/*10*/		"Valor de cobertura antecipação especial por doença inválido",
/*11*/		"Valor de cobertura despesas médico-hospitalares inválido",
/*12*/		"Valor de cobertura doenças graves inválido",
/*13*/		"Valor de cobertura diária por internação inválido",
/*14*/		"Majoração não pode ser contratada (profissão ou valor em dobro)",
/*15*/		"DIT não pode ser contratada para o perfil",
/*16*/		"Franquia DIT inválida para o perfil",
/*17*/		"Contribuição mensal menor que o valor mínimo aceito",
/*18*/		"Aporte ou portabilidade menores que o valor mínimo aceito",
/*19*/		"Prazo máximo para renda mensal temporária excedido",
/*20*/		"Idade do filho menor excede a idade máxima permitida",
/*21*/		"Idade do cônjuge é inferior à idade mínima permitida",
/*22*/		"Idade do cônjuge é superior à idade máxima permitida",
/*23*/		"Prazo estabelecido para renda do cônjuge não permitido",
/*24*/		"Somatória de pensões e rendas maior que a permitida",
/*25*/		"Idade do participante menor que a mínima permitida",
/*26*/		"Idade do participante maior que a máxima permitida",
};


/*!
 *  Recebe um arquivo JSON e retorna um Value JSONCPP
 */
Json::Value retornaValueJSON(const char* arquivo) {
    Json::Value valor;
    ifstream ifs_perfil(arquivo);
    ifs_perfil >> valor;
    return valor;
}

std::string retornaCodigosOperacaoPrevidencia() {

	Json::Value output_val;
	Json::Value aux_val;

	for(int i = 6; i < 23; i++) {
			aux_val["valor"] = i+1;
			aux_val["codigo"] = Codigos_Operacao[i];
			output_val["operacoes"].append(aux_val);
	}
	return returnStringFromJson(output_val);
}

/*!
 *  Lê um Value JSON e atribui os valores lidos a um objeto Investidor
 */
int criaInvestidor(const Json::Value& val, Investidor& investidor) {

    if(val.type() == Json::objectValue) {
		if (val.empty())
			return 0;
		else {
			vector<string> keys = val.getMemberNames();
			for (size_t i = 0; i < keys.size(); i++) {
				const string& key = keys[i];
				criaInvestidor(val[key], investidor);
				if(key == "dataNascimento") {
					investidor.setData_nascimento(val[key].asString());
					investidor.setIdade(calculaIdade(investidor));
				}
				else if(key == "sexo") {
					val[key] == 1 ? investidor.setSexo(investidor.MASCULINO) : investidor.setSexo(investidor.FEMININO);
				}
				else if(key == "fumante")
				{
					val[key] == 1 ? investidor.setFumante(investidor.SIM) : investidor.setFumante(investidor.NAO);
				}
				else if(key == "codigoProfissao") {
					Profissao profissao;
					profissao.setId(val[key].asLargestInt());
					investidor.setProfissao(val[key].asLargestInt());
					investidor.setGrupo(profissao.verificaGrupoDIT(profissao.getId()));
				}
				else if(key == "iddAposentadoria") {
					investidor.setIdade_aposentadoria(val[key].asLargestInt());
				}
				else if(key == "declaracaoINSS")
				{
					val[key] == "S" ? investidor.setDeclaracao_INSS(investidor.SIM ): investidor.setDeclaracao_INSS(investidor.NAO);
				}
				else if(key == "tempoInvestimento")
				{
					if(val[key] == 1)
						investidor.setTempo_investimento(investidor.CURTO);
					else if(val[key] == 2)
						investidor.setTempo_investimento(investidor.MEDIO);
					else
						investidor.setTempo_investimento(investidor.LONGO);
				}
				else if(key == "seguroInvestimentos")
				{
					val[key] == "S" ? investidor.setSeguro_investimentos(investidor.SIM ): investidor.setSeguro_investimentos(investidor.NAO);
				}
				else if(key == "objetivo")
				{
					switch(val[key].asLargestInt()) {
						case 2:
							investidor.setObjetivo(investidor.RENDA_VITALICIA);
							break;
						case 3:
							investidor.setObjetivo(investidor.RENDA_TEMPORARIA_CURTA);
							break;
						case 4:
							investidor.setObjetivo(investidor.RENDA_TEMPORARIA_LONGA);
							break;
						default:
							investidor.setObjetivo(investidor.RESGATE);
							break;
					}
				}
				else if(key == "classeRisco")
				{
					if(val[key] == 1)
						investidor.setClasse_risco(investidor.EVITAR_PERDAS_CURTO);
					else if(val[key] == 2)
						investidor.setClasse_risco(investidor.EVITAR_PERDAS_LONGO);
					else
						investidor.setClasse_risco(investidor.ACEITAR_PERDAS);
				}
				else if(key == "reacaoMercado")
				{
					switch(val[key].asLargestInt()) {
						case 2:
							investidor.setReacao_mercado(investidor.ESPERAR);
							break;
						case 3:
							investidor.setReacao_mercado(investidor.PROCURAR);
							break;
						default:
							investidor.setReacao_mercado(investidor.RESGATAR);
							break;
					}
				}
				else if(key == "patrimonio")
				{
					switch(val[key].asLargestInt()) {
						case 2:
							investidor.setPatrimonio(investidor.CONSTRUINDO_COM);
							break;
						case 3:
							investidor.setPatrimonio(investidor.EXISTENTE);
							break;
						case 4:
							investidor.setPatrimonio(investidor.CONSOLIDADO);
							break;
						default:
							investidor.setPatrimonio(investidor.CONSTRUINDO_SEM);
							break;
					}
				}
				else if(key == "expertise")
				{
					switch(val[key].asLargestInt()) {
						case 2:
							investidor.setExpertise(investidor.RENDA_FIXA);
							break;
						case 3:
							investidor.setExpertise(investidor.ACOES);
							break;
						default:
							investidor.setExpertise(investidor.POUPANCA_CDB_FUNDOS);
							break;
					}
				}
			}
			investidor.setDesconto_segregacao(investidor.determinaFatorDeSegregacao());
		}
	}

    int cod_err = 0;
	if(investidor.getIdade() < investidor.IDADE_MINIMA || investidor.getIdade() > investidor.IDADE_MAXIMA) {
		if(investidor.getIdade() < investidor.IDADE_MINIMA) {
			cod_err = IDADE_MENOR;
		}
		else {
			cod_err = IDADE_MAIOR;
		}
	}
	else if(investidor.getIdade_aposentadoria() > investidor.IDADE_MAXIMA) {
		cod_err = IDADE_MAIOR;
	}
    return cod_err;
}

/*!
 *  Lê um Value JSON e atribui os valores lidos a um objeto Previdencia
 */
int criaPrevidencia(const Json::Value& val, Previdencia& previdencia) {

	if(val.type() == Json::objectValue) {
		if (val.empty())
			return 0;
		else {
			vector<string> keys = val.getMemberNames();
			for (size_t i = 0; i < keys.size(); i++) {
				const string& key = keys[i];
				criaPrevidencia(val[key], previdencia);
				if(key == "aporte") {
					previdencia.setAporte(val[key].asDouble());
				}
				else if(key == "investimentoMensal") {
					previdencia.setContribuicao(val[key].asDouble());
				}
				else if(key == "portabilidade") {
					previdencia.setPortabilidade(val[key].asDouble());
				}
				else if(key == "duracaoRMT") {
					previdencia.setDuracao_RMT(val[key].asLargestInt());
				}
				else if(key == "taxaJuros") {
					previdencia.setTaxa_juros(val[key].asDouble());
				}
				else if(key == "cobMorte") {
					previdencia.setCobertura_morte(val[key].asDouble());
				}
				else if(key == "cobMorteAcidental") {
					previdencia.setCobertura_map(val[key].asDouble());
				}
				else if(key == "cobInvalidezAcidente") {
					previdencia.setCobertura_ipa(val[key].asDouble());
				}
				else if(key == "antecipacaoDoenca") {
						previdencia.setCobertura_aed(val[key].asDouble());
				}
				else if(key == "assistFuneralFamiliar") {
					val[key] == "S" ? previdencia.setFuneral_familia(SIM): previdencia.setFuneral_familia(NAO);
				}
				else if(key == "cobAssistenciaFuneral") {
					previdencia.setCobertura_aff(val[key].asDouble());
				}
				else if(key == "majoracao") {
					val[key] == "S" ? previdencia.setMajoracao(SIM): previdencia.setMajoracao(NAO);
				}
				else if(key == "codOperacao") {
					previdencia.setOperacao(val[key].asLargestInt());
				}
				else if(key == "pensaoMenores") {
						previdencia.setPensao_Menores(val[key].asDouble());
				}
				else if(key == "idadeMenores") {
					previdencia.setIdade_Menores(val[key].asLargestInt());
				}
				else if(key == "pensaoConjuge") {
						previdencia.setPensao_Conjuge(val[key].asDouble());
				}
				else if(key == "idadeConjuge") {
					previdencia.setIdade_Conjuge(val[key].asLargestInt());
				}
				else if(key == "duracaoPensaoConjuge") {
					previdencia.setDuracao_Pensao_Conjuge(val[key].asLargestInt());
				}
				else if(key == "pensaoPrazoCerto") {
						previdencia.setPensao_Prazo_Certo(val[key].asDouble());
				}
				else if(key == "duracaoPrazoCerto") {
					previdencia.setDuracao_Prazo_Certo(val[key].asLargestInt());
				}
				else if(key == "rendaIPA") {
						previdencia.setRenda_IPA(val[key].asDouble());
				}
				else if(key == "rendaIPD") {
						previdencia.setRenda_IPD(val[key].asDouble());
				}
			}
			previdencia.setIof(VALOR_TAXA_IOF);
		}
	}

	int cod_err = 0;

	if(previdencia.getCobertura_morte() != 0.0 && (!(previdencia.getCobertura_map() == previdencia.getCobertura_morte() || previdencia.getCobertura_map() == previdencia.getCobertura_morte() * 2))) {
			cod_err = COB_MAP;
	}
	else if(previdencia.getCobertura_ipa() != 0.0 && (!(previdencia.getCobertura_ipa() == previdencia.getCobertura_morte() || previdencia.getCobertura_ipa() == previdencia.getCobertura_map()))) {
			cod_err = COB_IPA;
	}
	else if(previdencia.getContribuicao() < CONTRIBUICAO_MINIMA_PREVIDENCIA) {
		cod_err = CONTRIBUICAO_MINIMA;
	}
	else if((previdencia.getAporte() != 0.0 && previdencia.getAporte() < APORTE_MINIMO_PREVIDENCIA)
			|| (previdencia.getPortabilidade() != 0.0 && previdencia.getPortabilidade() < APORTE_MINIMO_PREVIDENCIA)) {
		cod_err = APORTE_MINIMO;
	}
	else if(previdencia.getDuracao_RMT() > PRAZO_MAXIMO_RMT) {
		cod_err = PRAZO_RMT;
	}
	else if(previdencia.getIdade_Menores() > IDADE_LIMITE_MENORES) {
		cod_err = IDADE_MENORES;
	}
	else if(previdencia.getIdade_Conjuge() != 0 && previdencia.getIdade_Conjuge() < IDADE_MINIMA_PREVIDENCIA) {
		cod_err = IDADE_CONJUGE_MIN;
	}
	else if(previdencia.getIdade_Conjuge() > IDADE_LIMITE_CONJUGE) {
		cod_err = IDADE_CONJUGE_MAX;
	}
	else if(previdencia.getDuracao_Pensao_Conjuge() != 0
			&& previdencia.getDuracao_Pensao_Conjuge() != 5
			&& previdencia.getDuracao_Pensao_Conjuge() != 10
			&& previdencia.getDuracao_Pensao_Conjuge() != 15
			&& previdencia.getDuracao_Pensao_Conjuge() != 20) {
		cod_err = PRAZO_RENDA_CONJUGE;
	}
	return cod_err;
}

/*!
 *  Calcula o valor futuro com ou sem aporte
 */
double calculaValorFuturo(const Investidor &investidor, const Previdencia &previdencia) {
	/*! \n
	\f{eqnarray*}{
		VF&=&PMT*[\frac{(1+i)^n-1}{i}] \\
	\f}
	*/

	/*! \n
	\f{eqnarray*}{
		VFAporte&=&PMT*[\frac{(1+i)^n-1}{i}] +\\
		VF&=&VP*(1+i)^n
	\f}
	*/
	time_t data_atual;
	std::string data_str;
	struct tm* inicio_vigencia;
	struct tm data_nascimento;
	struct tm data_saida;
	double VP = previdencia.getAporte() + previdencia.getPortabilidade();
	double PMT = VP;
	double VF = 0;
	double juro_anual = previdencia.getTaxa_juros() / 100.0;
	int idade_saida = investidor.getIdade_aposentadoria();
	int prazo_meses = 0;

	time(&data_atual);
	inicio_vigencia = localtime (&data_atual);

	data_str = investidor.getData_nascimento();
	istringstream buffer(data_str.substr(0,2));
	buffer >> data_nascimento.tm_mday;
	istringstream buffer1(data_str.substr(3,2));
	buffer1 >> data_nascimento.tm_mon; data_nascimento.tm_mon -= 1;
	istringstream buffer2(data_str.substr(6,4));
	buffer2 >> data_nascimento.tm_year; data_nascimento.tm_year -= 1900;

	data_saida.tm_mday = data_nascimento.tm_mday;
	data_saida.tm_mon = data_nascimento.tm_mon;

	data_saida.tm_year = inicio_vigencia->tm_year + (idade_saida - (inicio_vigencia->tm_year - data_nascimento.tm_year));
	prazo_meses = ( ( data_saida.tm_year - inicio_vigencia->tm_year ) * 12 ) + ( ( data_saida.tm_mon - inicio_vigencia->tm_mon ) + 1 );

	double juro_mensal = (pow(juro_anual + 1.0, 1.0 / 12.0)) - 1.0;

	//ITERAÇÃO PARA CONFERÊNCIA
	/*for (int i = 0; i < prazo_meses; ++i) {
		std::cout << std::fixed << std::setprecision(2) << 	PMT << "\n";
		VF = ( PMT * ( 1.0 + juro_mensal )) + previdencia.getContribuicao();
		PMT = VF;
	}*/
	if(idade_saida && prazo_meses && prazo_meses <= (IDADE_MAXIMA_APOSENTADORIA - IDADE_MINIMA_PREVIDENCIA) * 12)
		VF = PMT * ( pow( ( 1.0 + juro_mensal ), prazo_meses) ) + previdencia.getContribuicao() * ( ( pow( ( 1.0 + juro_mensal ), prazo_meses) ) - 1) / ( ( 1.0 + juro_mensal ) - 1 );
	else
		VF = 0.0;
	return VF;
}

double calculaRMT(double &valor_futuro, const Investidor &investidor, const Previdencia &previdencia) {

	double fator_RMT = 0.0;
	try {
		if((previdencia.getDuracao_RMT() >= PRAZO_MINIMO_RMT && previdencia.getDuracao_RMT() <= PRAZO_MAXIMO_RMT) &&
					(investidor.getIdade_aposentadoria() && investidor.getIdade_aposentadoria() <= IDADE_MAXIMA_APOSENTADORIA)) {
			if(investidor.getSexo() == investidor.FEMININO) {
				fator_RMT = tabela_Fator_Renda_Temporaria_Fem[investidor.getIdade_aposentadoria()][previdencia.getDuracao_RMT() - 1];
			}
			else {
				fator_RMT = tabela_Fator_Renda_Temporaria_Masc[investidor.getIdade_aposentadoria()][previdencia.getDuracao_RMT() - 1];
			}
		}
	}
	catch (const std::out_of_range& oor) {
		std::cerr << "Out of Range error: " << oor.what() << '\n';
	}
	return valor_futuro * fator_RMT;
}

double calculaRMV(double &valor_futuro, const Investidor &investidor) {

	double fator_RMV = 0.0;
	try {
		if(investidor.getIdade_aposentadoria() && investidor.getIdade_aposentadoria() <= IDADE_MAXIMA_APOSENTADORIA) {

			if(investidor.getSexo() == investidor.FEMININO) {
				fator_RMV = tabela_Fator_Renda_Vitalicia[investidor.getIdade_aposentadoria()][1];
			}
			else {
				fator_RMV = tabela_Fator_Renda_Vitalicia[investidor.getIdade_aposentadoria()][0];
			}
		}
	}
	catch (const std::out_of_range& oor) {
		std::cerr << "Out of Range error: " << oor.what() << '\n';
	}
	return valor_futuro * fator_RMV;
}

std::string calculaPremiosRiscoVida(Investidor& investidor,Previdencia& previdencia) {

	Json::Value output_val;

	double taxa_codigo_operacao = 0.0;
	double taxa_morte = 0.0;
	double taxa_map = 0.0;
	double taxa_ipa = 0.0;
	double taxa_aed = 0.0;
	double desconto_perfil = investidor.getDesconto_segregacao();
	double taxa_majoracao = 0.0;
	double taxa_af = 0.0;
	double premio = 0.0;
	double cobertura_majoracao = 0.0;

	for (int i = 0; i < LINHAS_TABELA_OPERACAO_DESC; ++i) {
		if(previdencia.getOperacao() == (int)(tabela_Descontos[i][0]))
		{
			taxa_codigo_operacao = tabela_Descontos[i][1];
			break;
		}
	}
	taxa_morte = tabela_morte_Previdencia[investidor.getIdade() - IDADE_MINIMA_PREVIDENCIA];

	//Taxas iguais, mas condicional mantida para futuras alterações
	if(previdencia.getCobertura_map() == previdencia.getCobertura_morte() * 2.0) {
		taxa_map = taxa_MAP_Dobro_Previdencia;
	}
	else {
		taxa_map = taxa_MAP_Previdencia;
	}
	if(previdencia.getCobertura_ipa() == previdencia.getCobertura_morte() * 2.0) {
		taxa_ipa = taxa_IPA_Previdencia;
	}
	else {
		taxa_ipa = taxa_IPA_Previdencia;
		taxa_majoracao = taxa_Majoracao_Membros;
	}

	taxa_aed = tabela_AED_Previdencia[investidor.getIdade() - IDADE_MINIMA_PREVIDENCIA];

	if(previdencia.getFuneral_familia() == SIM) {
		taxa_af = taxa_AFF_Previdencia;
	}
	else {
		taxa_af = taxa_AFT_Previdencia;
	}

	if(previdencia.getMajoracao() == SIM)
	{
		cobertura_majoracao = previdencia.getCobertura_ipa();
	}
	else {
		cobertura_majoracao = 0.0;
	}
	premio = round	( previdencia.getCobertura_morte() * taxa_morte * taxa_codigo_operacao * desconto_perfil * 100.0 * ( 1.0 + previdencia.getIof() ) ) / 100.0;
	output_val["Premio Risco Vida"]["morteNatural"].append(Json::Value(round(premio * 1000) / 1000));

	premio = round	( previdencia.getCobertura_map() * taxa_map * taxa_codigo_operacao * desconto_perfil * 100.0 * ( 1.0 + previdencia.getIof() ) ) / 100.0;
	output_val["Premio Risco Vida"]["morteAcidental"].append(Json::Value(round(premio * 1000) / 1000));

	premio = round	( previdencia.getCobertura_ipa() * taxa_ipa * taxa_codigo_operacao * desconto_perfil * 100.0 * ( 1.0 + previdencia.getIof() ) ) / 100.0;
	output_val["Premio Risco Vida"]["invalidezAcidente"].append(Json::Value(round(premio * 1000) / 1000));

	premio = round	( previdencia.getCobertura_aed() * taxa_aed * taxa_codigo_operacao * desconto_perfil * 100.0 * ( 1.0 + previdencia.getIof() ) ) / 100.0;
	output_val["Premio Risco Vida"]["antecipacaoDoenca"].append(Json::Value(round(premio * 1000) / 1000));

	premio = round	( cobertura_majoracao * taxa_majoracao * taxa_codigo_operacao * 100.0 * ( 1.0 + previdencia.getIof() ) ) / 100.0;
	output_val["Premio Risco Vida"]["majoracao"].append(Json::Value(round(premio * 1000) / 1000));

	premio = round	( previdencia.getCobertura_aff() * taxa_af * taxa_codigo_operacao * 100.0 * ( 1.0 + previdencia.getIof() ) ) / 100.0;
	output_val["Premio Risco Vida"]["assistenciaFuneral"].append(Json::Value(round(premio * 1000) / 1000));

	return returnStringFromJson(output_val);
}

std::string calculaPremiosRiscoPrevidencia(Investidor& investidor,Previdencia& previdencia) {

	Json::Value output_val;

	double premio = 0.0;
	double taxa_codigo_operacao = 1.0;
	double desconto_perfil = investidor.getDesconto_segregacao();
	double taxa_pensao_menores = 0.0;
	double taxa_pensao_conjuge = 0.0;
	double taxa_pensao_prazo_certo = 0.0;
	double taxa_renda_invalidez_acidente = 0.0;
	double taxa_renda_invalidez_doenca = 0.0;

	for (int i = 0; i < LINHAS_TABELA_OPERACAO_DESC; ++i) {
		if(previdencia.getOperacao() == (int)(tabela_Descontos[i][0]))
		{
			taxa_codigo_operacao = tabela_Descontos[i][1];
			break;
		}
	}

	taxa_pensao_menores = tabela_Pensao_Menores[investidor.getIdade()-5][previdencia.getIdade_Menores()];

	switch(previdencia.getDuracao_Pensao_Conjuge())
	{
		case 10:
			taxa_pensao_conjuge = tabela_Pensao_Conjuge_10_anos[investidor.getIdade()-16][previdencia.getIdade_Conjuge()-16];
			break;
		case 15:
			taxa_pensao_conjuge = tabela_Pensao_Conjuge_15_anos[investidor.getIdade()-16][previdencia.getIdade_Conjuge()-16];
			break;
		case 20:
			taxa_pensao_conjuge = tabela_Pensao_Conjuge_20_anos[investidor.getIdade()-16][previdencia.getIdade_Conjuge()-16];
			break;
		default:
			taxa_pensao_conjuge = tabela_Pensao_Conjuge_5_anos[investidor.getIdade()-16][previdencia.getIdade_Conjuge()-16];
			break;
	}

	taxa_pensao_prazo_certo = tabela_Pensao_Prazo_Certo[investidor.getIdade()-16][previdencia.getDuracao_Prazo_Certo()-5];

	taxa_renda_invalidez_acidente = tabela_Renda_Invalidez_Acidente[investidor.getIdade()-16];
	taxa_renda_invalidez_doenca = tabela_Renda_Invalidez_Doenca[investidor.getIdade()-16];

	premio = round ( previdencia.getPensao_Menores() * taxa_pensao_menores * taxa_codigo_operacao * desconto_perfil * 100.0 * ( 1.0 + previdencia.getIof() ) ) / 100.0;
	output_val["Premio Risco Previdencia"]["pensaoMenores"].append(Json::Value(round(premio * 1000) / 1000));

	premio = round	( previdencia.getPensao_Conjuge() * taxa_pensao_conjuge * taxa_codigo_operacao * desconto_perfil * 100.0 * ( 1.0 + previdencia.getIof() ) ) / 100.0;
	output_val["Premio Risco Previdencia"]["pensaoConjuge"].append(Json::Value(round(premio * 1000) / 1000));

	premio = round	( previdencia.getPensao_Prazo_Certo() * taxa_pensao_prazo_certo * taxa_codigo_operacao * desconto_perfil * 100.0 * ( 1.0 + previdencia.getIof() ) ) / 100.0;
	output_val["Premio Risco Previdencia"]["pensaoPrazoCerto"].append(Json::Value(round(premio * 1000) / 1000));

	premio = round	( previdencia.getRenda_IPA() * taxa_renda_invalidez_acidente * taxa_codigo_operacao * desconto_perfil * 100.0 * ( 1.0 + previdencia.getIof() ) ) / 100.0;
	output_val["Premio Risco Previdencia"]["rendaIPA"].append(Json::Value(round(premio * 1000) / 1000));

	premio = round	( previdencia.getRenda_IPD() * taxa_renda_invalidez_doenca * taxa_codigo_operacao * desconto_perfil * 100.0 * ( 1.0 + previdencia.getIof() ) ) / 100.0;
	output_val["Premio Risco Previdencia"]["rendaIPD"].append(Json::Value(round(premio * 1000) / 1000));

	return returnStringFromJson(output_val);
}

std::string calculaPrevidencia(const std::string input_string_investidor,const std::string input_string_previdencia) {
//Capital da cobertura x Taxa por cobertura x  Taxa Código de Operação x (1 - desc. Perfil) x (1 + IOF) = Prêmio Comercial

	/*! \n
	\f{eqnarray*}{
		PremioComercial&=&CapitalCobertura*TaxaCobertura*TaxaOperacao*(1.0-DescontoPerfil)*(1+IOF) \\
	\f}
	*/
	Json::Value output_val;
	int cod_err = 0;

	Investidor investidor;
	cod_err = criaInvestidor(returnJsonFromString(input_string_investidor),investidor);

	Previdencia previdencia;
	if(!cod_err) {
		cod_err = criaPrevidencia(returnJsonFromString(input_string_previdencia),previdencia);
	}

	if(cod_err) {
		output_val["resultado"]["codigo"] = cod_err;
		output_val["resultado"]["descricao"] = Erros[cod_err-100];
	}
	else {
		if(previdencia.getMajoracao() == SIM && ((investidor.getProfissao() != MEDICO && investidor.getProfissao() != DENTISTA)
				|| previdencia.getCobertura_ipa() == previdencia.getCobertura_morte() * 2
				|| previdencia.getCobertura_ipa() == previdencia.getCobertura_map() * 2)) {
			cod_err = MAJORACAO_PROFISSAO_DOBRO;
		}
		else if(previdencia.getPensao_Menores() + previdencia.getPensao_Conjuge() + previdencia.getPensao_Prazo_Certo()
				+ previdencia.getRenda_IPA() + previdencia.getRenda_IPD() > VALOR_MAXIMO_PENSOES) {
			cod_err = VALOR_MAX_PENSOES;
		}
		else if(investidor.getIdade() < IDADE_MINIMA_PARTICIPANTE
				&& (previdencia.getPensao_Conjuge() || previdencia.getPensao_Prazo_Certo() || previdencia.getRenda_IPA() || previdencia.getRenda_IPD())) {
			cod_err = IDADE_PARTICIPANTE_MENOR;
		}
		else if(investidor.getIdade() < IDADE_MINIMA_MENORES && previdencia.getPensao_Menores()) {
			cod_err = IDADE_PARTICIPANTE_MENOR;
		}
		else if(investidor.getIdade() > IDADE_MAXIMA_PARTICIPANTE
				&&  (previdencia.getPensao_Menores() || previdencia.getPensao_Conjuge() || previdencia.getPensao_Prazo_Certo() || previdencia.getRenda_IPA() || previdencia.getRenda_IPD())) {
			cod_err = IDADE_PARTICIPANTE_MAIOR;
		}

		if(cod_err) {
			output_val["resultado"]["codigo"] = cod_err;
			output_val["resultado"]["descricao"] = Erros[cod_err-100];
		}
		else {
			double VF = calculaValorFuturo(investidor,previdencia);
			output_val["calculosPrevidencia"]["valorFuturo"] = Json::Value(static_cast<std::ostringstream*>( &(std::ostringstream() << std::fixed << std::setprecision(2) << round(VF * 1000) / 1000))->str());
			output_val["calculosPrevidencia"]["rendaMensalTemporaria"] = Json::Value(static_cast<std::ostringstream*>( &(std::ostringstream() << std::fixed << std::setprecision(2) << round(calculaRMT(VF,investidor,previdencia) * 1000) / 1000))->str());
			output_val["calculosPrevidencia"]["rendaMensalVitalicia"] = Json::Value(static_cast<std::ostringstream*>( &(std::ostringstream() << std::fixed << std::setprecision(2) << round(calculaRMV(VF,investidor) * 1000) / 1000))->str());

			double taxa_codigo_operacao = 0.0;
			double taxa_morte = 0.0;
			double taxa_map = 0.0;
			double taxa_ipa = 0.0;
			double taxa_aed = 0.0;
			double desconto_perfil = investidor.getDesconto_segregacao();
			double taxa_majoracao = 0.0;
			double taxa_af = 0.0;
			double premio = 0.0;
			double premio_total = previdencia.getContribuicao();
			double cobertura_majoracao = 0.0;

			for (int i = 0; i < LINHAS_TABELA_OPERACAO_DESC; ++i) {
				if(previdencia.getOperacao() == (int)(tabela_Descontos[i][0]))
				{
					taxa_codigo_operacao = tabela_Descontos[i][1];
					break;
				}
			}
			taxa_morte = tabela_morte_Previdencia[investidor.getIdade() - IDADE_MINIMA_PREVIDENCIA];

			//Taxas iguais, mas condicional mantida para futuras alterações
			if(previdencia.getCobertura_morte() != 0.0) {
				if(previdencia.getCobertura_map() == previdencia.getCobertura_morte() * 2.0) {
					taxa_map = taxa_MAP_Dobro_Previdencia;
				}
				else {
					taxa_map = 0.0;
				}
			}
			else if(previdencia.getCobertura_map() != 0.0) {
				taxa_map = taxa_MAP_Previdencia;
			}

			if(previdencia.getCobertura_ipa() == previdencia.getCobertura_morte() * 2.0) {
				taxa_ipa = taxa_IPA_Previdencia;
			}
			else {
				taxa_ipa = taxa_IPA_Previdencia;
				taxa_majoracao = taxa_Majoracao_Membros;
			}

			taxa_aed = tabela_AED_Previdencia[investidor.getIdade() - IDADE_MINIMA_PREVIDENCIA];

			if(previdencia.getFuneral_familia() == SIM) {
				taxa_af = taxa_AFF_Previdencia;
			}
			else {
				taxa_af = taxa_AFT_Previdencia;
			}

			if(previdencia.getMajoracao() == SIM)
			{
				cobertura_majoracao = previdencia.getCobertura_ipa();
			}
			else {
				cobertura_majoracao = 0.0;
			}

			premio = round	( previdencia.getCobertura_morte() * taxa_morte * taxa_codigo_operacao * desconto_perfil * 100.0 * ( 1.0 + previdencia.getIof() ) ) / 100.0;
			output_val["calculosPrevidencia"]["morteNatural"] = Json::Value(static_cast<std::ostringstream*>( &(std::ostringstream() << std::fixed << std::setprecision(2) << round(premio * 1000) / 1000))->str());
			premio_total += premio;

			premio = round	( previdencia.getCobertura_map() * taxa_map * taxa_codigo_operacao * desconto_perfil * 100.0 * ( 1.0 + previdencia.getIof() ) ) / 100.0;
			output_val["calculosPrevidencia"]["morteAcidental"] = Json::Value(static_cast<std::ostringstream*>( &(std::ostringstream() << std::fixed << std::setprecision(2) << round(premio * 1000) / 1000))->str());
			premio_total += premio;

			premio = round	( previdencia.getCobertura_ipa() * taxa_ipa * taxa_codigo_operacao * desconto_perfil * 100.0 * ( 1.0 + previdencia.getIof() ) ) / 100.0;
			output_val["calculosPrevidencia"]["invalidezAcidente"] = Json::Value(static_cast<std::ostringstream*>( &(std::ostringstream() << std::fixed << std::setprecision(2) << round(premio * 1000) / 1000))->str());
			premio_total += premio;

			premio = round	( previdencia.getCobertura_aed() * taxa_aed * taxa_codigo_operacao * desconto_perfil * 100.0 * ( 1.0 + previdencia.getIof() ) ) / 100.0;
			output_val["calculosPrevidencia"]["antecipacaoDoenca"] = Json::Value(static_cast<std::ostringstream*>( &(std::ostringstream() << std::fixed << std::setprecision(2) << round(premio * 1000) / 1000))->str());
			premio_total += premio;

			premio = round	( cobertura_majoracao * taxa_majoracao * taxa_codigo_operacao * desconto_perfil * 100.0 * ( 1.0 + previdencia.getIof() ) ) / 100.0;
			output_val["calculosPrevidencia"]["majoracao"] = Json::Value(static_cast<std::ostringstream*>( &(std::ostringstream() << std::fixed << std::setprecision(2) << round(premio * 1000) / 1000))->str());
			premio_total += premio;

			premio = round	( previdencia.getCobertura_aff() * taxa_af * taxa_codigo_operacao * 100.0 * ( 1.0 + previdencia.getIof() ) ) / 100.0;
			output_val["calculosPrevidencia"]["assistenciaFuneral"] = Json::Value(static_cast<std::ostringstream*>( &(std::ostringstream() << std::fixed << std::setprecision(2) << round(premio * 1000) / 1000))->str());
			premio_total += premio;

			double taxa_pensao_menores = 0.0;
			double taxa_pensao_conjuge = 0.0;
			double taxa_pensao_prazo_certo = 0.0;
			double taxa_renda_invalidez_acidente = 0.0;
			double taxa_renda_invalidez_doenca = 0.0;

			for (int i = 0; i < LINHAS_TABELA_OPERACAO_DESC; ++i) {
				if(previdencia.getOperacao() == (int)(tabela_Descontos[i][0]))
				{
					taxa_codigo_operacao = tabela_Descontos[i][1];
					break;
				}
			}

			taxa_pensao_menores = tabela_Pensao_Menores[investidor.getIdade()-5][previdencia.getIdade_Menores()];

			switch(previdencia.getDuracao_Pensao_Conjuge())
			{
				case 10:
					taxa_pensao_conjuge = tabela_Pensao_Conjuge_10_anos[investidor.getIdade()-16][previdencia.getIdade_Conjuge()-16];
					break;
				case 15:
					taxa_pensao_conjuge = tabela_Pensao_Conjuge_15_anos[investidor.getIdade()-16][previdencia.getIdade_Conjuge()-16];
					break;
				case 20:
					taxa_pensao_conjuge = tabela_Pensao_Conjuge_20_anos[investidor.getIdade()-16][previdencia.getIdade_Conjuge()-16];
					break;
				default:
					taxa_pensao_conjuge = tabela_Pensao_Conjuge_5_anos[investidor.getIdade()-16][previdencia.getIdade_Conjuge()-16];
					break;
			}

			taxa_pensao_prazo_certo = tabela_Pensao_Prazo_Certo[investidor.getIdade()-16][(previdencia.getDuracao_Prazo_Certo()-5)/5];

			taxa_renda_invalidez_acidente = tabela_Renda_Invalidez_Acidente[investidor.getIdade()-16];
			taxa_renda_invalidez_doenca = tabela_Renda_Invalidez_Doenca[investidor.getIdade()-16];

			premio = round ( previdencia.getPensao_Menores() * taxa_pensao_menores * taxa_codigo_operacao * desconto_perfil * 100.0 * ( 1.0 + previdencia.getIof() ) ) / 100.0;
			output_val["calculosPrevidencia"]["pensaoMenores"] = Json::Value(static_cast<std::ostringstream*>( &(std::ostringstream() << std::fixed << std::setprecision(2) << round(premio * 1000) / 1000))->str());
			premio_total += premio;

			premio = round	( previdencia.getPensao_Conjuge() * taxa_pensao_conjuge * taxa_codigo_operacao * desconto_perfil * 100.0 * ( 1.0 + previdencia.getIof() ) ) / 100.0;
			output_val["calculosPrevidencia"]["pensaoConjuge"] = Json::Value(static_cast<std::ostringstream*>( &(std::ostringstream() << std::fixed << std::setprecision(2) << round(premio * 1000) / 1000))->str());
			premio_total += premio;

			premio = round	( previdencia.getPensao_Prazo_Certo() * taxa_pensao_prazo_certo * taxa_codigo_operacao * desconto_perfil * 100.0 * ( 1.0 + previdencia.getIof() ) ) / 100.0;
			output_val["calculosPrevidencia"]["pensaoPrazoCerto"] = Json::Value(static_cast<std::ostringstream*>( &(std::ostringstream() << std::fixed << std::setprecision(2) << round(premio * 1000) / 1000))->str());
			premio_total += premio;

			premio = round	( previdencia.getRenda_IPA() * taxa_renda_invalidez_acidente * taxa_codigo_operacao * desconto_perfil * 100.0 * ( 1.0 + previdencia.getIof() ) ) / 100.0;
			output_val["calculosPrevidencia"]["rendaIPA"] = Json::Value(static_cast<std::ostringstream*>( &(std::ostringstream() << std::fixed << std::setprecision(2) << round(premio * 1000) / 1000))->str());
			premio_total += premio;

			premio = round	( previdencia.getRenda_IPD() * taxa_renda_invalidez_doenca * taxa_codigo_operacao * desconto_perfil * 100.0 * ( 1.0 + previdencia.getIof() ) ) / 100.0;
			output_val["calculosPrevidencia"]["rendaIPD"] = Json::Value(static_cast<std::ostringstream*>( &(std::ostringstream() << std::fixed << std::setprecision(2) << round(premio * 1000) / 1000))->str());
			premio_total += premio;

			output_val["calculosPrevidencia"]["premioTotal"] = Json::Value(static_cast<std::ostringstream*>( &(std::ostringstream() << std::fixed << std::setprecision(2) << round(premio_total * 1000) / 1000))->str());
		}
	}
	return returnStringFromJson(output_val);
}


std::string calculaPerfilInvestidor(const std::string input_string_investidor,const std::string input_string_previdencia) {

	Json::Value output_val;
	Json::Value aux_val;

	int tipo_fundo = 0;
	int pontuacao_perfil = 0;
	int pontuacao_regime_tributario = 0;
	int cod_err = 0;
	int plano = 0;

	Investidor investidor;
	cod_err = criaInvestidor(returnJsonFromString(input_string_investidor),investidor);

	Previdencia previdencia;
	if(!cod_err) {
		cod_err = criaPrevidencia(returnJsonFromString(input_string_previdencia),previdencia);
	}

	if(cod_err) {
		output_val["resultado"]["codigo"] = cod_err;
		output_val["resultado"]["descricao"] = Erros[cod_err-100];
	}
	else {
		if(investidor.getDeclaracao_INSS() == investidor.SIM) {
			tipo_fundo = investidor.PGBL;
		}
		else {
			tipo_fundo = investidor.VGBL;
		}

		if(investidor.getTempo_investimento() == investidor.LONGO) {
			pontuacao_perfil += 70;
			pontuacao_regime_tributario += 40;
		}
		else if(investidor.getTempo_investimento() == investidor.MEDIO) {
			pontuacao_perfil += 30;
			pontuacao_regime_tributario += 17;
		}

		if(investidor.getSeguro_investimentos() == investidor.NAO) {
			pontuacao_regime_tributario += 10;
		}

		if(investidor.getObjetivo() == investidor.RESGATE) {
			pontuacao_regime_tributario += 24;
		}
		else if (investidor.getObjetivo() == investidor.RENDA_VITALICIA) {
			pontuacao_regime_tributario += 25;
		}
		else if (investidor.getObjetivo() == investidor.RENDA_TEMPORARIA_LONGA) {
			pontuacao_regime_tributario += 20;
		}

		if(investidor.getClasse_risco() == investidor.EVITAR_PERDAS_CURTO) {
			pontuacao_perfil += 0;
		}
		else if(investidor.getClasse_risco() == investidor.EVITAR_PERDAS_LONGO) {
			pontuacao_perfil += 20;
		}
		else if(investidor.getClasse_risco() == investidor.ACEITAR_PERDAS) {
			pontuacao_perfil += 50;
		}

		if(investidor.getReacao_mercado() == investidor.RESGATAR) {
			pontuacao_perfil += 0;
		}
		else if (investidor.getReacao_mercado() == investidor.ESPERAR) {
			pontuacao_perfil += 20;
		}
		else if (investidor.getReacao_mercado() == investidor.PROCURAR) {
			pontuacao_perfil += 40;
		}

		if(investidor.getPatrimonio() == investidor.CONSTRUINDO_SEM) {
			pontuacao_perfil += 30;
		}
		else if(investidor.getPatrimonio() == investidor.CONSTRUINDO_COM) {
			pontuacao_perfil += 20;
		}
		else if(investidor.getPatrimonio() == investidor.EXISTENTE) {
			pontuacao_perfil += 10;
		}
		else if(investidor.getPatrimonio() == investidor.CONSOLIDADO) {
			pontuacao_perfil += 0;
		}

		if(investidor.getExpertise() == investidor.POUPANCA_CDB_FUNDOS) {
			pontuacao_perfil += 0;
		}
		else if(investidor.getExpertise() == investidor.RENDA_FIXA) {
			pontuacao_perfil += 10;
		}
		else if(investidor.getExpertise() == investidor.ACOES) {
			pontuacao_perfil += 40;
		}

		if(tipo_fundo == investidor.PGBL) {
			output_val["modalidade"] = Json::Value("PGBL");
		}
		else {
			output_val["modalidade"] = Json::Value("VGBL");
		}

		if(pontuacao_regime_tributario <= investidor.PROGRESSIVO) {
			output_val["regimeTributario"] = Json::Value("Progressivo");
		}
		else {
			output_val["regimeTributario"] = Json::Value("Regressivo");
		}

		if(pontuacao_perfil <= investidor.CONSERVADOR) {
			output_val["perfilInvestidor"] = Json::Value("Conservador");
			investidor.setPerfil(investidor.CONSERVADOR);
		}
		else if(pontuacao_perfil <= investidor.MODERADO) {
			output_val["perfilInvestidor"] = Json::Value("Moderado");
			investidor.setPerfil(investidor.MODERADO);
		}
		else if(pontuacao_perfil <= investidor.ARROJADO) {
			output_val["perfilInvestidor"] = Json::Value("Arrojado");
			investidor.setPerfil(investidor.ARROJADO);
		}
		else {
			output_val["perfilInvestidor"] = Json::Value("Agressivo");
			investidor.setPerfil(investidor.AGRESSIVO);
		}

		if((previdencia.getContribuicao() >= 100.0 && previdencia.getAporte() + previdencia.getPortabilidade() >= 500000.0) ||
				(previdencia.getContribuicao() >= 3000.0 && previdencia.getAporte() + previdencia.getPortabilidade() >= 300000.0)) {
			output_val["plano"] = Json::Value("Diamante Plus");
			plano = DIAMANTEPLUS;
		}
		else if((previdencia.getContribuicao() >= 100.0 && previdencia.getAporte() + previdencia.getPortabilidade() >= 100000.0) ||
				(previdencia.getContribuicao() >= 1500.0 && previdencia.getAporte() + previdencia.getPortabilidade() >= 60000.0)) {
			output_val["plano"] = Json::Value("Diamante");
			plano = DIAMANTE;
		}
		else if((previdencia.getContribuicao() >= 100.0 && previdencia.getAporte() + previdencia.getPortabilidade() >= 20000.0) ||
				previdencia.getContribuicao() >= 500.0) {
			output_val["plano"] = Json::Value("Rubi Premium");
			plano = RUBIPREMIUM;
		}
		else if((previdencia.getContribuicao() >= 100.0 && previdencia.getAporte() + previdencia.getPortabilidade() >= 5000.0) ||
				previdencia.getContribuicao() >= 200.0) {
			output_val["plano"] = Json::Value("Rubi Plus");
			plano = RUBIPLUS;
		}

		switch(plano) {
			case DIAMANTEPLUS:
			case DIAMANTE:
				if(investidor.getPerfil() == investidor.CONSERVADOR){
					aux_val["valor"] = 70;
					aux_val["tipo"] = "Soberano";
					output_val["investimentos"].append(aux_val);
					aux_val["valor"] = 20;
					aux_val["tipo"] = "Renda Fixa";
					output_val["investimentos"].append(aux_val);
					aux_val["valor"] = 10;
					aux_val["tipo"] = "Renda Fixa Crédito Privado";
					output_val["investimentos"].append(aux_val);
				}
				else if(investidor.getPerfil() == investidor.MODERADO){
					aux_val["valor"] = 20;
					aux_val["tipo"] = "Soberano";
					output_val["investimentos"].append(aux_val);
					aux_val["valor"] = 30;
					aux_val["tipo"] = "Renda Fixa";
					output_val["investimentos"].append(aux_val);
					aux_val["valor"] = 30;
					aux_val["tipo"] = "Renda Fixa Crédito Privado";
					output_val["investimentos"].append(aux_val);
					aux_val["valor"] = 10;
					aux_val["tipo"] = "Renda Fixa Inflação";
					output_val["investimentos"].append(aux_val);
					aux_val["valor"] = 10;
					aux_val["tipo"] = "Multimercado";
					output_val["investimentos"].append(aux_val);
				}
				else if(investidor.getPerfil() == investidor.ARROJADO){
					aux_val["valor"] = 25;
					aux_val["tipo"] = "Renda Fixa";
					output_val["investimentos"].append(aux_val);
					aux_val["valor"] = 35;
					aux_val["tipo"] = "Renda Fixa Crédito Privado";
					output_val["investimentos"].append(aux_val);
					aux_val["valor"] = 15;
					aux_val["tipo"] = "Renda Fixa Inflação";
					output_val["investimentos"].append(aux_val);
					aux_val["valor"] = 25;
					aux_val["tipo"] = "Multimercado";
					output_val["investimentos"].append(aux_val);
				}
				else if(investidor.getPerfil() == investidor.AGRESSIVO){
					aux_val["valor"] = 25;
					aux_val["tipo"] = "Renda Fixa";
					output_val["investimentos"].append(aux_val);
					aux_val["valor"] = 20;
					aux_val["tipo"] = "Renda Fixa Crédito Privado";
					output_val["investimentos"].append(aux_val);
					aux_val["valor"] = 20;
					aux_val["tipo"] = "Renda Fixa Inflação";
					output_val["investimentos"].append(aux_val);
					aux_val["valor"] = 35;
					aux_val["tipo"] = "Multimercado";
					output_val["investimentos"].append(aux_val);
				}
				break;
			case RUBIPREMIUM:
				if(investidor.getPerfil() == investidor.CONSERVADOR){
					aux_val["valor"] = 90;
					aux_val["tipo"] = "Renda Fixa";
					output_val["investimentos"].append(aux_val);
					aux_val["valor"] = 10;
					aux_val["tipo"] = "Renda Fixa Crédito Privado";
					output_val["investimentos"].append(aux_val);
				}
				else if(investidor.getPerfil() == investidor.MODERADO){
					aux_val["valor"] = 60;
					aux_val["tipo"] = "Renda Fixa";
					output_val["investimentos"].append(aux_val);
					aux_val["valor"] = 25;
					aux_val["tipo"] = "Renda Fixa Crédito Privado";
					output_val["investimentos"].append(aux_val);
					aux_val["valor"] = 10;
					aux_val["tipo"] = "Renda Fixa Inflação";
					output_val["investimentos"].append(aux_val);
					aux_val["valor"] = 5;
					aux_val["tipo"] = "Multimercado";
					output_val["investimentos"].append(aux_val);
				}
				else if(investidor.getPerfil() == investidor.ARROJADO){
					aux_val["valor"] = 25;
					aux_val["tipo"] = "Renda Fixa";
					output_val["investimentos"].append(aux_val);
					aux_val["valor"] = 35;
					aux_val["tipo"] = "Renda Fixa Crédito Privado";
					output_val["investimentos"].append(aux_val);
					aux_val["valor"] = 15;
					aux_val["tipo"] = "Renda Fixa Inflação";
					output_val["investimentos"].append(aux_val);
					aux_val["valor"] = 25;
					aux_val["tipo"] = "Multimercado";
					output_val["investimentos"].append(aux_val);
				}
				else if(investidor.getPerfil() == investidor.AGRESSIVO){
					aux_val["valor"] = 25;
					aux_val["tipo"] = "Renda Fixa";
					output_val["investimentos"].append(aux_val);
					aux_val["valor"] = 20;
					aux_val["tipo"] = "Renda Fixa Crédito Privado";
					output_val["investimentos"].append(aux_val);
					aux_val["valor"] = 20;
					aux_val["tipo"] = "Renda Fixa Inflação";
					output_val["investimentos"].append(aux_val);
					aux_val["valor"] = 35;
					aux_val["tipo"] = "Multimercado";
					output_val["investimentos"].append(aux_val);
				}
				break;
			case RUBIPLUS:
				if(investidor.getPerfil() == investidor.CONSERVADOR){
					aux_val["valor"] = 100;
					aux_val["tipo"] = "Renda Fixa";
					output_val["investimentos"].append(aux_val);
				}
				else if(investidor.getPerfil() == investidor.MODERADO){
					aux_val["valor"] = 95;
					aux_val["tipo"] = "Renda Fixa";
					output_val["investimentos"].append(aux_val);
					aux_val["valor"] = 5;
					aux_val["tipo"] = "Multimercado";
					output_val["investimentos"].append(aux_val);
				}
				else if(investidor.getPerfil() == investidor.ARROJADO){
					aux_val["valor"] = 75;
					aux_val["tipo"] = "Renda Fixa";
					output_val["investimentos"].append(aux_val);
					aux_val["valor"] = 25;
					aux_val["tipo"] = "Multimercado";
					output_val["investimentos"].append(aux_val);
				}
				else if(investidor.getPerfil() == investidor.AGRESSIVO){
					aux_val["valor"] = 65;
					aux_val["tipo"] = "Renda Fixa";
					output_val["investimentos"].append(aux_val);
					aux_val["valor"] = 35;
					aux_val["tipo"] = "Multimercado";
					output_val["investimentos"].append(aux_val);
				}
				break;
		}
	}
	return returnStringFromJson(output_val);
}



/*!
 *  Lê um Value JSON e atribui os valores lidos a um objeto Perfil
 */
void criaPerfil(const Json::Value& val, Perfil& perfil) {

    if(val.type() == Json::objectValue) {
		if (val.empty())
			return;
		else {
			vector<string> keys = val.getMemberNames();
			for (size_t i = 0; i < keys.size(); i++) {
				const string& key = keys[i];
				criaPerfil(val[key], perfil);
				if(key == "dataNascimento") {
					perfil.setData_nascimento(val[key].asString());
					perfil.setIdade(calculaIdade(perfil));
				}
				else if(key == "sexo") {
					val[key] == 1 ? perfil.setSexo(perfil.MASCULINO) : perfil.setSexo(perfil.FEMININO);
				}
				else if(key == "fumante")
				{
					val[key] == 1 ? perfil.setFumante(perfil.SIM) : perfil.setFumante(perfil.NAO);
				}
				else if(key == "codigoProfissao") {
					Profissao profissao;
					profissao.setId(val[key].asLargestInt());
					perfil.setProfissao(val[key].asLargestInt());
					perfil.setGrupo(profissao.verificaGrupoDIT(profissao.getId()));
				}
				else if(key == "profCLT") {
					val[key] == "S" ? perfil.setRegime(perfil.CLT) : perfil.setRegime(perfil.PJ);
				}
				else if(key == "vigenciaInicial") {
					perfil.setVigencia(val[key].asLargestInt());
				}
			}
			perfil.determinaFatorDeSegregacao();
		}
	}
}

std::string returnStringFromJson(const Json::Value& input_val) {

	Json::FastWriter fastWriter;
	std::string output_string = fastWriter.write(input_val);
	return output_string;
}

Json::Value returnJsonFromString(const std::string& input_val) {

    Json::Value output_val;
    Json::Reader reader;

    bool parsingSuccessful = reader.parse(input_val.c_str(),output_val);
    if ( !parsingSuccessful )
    {
        std::cout  << "Failed to parse"
               << reader.getFormattedErrorMessages();
        return 0;
    }
    return output_val;
}

std::string listaProdutosIndicados_(const std::string input_string) {

	Perfil perfil;
	criaPerfil(returnJsonFromString(input_string),perfil);

	Json::Value output_val;
	Json::Value aux_val;
	int cod_err = 0;

	if(perfil.getVigencia() < perfil.VIGENCIA_MINIMA || perfil.getVigencia() > perfil.VIGENCIA_MAXIMA ) { /*!< Regra linha 2 */
		if(perfil.getVigencia() < perfil.VIGENCIA_MINIMA) {
			cod_err = VIGENCIA_MENOR;
		}
		else {
			cod_err = VIGENCIA_MAIOR;
		}
	}
	else if(perfil.getIdade() < perfil.IDADE_MINIMA || perfil.getIdade() > perfil.IDADE_MAXIMA) { /*!< Regra linha 3 */
		if(perfil.getIdade() < perfil.IDADE_MINIMA) {
			cod_err = IDADE_MENOR;
		}
		else {
			cod_err = IDADE_MAIOR;
		}
	}
	if(cod_err) {
		output_val["resultado"]["codigo"] = cod_err;
		output_val["resultado"]["descricao"] = Erros[cod_err-100];
	}
	else {
		if(perfil.getVigencia() < perfil.VIGENCIA_MAXIMA) {
			aux_val["nome"] = "Acidentes Pessoais Individual Curto Prazo";
			aux_val["codigo"] = "35";
			output_val["produtos"].append(aux_val);
		}
		else if (perfil.getIdade() >= perfil.IDADE_MAXIMA - 5) {
			aux_val["nome"] = "Acidentes Pessoais Individual Plus";
			aux_val["codigo"] = "32";
			output_val["produtos"].append(aux_val);
		}
		else {
			if(perfil.getSexo() == perfil.FEMININO) {
				aux_val["nome"] = "Vida Mais Mulher";
				aux_val["codigo"] = "33";
				output_val["produtos"].append(aux_val);
			}
			aux_val["nome"] = "Vida Individual";
			aux_val["codigo"] = "31";
			output_val["produtos"].append(aux_val);
			aux_val["nome"] = "Vida Mais Simples";
			aux_val["codigo"] = "34";
			output_val["produtos"].append(aux_val);
			aux_val["nome"] = "Acidentes Pessoais Individual Plus";
			aux_val["codigo"] = "32";
			output_val["produtos"].append(aux_val);
		}
	}
	return returnStringFromJson(output_val);
}

std::string retornaCodigosOperacaoVida(const std::string input_string) {
	Produto produto;
	criaProduto(returnJsonFromString(input_string),produto);

	Json::Value output_val;
	Json::Value aux_val;

	switch(produto.getTipo()) {
		case VI:
		case APIP:
		case VMM:
			for(int i = 6; i < 23; i++) {
				if(!(produto.getTipo() == VMM && i == 6)) {
					aux_val["valor"] = i+1;
					aux_val["codigo"] = Codigos_Operacao[i];
					output_val["operacoes"].append(aux_val);
				}
			}
			aux_val["valor"] = 26;
			aux_val["codigo"] = Codigos_Operacao[26-1];
			output_val["operacoes"].append(aux_val);
			aux_val["valor"] = 28;
			aux_val["codigo"] = Codigos_Operacao[28-1];
			output_val["operacoes"].append(aux_val);
			break;
		case VMS:
			aux_val["valor"] = 20;
			aux_val["codigo"] = Codigos_Operacao[20-1];
			output_val["operacoes"].append(aux_val);
			aux_val["valor"] = 21;
			aux_val["codigo"] = Codigos_Operacao[21-1];
			output_val["operacoes"].append(aux_val);
			break;
		case APICP:
			for(int i = 2; i < LINHAS_TABELA_OPERACAO_APICP; i++) {
				aux_val["valor"] = (int)tabela_taxas_COD_OP_APICP[i][0];
				aux_val["codigo"] = Codigos_Operacao[(int)(tabela_taxas_COD_OP_APICP[i][0])-1];
				output_val["operacoes"].append(aux_val);
			}
			break;
	}
	return returnStringFromJson(output_val);
}

/*void listaProdutosIndicados(const char* input_file, const char* output_file) {

	Perfil perfil;
	Json::Value valor = retornaValueJSON(input_file);
	criaPerfil(valor,perfil);

	std::ofstream file_id;
	file_id.open(output_file);
	Json::Value event;

	if(perfil.getVigencia() < 365) {
		event["Opcoes"]["produto"].append(Json::Value(35));
		event["Opcoes"]["dominio"].append("Acidentes Pessoais Individual");
	}
	else if (perfil.getIdade() >= 65) {
		event["Opcoes"]["produto"].append(Json::Value(32));
		event["Opcoes"]["dominio"].append("Acidentes Pessoais Individual Plus");
	}
	else {
		if(perfil.getSexo() == perfil.FEMININO) {
			event["Opcoes"]["produto"].append(Json::Value(33));
			event["Opcoes"]["dominio"].append("Vida Mais Mulher");
		}
		event["Opcoes"]["produto"].append(Json::Value(31));
		event["Opcoes"]["produto"].append(Json::Value(34));
		event["Opcoes"]["produto"].append(Json::Value(32));
		event["Opcoes"]["dominio"].append("Vida Individual");
		event["Opcoes"]["dominio"].append("Vida Mais Simples");
		event["Opcoes"]["dominio"].append("Acidentes Pessoais Individual Plus");
	}

    Json::StyledStreamWriter styledStream;
    styledStream.write(file_id,event);
    file_id.close();
}*/

inline bool verificaValorCobertura(double valor, const double tabela[], int linhas, bool dobro) {

	int mult;

	if(valor == 0.0)
		return true;

	dobro ? mult = 2 : mult = 1;
	for (int i = 0; i < linhas; ++i) {
		if(valor == tabela[i] * mult) {
			return true;
		}
	}
	return false;
}

/*!
 *  Lê um Value JSON e atribui os valores lidos a um objeto Produto
 */
int criaProduto(const Json::Value& val, Produto& produto) {

	if(val.type() == Json::objectValue) {
		if (val.empty()) {
			return 0;
		}
		else {
			vector<string> keys = val.getMemberNames();
			for (size_t i = 0; i < keys.size(); i++) {
				const string& key = keys[i];
				criaProduto(val[key], produto);
				if(key == "antecipacaoDoenca") {
						produto.setCobertura_aed(val[key].asDouble());
				}
				else if(key == "assistFuneralFamiliar") {
					val[key] == "S" ? produto.setFuneral_familia(SIM): produto.setFuneral_familia(NAO);
				}
				else if(key == "cobAssistenciaFuneral") {
					produto.setCobertura_aff(val[key].asDouble());
				}
				else if(key == "cobDespesasMedicas") {
					produto.setDespesas_medicas(val[key].asDouble());
				}
				else if(key == "cobDoencasGraves") {
					produto.setDoencas_graves(val[key].asDouble());
				}
				else if(key == "cobInvalidezAcidente") {
					produto.setCobertura_ipa(val[key].asDouble());
				}
				else if(key == "cobMorte") {
					produto.setCobertura_morte(val[key].asDouble());
				}
				else if(key == "cobMorteAcidental") {
					produto.setCobertura_map(val[key].asDouble());
				}
				else if(key == "codOperacao") {
					produto.setOperacao(val[key].asLargestInt());
				}
				else if(key == "diariaIncapacidadeTemp") {
					produto.setDiaria_it(val[key].asDouble());
				}
				else if(key == "dominio")
				{
				   produto.setTipo(val[key].asLargestInt());
				}
				else if(key == "franquiaReduzida") {
					val[key] == "S" ? produto.setFranquia_reduzida(SIM) : produto.setFranquia_reduzida(NAO);
				}
				else if(key == "majoracao") {
					val[key] == "S" ? produto.setMajoracao(SIM): produto.setMajoracao(NAO);
				}
				else if(key == "tipoDoencaGrave") {
					produto.setTipo_doenca_grave(val[key].asLargestInt());
				}
			}
			produto.setIof(VALOR_TAXA_IOF);
		}
	}

	int cod_err = 0;

    switch (produto.getTipo()) {
		case VMS:
			if(!verificaValorCobertura(produto.getCobertura_morte(), tabela_cobertura_morte_MAP_IPA_VMS, OPCOES_COBERTURA, false))
				cod_err = COB_MORTE;
			else if(!verificaValorCobertura(produto.getCobertura_map(), tabela_cobertura_morte_MAP_IPA_VMS, OPCOES_COBERTURA, true)
					|| (produto.getCobertura_map() != 0.0 && produto.getCobertura_morte() != produto.getCobertura_map() / 2))
				cod_err = COB_MAP;
			else if(!verificaValorCobertura(produto.getCobertura_ipa(), tabela_cobertura_morte_MAP_IPA_VMS, OPCOES_COBERTURA, false)
					|| (produto.getCobertura_ipa() != 0.0 && produto.getCobertura_morte() != produto.getCobertura_ipa()))
				cod_err = COB_IPA;
			break;
		case VMM:
			if(!verificaValorCobertura(produto.getCobertura_morte(), tabela_cobertura_morte_IPA_VMM, OPCOES_COBERTURA, false))
				cod_err = COB_MORTE;
			else if(!verificaValorCobertura(produto.getCobertura_map(), tabela_cobertura_CANCER_VMM, OPCOES_COBERTURA, false)
					|| (produto.getCobertura_map() != 0.0 && produto.getCobertura_morte() != produto.getCobertura_map() * 2))
				cod_err = COB_CANCER;
			else if(!verificaValorCobertura(produto.getCobertura_ipa(), tabela_cobertura_morte_IPA_VMM, OPCOES_COBERTURA, false)
				|| (produto.getCobertura_ipa() != 0.0 && produto.getCobertura_morte() != produto.getCobertura_ipa()))
				cod_err = COB_IPA;
			break;
		case VI:
			if(!verificaValorCobertura(produto.getCobertura_morte(), tabela_cobertura_morte_MAP_IPA_AED_DG_VI, OPCOES_COBERTURA_VI, false))
				cod_err = COB_MORTE;
			else if(!verificaValorCobertura(produto.getCobertura_map(), tabela_cobertura_morte_MAP_IPA_AED_DG_VI, OPCOES_COBERTURA_VI, true)
				|| (produto.getCobertura_map() != 0.0 && produto.getCobertura_map() == tabela_cobertura_morte_MAP_IPA_AED_DG_VI[0])
				|| (produto.getCobertura_map() != 0.0 && produto.getCobertura_morte() != produto.getCobertura_map() / 2)) {
				cod_err = COB_MAP;
			}
			else if(!verificaValorCobertura(produto.getCobertura_ipa(), tabela_cobertura_morte_MAP_IPA_AED_DG_VI, OPCOES_COBERTURA_VI, false)
				|| (produto.getCobertura_ipa() != 0.0 && produto.getCobertura_morte() != produto.getCobertura_ipa() && produto.getCobertura_morte() != produto.getCobertura_ipa() / 2)) {
				cod_err = COB_IPA;
			}
			else if(!verificaValorCobertura(produto.getCobertura_aed(), tabela_cobertura_morte_MAP_IPA_AED_DG_VI, OPCOES_COBERTURA_VI, false)
				|| (produto.getCobertura_aed() != 0.0 && produto.getCobertura_morte() != produto.getCobertura_aed())) {
				cod_err = COB_AED;
			}
			else if(produto.getDespesas_medicas() != 0.0 && !(produto.getDespesas_medicas() >= 100.0 && produto.getDespesas_medicas() <= 10000.0 && produto.getDespesas_medicas() <= (produto.getCobertura_morte() * 0.1)))
				cod_err = COB_DMHO;
			else if(!verificaValorCobertura(produto.getDoencas_graves(), tabela_cobertura_morte_MAP_IPA_AED_DG_VI, OPCOES_COBERTURA_VI-19, false))
				cod_err = COB_DG;
			else if(!verificaValorCobertura(produto.getDiaria_it(), tabela_cobertura_DIT_VI, OPCOES_COBERTURA_DIT_VI, false))
				cod_err = COB_DIT;
			break;
		case APIP:
			if(!verificaValorCobertura(produto.getCobertura_map(), tabela_cobertura_morte_MAP_IPA_AED_DG_VI, OPCOES_COBERTURA_VI, false)) {
				cod_err = COB_MAP;
			}
			else if(!verificaValorCobertura(produto.getCobertura_ipa(), tabela_cobertura_morte_MAP_IPA_AED_DG_VI, OPCOES_COBERTURA_VI, false)
					|| (produto.getCobertura_ipa() != 0.0 && produto.getCobertura_map() != produto.getCobertura_ipa())) {
				cod_err = COB_IPA;
			}
			else if(produto.getDespesas_medicas() != 0.0 && !(produto.getDespesas_medicas() >= 100.0 && produto.getDespesas_medicas() <= 10000.0 && produto.getDespesas_medicas() <= (produto.getCobertura_map() * 0.1)))
				cod_err = COB_DMHO;
			else if(!verificaValorCobertura(produto.getDiaria_it(), tabela_cobertura_DIT_VI, OPCOES_COBERTURA_DIT_VI, false))
				cod_err = COB_DIT;
			break;
		case APICP:
			if(!(produto.getCobertura_map() >= 10000.0 && produto.getCobertura_map() <= 150000.0))
				cod_err = COB_MAP;
			else if(!(produto.getCobertura_ipa() >= 10000.0 && produto.getCobertura_ipa() <= 150000.0)
					|| (produto.getCobertura_ipa() != 0.0 && produto.getCobertura_map() != produto.getCobertura_ipa()))
				cod_err = COB_IPA;
			else if(produto.getDespesas_medicas() != 0.0 && !(produto.getDespesas_medicas() >= 100.0 && produto.getDespesas_medicas() <= 10000.0 && produto.getDespesas_medicas() <= (produto.getCobertura_map() * 0.1)))
				cod_err = COB_DMHO;
			break;
	}
    return cod_err;
}

/*!
 *  Calcula, imprime os valores e retorna o valor do prêmio total conforme o perfil e o produto
 */
double calculaImprimePremio(const Produto &produto, const Perfil &perfil) {

	double taxa_morte = 0.0;
	double taxa_map = 0.0;
	double taxa_ipa = 0.0;
	double taxa_aed = 0.0;
	double taxa_franquia_dit = 0.0;
	double taxa_despesas_medicas = 0.0;
	double taxa_doencas_graves = 0.0;
	double taxa_majoracao = 0.0;
	double taxa_af = 0.0;
	double taxa_operacao = 0.0;
	double premio = 0.0;
	double cobertura_majoracao = 0.0;

    switch (produto.getTipo())
	{
		case VMS:
			taxa_morte = tabela_VMS_Morte[perfil.getIdade()][perfil.getSegregacao()-1];
			taxa_map = tabela_VMS_MAP[perfil.getIdade()][perfil.getSegregacao()-1];
			taxa_ipa = tabela_VMS_IPA[perfil.getIdade()][perfil.getSegregacao()-1];
			//***ALERTA ! No produto Vida Mais Simples, a cobertura de Assistência Funeral Titular é gratuita.
			if(produto.getFuneral_familia() == SIM) {
				taxa_af = tabela_VMS_AFF[perfil.getIdade()][perfil.getSegregacao()-1];
			}
			else
				taxa_af = 0.0;
			std::cout << "\nVida Mais Simples\n";
			std::cout << "Cobert morte	: " << produto.getCobertura_morte() << "\n";
			std::cout << "Cobert MAP	: " << produto.getCobertura_map() << "\n";
			std::cout << "Cobert IPA	: " << produto.getCobertura_ipa() << "\n";
			std::cout << "Cobert AF	: " << produto.getCobertura_aff() << "\n";
			std::cout << "Taxa de morte	: " << taxa_morte << "\n";
			std::cout << "Taxa de MAP	: " << taxa_map << "\n";
			std::cout << "Taxa de IPA	: " << taxa_ipa << "\n";
			std::cout << "Taxa de AF	: " << taxa_af << "\n";

		    premio =  ( roundf( produto.getCobertura_morte() * taxa_morte * 100 ) / 100 +
		             	 	   roundf( produto.getCobertura_map() / 2.0 * taxa_map * 100 ) / 100 +
							   roundf( produto.getCobertura_ipa() * taxa_ipa * 100 ) / 100 +
							   roundf( produto.getCobertura_aff() * taxa_af * 100 ) / 100 ) * ( 1.0 + produto.getIof());
			std::cout << std::fixed << std::setprecision(2) << "Prêmio	: " << premio << "\n";
			break;
		case VMM:
			taxa_morte = tabela_VMM_Morte[perfil.getIdade()][perfil.getSegregacao()-1];
			taxa_map = tabela_VMM_CANCER[perfil.getIdade()][perfil.getSegregacao()-1];
			taxa_ipa = tabela_VMM_IPA[perfil.getIdade()][perfil.getSegregacao()-1];
			if(produto.getFuneral_familia() == SIM) {
				taxa_af = tabela_AFF[perfil.getIdade()][perfil.getSegregacao()-1];
			}
			else {
				taxa_af = tabela_AFT[perfil.getIdade()][perfil.getSegregacao()-1];
			}
			for (int i = 0; i < LINHAS_TABELA_OPERACAO; ++i) {
				if(produto.getOperacao() == (int)(tabela_taxas_COD_OP[i][0]))
				{
					taxa_operacao = tabela_taxas_COD_OP[i][1];
					break;
				}
			}
			std::cout << "\nVida Mais Mulher\n";
			std::cout << "Cobert morte	: " << produto.getCobertura_morte() << "\n";
			std::cout << "Cobert Câncer	: " << produto.getCobertura_map() << "\n";		//Cobertura CÂNCER
			std::cout << "Cobert IPA	: " << produto.getCobertura_ipa() << "\n";
			std::cout << "Cobert AF	: " << produto.getCobertura_aff() << "\n";
			std::cout << "Taxa de morte	: " << taxa_morte << "\n";
			std::cout << "Taxa Câncer	: " << taxa_map << "\n";
			std::cout << "Taxa de IPA	: " << taxa_ipa << "\n";
			std::cout << "Taxa de AF	: " << taxa_af << "\n";
			std::cout << "Taxa Oper	: " << taxa_operacao << "\n";

		    premio =  ( roundf( produto.getCobertura_morte() * taxa_morte * taxa_operacao * 100 ) / 100 +
		             	 	   roundf( produto.getCobertura_map() * taxa_map * taxa_operacao * 100 ) / 100 +
							   roundf( produto.getCobertura_ipa() * taxa_ipa * taxa_operacao * 100 ) / 100 +
							   roundf( produto.getCobertura_aff() * taxa_af * taxa_operacao * 100 ) / 100 ) * ( 1.0 + produto.getIof());
			std::cout << std::fixed << std::setprecision(2) << "Prêmio	: " << premio << "\n";
			break;
		case VI:
			for (int i = 0; i < LINHAS_TABELA_OPERACAO; ++i) {
				if(produto.getOperacao() == (int)(tabela_taxas_COD_OP[i][0]))
				{
					taxa_operacao = tabela_taxas_COD_OP[i][1];
					break;
				}
			}
			taxa_morte = tabela_VI_Morte[perfil.getIdade()][perfil.getSegregacao()-1];
			taxa_map = tabela_VI_MAP[perfil.getIdade()][perfil.getSegregacao()-1];
			taxa_ipa = tabela_VI_IPA[perfil.getIdade()][perfil.getSegregacao()-1];
			taxa_aed = tabela_VI_AED[perfil.getIdade()][perfil.getSegregacao()-1];
			if(produto.getFuneral_familia() == SIM) {
				taxa_af = tabela_AFF[perfil.getIdade()][perfil.getSegregacao()-1];
			}
			else {
				taxa_af = tabela_AFT[perfil.getIdade()][perfil.getSegregacao()-1];
			}
			for(int i = 0; i < LINHAS_DIT; i++) {
				if(int(tabela_taxa_DIT[i][0]) == perfil.getGrupo()
						&& int(tabela_taxa_DIT[i][1]) == perfil.getIdade()
						&& int(tabela_taxa_DIT[i][2]) == (int)(produto.getDiaria_it())
						&& int(tabela_taxa_DIT[i][3]) == perfil.getSexo()
						)
				{
					taxa_franquia_dit = tabela_taxa_DIT[i][4];
					break;
				}
			}
			taxa_despesas_medicas = tabela_VI_DMHO[perfil.getIdade()][0];

			if(produto.getTipo_doenca_grave() == 6) {
				if(perfil.getSexo() == perfil.FEMININO) {
					taxa_doencas_graves = tabela_VI_DG_FEM_6TIPOS[perfil.getIdade()-18][1];
				}
				else {
					taxa_doencas_graves = tabela_VI_DG_MASC_6TIPOS[perfil.getIdade()-18][1];
				}
			}
			else if(produto.getTipo_doenca_grave() == 10) {
				if(perfil.getSexo() == perfil.FEMININO) {
					taxa_doencas_graves = tabela_VI_DG_FEM_10TIPOS[perfil.getIdade()-18][1];
				}
				else {
					taxa_doencas_graves = tabela_VI_DG_MASC_10TIPOS[perfil.getIdade()-18][1];
				}
			}

			taxa_majoracao = tabela_VI_MAJ[perfil.getIdade()][0/*perfil.getSegregacao()-1*/]; //Alterada taxa de majoração em 07/12/16

			std::cout << "\nVida Individual\n";
			std::cout << "Cobert morte	: " << produto.getCobertura_morte() << "\n";
			std::cout << "Cobert MAP	: " << produto.getCobertura_map() << "\n";
			std::cout << "Cobert IPA	: " << produto.getCobertura_ipa() << "\n";
			std::cout << "Cobert AED	: " << produto.getCobertura_aed() << "\n";
			std::cout << "Cobert AF	: " << produto.getCobertura_aff() << "\n";
			std::cout << "Cobert Diaria	: " << produto.getDiaria_it() << "\n";
			std::cout << "Cobert DMHO	: " << produto.getDespesas_medicas() << "\n";
			std::cout << "Cobert DG	: " << produto.getDoencas_graves() << "\n";
			if(produto.getMajoracao() == SIM)
			{
				std::cout << "Cobert MAJ	: " << produto.getCobertura_ipa() << "\n";
				cobertura_majoracao = produto.getCobertura_ipa();
			}
			else {
				cobertura_majoracao = 0.0;
			}

			std::cout << "Taxa Oper	: " << taxa_operacao << "\n";

			std::cout << "Taxa de morte	: " << taxa_morte << "\n";
			std::cout << "Taxa de MAP	: " << taxa_map << "\n";
			std::cout << "Taxa de IPA	: " << taxa_ipa << "\n";
			std::cout << "Taxa de AED	: " << taxa_aed << "\n";
			std::cout << "Taxa de AF	: " << taxa_af << "\n";
			std::cout << "Taxa DIT	: " << taxa_franquia_dit << "\n";
			std::cout << "Taxa DMHO	: " << taxa_despesas_medicas << "\n";
			std::cout << "Taxa DG		: " << taxa_doencas_graves << "\n";
			std::cout << "Taxa MAJ	: "<< taxa_majoracao << "\n";

			std::cout << std::fixed << std::setprecision(2) << "Prêmio morte	: " << roundf	( produto.getCobertura_morte()		*	taxa_morte				*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 << "\n";
			std::cout << std::fixed << std::setprecision(2) << "Prêmio map	: " << roundf	( produto.getCobertura_map()		*	taxa_map / 2.0				*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 << "\n";
			std::cout << std::fixed << std::setprecision(2) << "Prêmio ipa	: " << roundf	( produto.getCobertura_ipa()		*	taxa_ipa				*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 << "\n";
			std::cout << std::fixed << std::setprecision(2) << "Prêmio aed	: " << roundf	( produto.getCobertura_aed()		*	taxa_aed				*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 << "\n";
			std::cout << std::fixed << std::setprecision(2) << "Prêmio aft 	: " << roundf	( produto.getCobertura_aff()		*	taxa_af				*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 << "\n";

			if(produto.getFranquia_reduzida() == NAO) {
				std::cout << std::fixed << std::setprecision(2) << "Prêmio dit	: " << roundf	( produto.getDiaria_it()			*	taxa_franquia_dit		*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 << "\n";
				premio = (
					roundf	( produto.getCobertura_morte()		*	taxa_morte				*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
					roundf	( produto.getCobertura_map()		*	taxa_map / 2.0			*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
					roundf	( produto.getCobertura_ipa()		*	taxa_ipa				*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
					roundf	( produto.getCobertura_aed()		*	taxa_aed				*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
					roundf	( produto.getCobertura_aff()		*	taxa_af					*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
					roundf	( produto.getDiaria_it()			*	taxa_franquia_dit		*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
					roundf	( produto.getDespesas_medicas()		*	taxa_despesas_medicas	*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
					roundf	( produto.getDoencas_graves()		*	taxa_doencas_graves		*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
					roundf	( cobertura_majoracao				*	taxa_majoracao			*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100
				);
			}
			else {
				std::cout << std::fixed << std::setprecision(2) << "Prêmio dit	: " << roundf	( produto.getDiaria_it()			*	taxa_franquia_dit	*	( 1 + 0.21 )	*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 << "\n";
				premio = (
					roundf	( produto.getCobertura_morte()		*	taxa_morte				*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
					roundf	( produto.getCobertura_map()		*	taxa_map / 2.0			*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
					roundf	( produto.getCobertura_ipa()		*	taxa_ipa				*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
					roundf	( produto.getCobertura_aed()		*	taxa_aed				*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
					roundf	( produto.getCobertura_aff()		*	taxa_af					*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
					roundf	( produto.getDiaria_it()			*	taxa_franquia_dit 		*	( 1 + 0.21 )		*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
					roundf	( produto.getDespesas_medicas()		*	taxa_despesas_medicas	*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
					roundf	( produto.getDoencas_graves()		*	taxa_doencas_graves		*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
					roundf	( cobertura_majoracao		*	taxa_majoracao			*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100
				);
			}
			std::cout << std::fixed << std::setprecision(2) << "Prêmio dmho	: " << roundf	( produto.getDespesas_medicas()		*	taxa_despesas_medicas	*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 << "\n";
			std::cout << std::fixed << std::setprecision(2) << "Prêmio dg	: " << roundf	( produto.getDoencas_graves()		*	taxa_doencas_graves		*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 << "\n";
			std::cout << std::fixed << std::setprecision(2) << "Prêmio major	: " << roundf	( cobertura_majoracao	*	taxa_majoracao			*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 << "\n";

			std::cout << std::fixed << std::setprecision(2) << "Prêmio total	: " << premio << "\n";
			break;
		case APIP:
			taxa_map = tabela_VI_MAP[perfil.getIdade()][0];
			taxa_ipa = tabela_VI_IPA[perfil.getIdade()][0];
			taxa_despesas_medicas = tabela_VI_DMHO[perfil.getIdade()][0];
			if(produto.getFuneral_familia() == SIM) {
				taxa_af = TAXA_APIP_AFF;
			}
			else {
				taxa_af = TAXA_APIP_AFT;
			}
			taxa_majoracao = tabela_APIP_MAJ[perfil.getIdade()];
			std::cout << "\nAcidentes Pessoais Individual Plus\n";
			std::cout << "Cobert MAP	: " << produto.getCobertura_map() << "\n";
			std::cout << "Cobert IPA	: " << produto.getCobertura_ipa() << "\n";
			std::cout << "Cobert DMHO	: " << produto.getDespesas_medicas() << "\n";
			if(produto.getMajoracao() == SIM)
			{
				std::cout << "Cobert MAJ	: " << produto.getCobertura_ipa() << "\n";
				cobertura_majoracao = produto.getCobertura_ipa();
			}
			else {
				cobertura_majoracao = 0.0;
			}
			std::cout << "Cobert Diaria	: " << produto.getDiaria_it() << "\n";
			std::cout << "Taxa de MAP	: " << taxa_map << "\n";
			std::cout << "Taxa de IPA	: " << taxa_ipa << "\n";
			std::cout << "Taxa DMHO	: " << taxa_despesas_medicas << "\n";
			std::cout << "Taxa MAJ	: "<< taxa_majoracao << "\n";
			for(int i = 0; i < LINHAS_DIT; i++) {
				if(int(tabela_taxa_DIT[i][0]) == perfil.getGrupo()
						&& int(tabela_taxa_DIT[i][1]) == perfil.getIdade()
						&& int(tabela_taxa_DIT[i][2]) == (int)(produto.getDiaria_it())
						&& int(tabela_taxa_DIT[i][3]) == perfil.getSexo()
						)
				{
					taxa_franquia_dit = tabela_taxa_DIT[i][4];
					break;
				}
			}
			std::cout << "Taxa DIT	: " << taxa_franquia_dit << "\n";
			for (int i = 0; i < LINHAS_TABELA_OPERACAO; ++i) {
				if(produto.getOperacao() == (int)(tabela_taxas_COD_OP[i][0]))
				{
					taxa_operacao = tabela_taxas_COD_OP[i][1];
					break;
				}
			}
			std::cout << "Taxa Oper	: " << taxa_operacao << "\n";
			std::cout << std::fixed << std::setprecision(2) << "Prêmio map	: " << roundf	( produto.getCobertura_map()		*	taxa_map				*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 << "\n";
			std::cout << std::fixed << std::setprecision(2) << "Prêmio ipa	: " << roundf	( produto.getCobertura_ipa()		*	taxa_ipa				*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 << "\n";
			std::cout << std::fixed << std::setprecision(2) << "Prêmio dmho	: " << roundf	( produto.getDespesas_medicas()		*	taxa_despesas_medicas	*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 << "\n";
			std::cout << std::fixed << std::setprecision(2) << "Prêmio major	: " << roundf	( cobertura_majoracao	*	taxa_majoracao			*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 << "\n";
			if(produto.getFranquia_reduzida() == NAO) {
				std::cout << std::fixed << std::setprecision(2) << "Prêmio dit	: " << roundf	( produto.getDiaria_it()			*	taxa_franquia_dit		*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 << "\n";
				premio = (
					roundf	( produto.getCobertura_aff()		*	taxa_af					*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
					roundf	( produto.getCobertura_map()		*	taxa_map				*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
					roundf	( produto.getCobertura_ipa()		*	taxa_ipa				*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
					roundf	( produto.getDespesas_medicas()		*	taxa_despesas_medicas	*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
					roundf	( cobertura_majoracao				*	taxa_majoracao			*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
					roundf	( produto.getDiaria_it()			*	taxa_franquia_dit		*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100
				);
			}
			else {
				std::cout << std::fixed << std::setprecision(2) << "Prêmio dit	: " << roundf	( produto.getDiaria_it()			*	taxa_franquia_dit	*	( 1 + 0.21 )		*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 << "\n";
				premio = (
					roundf	( produto.getCobertura_aff()		*	taxa_af					*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
					roundf	( produto.getCobertura_map()		*	taxa_map				*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
					roundf	( produto.getCobertura_ipa()		*	taxa_ipa				*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
					roundf	( produto.getDespesas_medicas()		*	taxa_despesas_medicas	*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
					roundf	( cobertura_majoracao				*	taxa_majoracao			*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
					roundf	( produto.getDiaria_it()			*	taxa_franquia_dit	*	( 1 + 0.21 )		*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100
				);
			}
			std::cout << std::fixed << std::setprecision(2) << "Prêmio total	: " << premio << "\n";
			break;
		case APICP:
			taxa_map = tabela_APICP_MAP[perfil.getIdade()];
			taxa_ipa = tabela_APICP_IPA[perfil.getIdade()];
			taxa_despesas_medicas = tabela_APICP_DMHO[perfil.getIdade()];
			std::cout << "\nAcidentes Pessoais Individual Prazo Curto\n";
			std::cout << "Cobert MAP	: " << produto.getCobertura_map() << "\n";
			std::cout << "Cobert IPA	: " << produto.getCobertura_ipa() << "\n";
			std::cout << "Cobert DMHO	: " << produto.getDespesas_medicas() << "\n";
			std::cout << "Vigência	: " << perfil.getVigencia() << "\n";
			for (int i = 0; i < LINHAS_TABELA_OPERACAO_APICP; ++i) {
				if(produto.getOperacao() == (int)(tabela_taxas_COD_OP_APICP[i][0]))
				{
					taxa_operacao = tabela_taxas_COD_OP_APICP[i][1];
					break;
				}
			}

			double taxa_percentual_premio = (double)(tabela_APICP_PERCENTUAL_PREMIO[perfil.getVigencia()]) / 100.0;
			std::cout << "Taxa de MAP	: " << taxa_map << "\n";
			std::cout << "Taxa de IPA	: " << taxa_ipa << "\n";
			std::cout << "Taxa DMHO	: " << taxa_despesas_medicas << "\n";
			std::cout << "Taxa Vigência	: " << taxa_percentual_premio * 100 << "%" << "\n";
			std::cout << "Taxa Oper	: " << taxa_operacao << "\n";

			premio = (
				roundf 	( produto.getCobertura_map()	*	taxa_map				*	taxa_percentual_premio 	*	taxa_operacao	*	100	*	( 1.0 + produto.getIof() ) ) / 100 +
				roundf	( produto.getCobertura_ipa()	*	taxa_ipa				*	taxa_percentual_premio	*	taxa_operacao	*	100	*	( 1.0 + produto.getIof() ) ) / 100 +
				roundf	( produto.getDespesas_medicas()	*	taxa_despesas_medicas	*	taxa_percentual_premio	*	taxa_operacao	*	100	*	( 1.0 + produto.getIof() ) ) / 100
			);
			std::cout << std::fixed << std::setprecision(2) << "Prêmio total	: " << premio << "\n";
			break;
    }
    return premio;
}


/*!
 *  Calcula e retorna o valor do prêmio total conforme o perfil e o produto
 */
double calculaPremio(const Produto &produto, const Perfil &perfil) {

	double taxa_morte = 0.0;
	double taxa_map = 0.0;
	double taxa_ipa = 0.0;
	double taxa_aed = 0.0;
	double taxa_franquia_dit = 0.0;
	double taxa_despesas_medicas = 0.0;
	double taxa_doencas_graves = 0.0;
	double taxa_majoracao = 0.0;
	double taxa_af = 0.0;
	double taxa_operacao = 0.0;
	double premio = 0.0;
	double cobertura_majoracao = 0.0;

    switch (produto.getTipo())
	{
		case VMS:
			/*! Vida Mais Simples\n
			\f{eqnarray*}{
				Premio&=&(CobMorte*TaxaMorte \\
				&+&\frac{CobMorteAcidental}{2.0}*TaxaMorteAcidental \\
				&+&CobInvalidezAcidente*TaxaInvalidezAcidente \\
				&+&CobAssistenciaFuneral*TaxaAssistenciaFuneral) \\
				&*&(1+\frac{IOF}{100})
			\f}
   			*/
			taxa_morte = tabela_VMS_Morte[perfil.getIdade()][perfil.getSegregacao()-1];
			taxa_map = tabela_VMS_MAP[perfil.getIdade()][perfil.getSegregacao()-1];
			taxa_ipa = tabela_VMS_IPA[perfil.getIdade()][perfil.getSegregacao()-1];
			//***ALERTA ! No produto Vida Mais Simples, a cobertura de Assistência Funeral Titular é gratuita.
			if(produto.getFuneral_familia() == SIM) {
				taxa_af = tabela_VMS_AFF[perfil.getIdade()][perfil.getSegregacao()-1];
			}
			else
				taxa_af = 0.0;
		    premio =  ( round( produto.getCobertura_morte() * taxa_morte * 100 ) / 100 +
		    			round( produto.getCobertura_map() / 2.0 * taxa_map * 100 ) / 100 +
						round( produto.getCobertura_ipa() * taxa_ipa * 100 ) / 100 +
						round( produto.getCobertura_aff() * taxa_af * 100 ) / 100 ) * ( 1.0 + produto.getIof());
			break;
		case VMM:
			/*! Vida Mais Mulher\n
			\f{eqnarray*}{
				Premio&=&(CobMorte*TaxaMorte*Operacao \\
				&+&CobCancer*TaxaCancer*Operacao \\
				&+&CobInvalidezAcidente*TaxaInvalidezAcidente*Operacao \\
				&+&CobAssistenciaFuneral*TaxaAssistenciaFuneral*Operacao) \\
				&*&(1+\frac{IOF}{100})
			\f}
   			*/
			taxa_morte = tabela_VMM_Morte[perfil.getIdade()][perfil.getSegregacao()-1];
			taxa_map = tabela_VMM_CANCER[perfil.getIdade()][perfil.getSegregacao()-1];
			taxa_ipa = tabela_VMM_IPA[perfil.getIdade()][perfil.getSegregacao()-1];
			if(produto.getFuneral_familia() == SIM) {
				taxa_af = tabela_AFF[perfil.getIdade()][perfil.getSegregacao()-1];
			}
			else {
				taxa_af = tabela_AFT[perfil.getIdade()][perfil.getSegregacao()-1];
			}
			for (int i = 0; i < LINHAS_TABELA_OPERACAO; ++i) {
				if(produto.getOperacao() == (int)(tabela_taxas_COD_OP[i][0]))
				{
					taxa_operacao = tabela_taxas_COD_OP[i][1];
					break;
				}
			}
		    premio =  ( round( produto.getCobertura_morte() * taxa_morte * taxa_operacao * 100 ) / 100 +
		    			round( produto.getCobertura_map() * taxa_map * taxa_operacao * 100 ) / 100 +
						round( produto.getCobertura_ipa() * taxa_ipa * taxa_operacao * 100 ) / 100 +
						round( produto.getCobertura_aff() * taxa_af * taxa_operacao * 100 ) / 100 ) * ( 1.0 + produto.getIof());
			break;
		case VI:
			/*! Vida Individual\n
			\f{eqnarray*}{
				Premio&=&((CobMorte*TaxaMorte*Operacao*(1+\frac{IOF}{100})) \\
				&+&(\frac{CobMorteAcidental}{2.0}*TaxaMorteAcidental*Operacao*(1+\frac{IOF}{100})) \\
				&+&(CobInvalidezAcidente*TaxaInvalidezAcidente*Operacao*(1+\frac{IOF}{100})) \\
				&+&(CobAntecipacaoDoenca*TaxaAntecipacaoDoenca*Operacao*(1+\frac{IOF}{100})) \\
				&+&(CobAssistenciaFuneral*TaxaAssistenciaFuneral*Operacao*(1+\frac{IOF}{100})) \\
				&+&(CobDiariaDIT*TaxaDiariaDIT*Franquia*Operacao*(1+\frac{IOF}{100})) \\
				&+&(CobDespesasMedicas*TaxaDespesasMedicas*Operacao*(1+\frac{IOF}{100})) \\
				&+&(CobDoencasGraves*TaxaDoencasGraves*Operacao*(1+\frac{IOF}{100})) \\
				&+&(CobMajoracao*TaxaMajoracao*Operacao*(1+\frac{IOF}{100})))
			\f}
   			*/
			for (int i = 0; i < LINHAS_TABELA_OPERACAO; ++i) {
				if(produto.getOperacao() == (int)(tabela_taxas_COD_OP[i][0]))
				{
					taxa_operacao = tabela_taxas_COD_OP[i][1];
					break;
				}
			}
			taxa_morte = tabela_VI_Morte[perfil.getIdade()][perfil.getSegregacao()-1];
			taxa_map = tabela_VI_MAP[perfil.getIdade()][perfil.getSegregacao()-1];
			taxa_ipa = tabela_VI_IPA[perfil.getIdade()][perfil.getSegregacao()-1];
			taxa_aed = tabela_VI_AED[perfil.getIdade()][perfil.getSegregacao()-1];
			if(produto.getFuneral_familia() == SIM) {
				taxa_af = tabela_AFF[perfil.getIdade()][perfil.getSegregacao()-1];
			}
			else {
				taxa_af = tabela_AFT[perfil.getIdade()][perfil.getSegregacao()-1];
			}
			for(int i = 0; i < LINHAS_DIT; i++) {
				if(int(tabela_taxa_DIT[i][0]) == perfil.getGrupo()
						&& int(tabela_taxa_DIT[i][1]) == perfil.getIdade()
						&& int(tabela_taxa_DIT[i][2]) == (int)(produto.getDiaria_it())
						&& int(tabela_taxa_DIT[i][3]) == perfil.getSexo()
						)
				{
					taxa_franquia_dit = tabela_taxa_DIT[i][4];
					break;
				}
			}
			taxa_despesas_medicas = tabela_VI_DMHO[perfil.getIdade()][0];
			if(produto.getTipo_doenca_grave() == 6) {
				if(perfil.getSexo() == perfil.FEMININO) {
					taxa_doencas_graves = tabela_VI_DG_FEM_6TIPOS[perfil.getIdade()-18][1];
				}
				else {
					taxa_doencas_graves = tabela_VI_DG_MASC_6TIPOS[perfil.getIdade()-18][1];
				}
			}
			else if(produto.getTipo_doenca_grave() == 10) {
				if(perfil.getSexo() == perfil.FEMININO) {
					taxa_doencas_graves = tabela_VI_DG_FEM_10TIPOS[perfil.getIdade()-18][1];
				}
				else {
					taxa_doencas_graves = tabela_VI_DG_MASC_10TIPOS[perfil.getIdade()-18][1];
				}
			}

			taxa_majoracao = tabela_VI_MAJ[perfil.getIdade()][0/*perfil.getSegregacao()-1*/]; //Alterada taxa de majoração em 07/12/16
			if(produto.getMajoracao() == SIM)
			{
				cobertura_majoracao = produto.getCobertura_ipa();
			}
			else {
				cobertura_majoracao = 0.0;
			}

			if(produto.getFranquia_reduzida() == NAO) {
				premio = (
						round	( produto.getCobertura_morte()		*	taxa_morte				*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
						round	( produto.getCobertura_map()		*	taxa_map / 2.0			*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
						round	( produto.getCobertura_ipa()		*	taxa_ipa				*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
						round	( produto.getCobertura_aed()		*	taxa_aed				*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
						round	( produto.getCobertura_aff()		*	taxa_af					*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
						round	( produto.getDiaria_it()			*	taxa_franquia_dit		*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
						round	( produto.getDespesas_medicas()		*	taxa_despesas_medicas	*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
						round	( produto.getDoencas_graves()		*	taxa_doencas_graves		*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
						round	( cobertura_majoracao				*	taxa_majoracao			*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100
				);
			}
			else {
				premio = (
						round	( produto.getCobertura_morte()		*	taxa_morte				*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
						round	( produto.getCobertura_map()		*	taxa_map / 2.0			*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
						round	( produto.getCobertura_ipa()		*	taxa_ipa				*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
						round	( produto.getCobertura_aed()		*	taxa_aed				*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
						round	( produto.getCobertura_aff()		*	taxa_af					*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
						round	( produto.getDiaria_it()			*	taxa_franquia_dit 		*	( 1 + 0.21 )		*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
						round	( produto.getDespesas_medicas()		*	taxa_despesas_medicas	*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
						round	( produto.getDoencas_graves()		*	taxa_doencas_graves		*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
						round	( cobertura_majoracao				*	taxa_majoracao			*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100
				);
			}
			break;
		case APIP:
			/*! Acidentes Pessoais Individual Plus\n
			\f{eqnarray*}{
				Premio&=&((CobMorteAcidental*TaxaMorteAcidental*Operacao*(1+\frac{IOF}{100})) \\
				&+&(CobInvalidezAcidente*TaxaInvalidezAcidente*Operacao*(1+\frac{IOF}{100})) \\
				&+&(CobDespesasMedicas*TaxaDespesasMedicas*Operacao*(1+\frac{IOF}{100})) \\
				&+&(CobMajoracao*TaxaMajoracao*Operacao*(1+\frac{IOF}{100})) \\
				&+&(CobDiariaDIT*TaxaDiariaDIT*Franquia*Operacao*(1+\frac{IOF}{100})))
			\f}
   			*/
			taxa_map = tabela_VI_MAP[perfil.getIdade()][0];
			taxa_ipa = tabela_VI_IPA[perfil.getIdade()][0];
			taxa_despesas_medicas = tabela_VI_DMHO[perfil.getIdade()][0];
			if(produto.getFuneral_familia() == SIM) {
				taxa_af = TAXA_APIP_AFF;
			}
			else {
				taxa_af = TAXA_APIP_AFT;
			}
			taxa_majoracao = tabela_APIP_MAJ[perfil.getIdade()];
			if(produto.getMajoracao() == SIM)
			{
				cobertura_majoracao = produto.getCobertura_ipa();
			}
			else {
				cobertura_majoracao = 0.0;
			}
			for(int i = 0; i < LINHAS_DIT; i++) {
				if(int(tabela_taxa_DIT[i][0]) == perfil.getGrupo()
						&& int(tabela_taxa_DIT[i][1]) == perfil.getIdade()
						&& int(tabela_taxa_DIT[i][2]) == (int)(produto.getDiaria_it())
						&& int(tabela_taxa_DIT[i][3]) == perfil.getSexo()
						)
				{
					taxa_franquia_dit = tabela_taxa_DIT[i][4];
					break;
				}
			}
			for (int i = 0; i < LINHAS_TABELA_OPERACAO; ++i) {
				if(produto.getOperacao() == (int)(tabela_taxas_COD_OP[i][0]))
				{
					taxa_operacao = tabela_taxas_COD_OP[i][1];
					break;
				}
			}
			if(produto.getFranquia_reduzida() == NAO) {
				premio = (
						round	( produto.getCobertura_aff()		*	taxa_af					*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
						round	( produto.getCobertura_map()		*	taxa_map				*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
						round	( produto.getCobertura_ipa()		*	taxa_ipa				*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
						round	( produto.getDespesas_medicas()		*	taxa_despesas_medicas	*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
						round	( cobertura_majoracao				*	taxa_majoracao			*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
						round	( produto.getDiaria_it()			*	taxa_franquia_dit		*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100
				);
			}
			else {
				premio = (
						round	( produto.getCobertura_aff()		*	taxa_af					*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
						round	( produto.getCobertura_map()		*	taxa_map				*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
						round	( produto.getCobertura_ipa()		*	taxa_ipa				*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
						round	( produto.getDespesas_medicas()		*	taxa_despesas_medicas	*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
						round	( cobertura_majoracao				*	taxa_majoracao			*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
						round	( produto.getDiaria_it()			*	taxa_franquia_dit	*	( 1 + 0.21 )		*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100
				);
			}
			break;
		case APICP:
			/*! Acidentes Pessoais Individual Curto Prazo\n
			\f{eqnarray*}{
				Premio&=&((CobMorteAcidental*TaxaMorteAcidental*\frac{PercDiasVigencia}{100}*Operacao*(1+\frac{IOF}{100})) \\
				&+&(CobInvalidezAcidente*TaxaInvalidezAcidente*\frac{PercDiasVigencia}{100}*Operacao*(1+\frac{IOF}{100})) \\
				&+&(CobDespesasMedicas*TaxaDespesasMedicas*\frac{PercDiasVigencia}{100}*Operacao*(1+\frac{IOF}{100})))
			\f}
   			*/
			taxa_map = tabela_APICP_MAP[perfil.getIdade()];
			taxa_ipa = tabela_APICP_IPA[perfil.getIdade()];
			taxa_despesas_medicas = tabela_APICP_DMHO[perfil.getIdade()];
			for (int i = 0; i < LINHAS_TABELA_OPERACAO_APICP; ++i) {
				if(produto.getOperacao() == (int)(tabela_taxas_COD_OP_APICP[i][0]))
				{
					taxa_operacao = tabela_taxas_COD_OP_APICP[i][1];
					break;
				}
			}
			double taxa_percentual_premio = (double)(tabela_APICP_PERCENTUAL_PREMIO[perfil.getVigencia()]) / 100.0;
			premio = (
					round 	( produto.getCobertura_map()	*	taxa_map				*	taxa_percentual_premio 	*	taxa_operacao	*	100	*	( 1.0 + produto.getIof() ) ) / 100 +
					round	( produto.getCobertura_ipa()	*	taxa_ipa				*	taxa_percentual_premio	*	taxa_operacao	*	100	*	( 1.0 + produto.getIof() ) ) / 100 +
					round	( produto.getDespesas_medicas()	*	taxa_despesas_medicas	*	taxa_percentual_premio	*	taxa_operacao	*	100	*	( 1.0 + produto.getIof() ) ) / 100
			);
			break;
    }
    return premio;
}

/*!
 *  Recebe o valor calculado do prêmio e salva um JSON
 */
void montaJSONPremio(double premio,const char* output_file) {

	std::ofstream file_id;
	file_id.open(output_file);

	Json::Value event;
	event["Premio"].append(Json::Value(premio));

	Json::StyledStreamWriter styledStream;
    styledStream.write(file_id,event);
    file_id.close();
}

std::string gravaValorPremio_(const std::string input_string_perfil,const std::string input_string_produto) {

 	int cod_err = 0;

	Perfil perfil;
	criaPerfil(returnJsonFromString(input_string_perfil),perfil);

	Produto produto;
	cod_err = criaProduto(returnJsonFromString(input_string_produto),produto);

	Json::Value output_val;

	if(perfil.getGrupo() == 255) {
		cod_err = PROFISSAO_INVALIDA;
	}

	if(cod_err) {
		output_val["resultado"]["codigo"] = cod_err;
		output_val["resultado"]["descricao"] = Erros[cod_err-100];
	}
	else {
		if(produto.getMajoracao() == SIM && ((perfil.getProfissao() != MEDICO && perfil.getProfissao() != DENTISTA)
				|| produto.getCobertura_ipa() == produto.getCobertura_morte() * 2
				|| produto.getCobertura_ipa() == produto.getCobertura_map() * 2)) {
			cod_err = MAJORACAO_PROFISSAO_DOBRO;
		}
		else if(produto.getDiaria_it() != 0.0 && (perfil.getGrupo() == BLANK
				|| produto.getCobertura_morte() >= VALOR_MAXIMO_DIT
				|| (produto.getTipo() == APIP && produto.getCobertura_map() >= VALOR_MAXIMO_DIT))) {
			cod_err = COB_DIT_INVALIDA;
		}
		else if(perfil.getRegime() == perfil.CLT && produto.getFranquia_reduzida() == SIM) {
			cod_err = FRANQUIA_DIT_INVALIDA;
		}
		if(cod_err) {
			output_val["resultado"]["codigo"] = cod_err;
			output_val["resultado"]["descricao"] = Erros[cod_err-100];
		}
	}
	if(!cod_err) {
		string valor_premio = static_cast<std::ostringstream*>( &(std::ostringstream() << calculaPremio(produto,perfil)) )->str();
		output_val["calculosVida"]["premioTotal"] = valor_premio;
	}

	return returnStringFromJson(output_val);
}

/*double gravaValorPremio(const char* perfil_input_file, const char* produto_input_file, const char* output_file) {

	Perfil perfil;
	Json::Value valor_perfil = retornaValueJSON(perfil_input_file);
	criaPerfil(valor_perfil,perfil);

	Produto produto;
	Json::Value valor_produto = retornaValueJSON(produto_input_file);
	criaProduto(valor_produto,produto);

	double premio = calculaPremio(produto,perfil);
	montaJSONPremio(premio,output_file);
	return premio;
}*/
