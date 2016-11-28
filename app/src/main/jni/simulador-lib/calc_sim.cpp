#include "calc_sim.h"

/*!
 *  Inicializa e retorna um objeto Perfil
 */
Perfil inicializaPerfil() {
	Perfil perfil;
	return perfil;
}

/*!
 *  Recebe um arquivo JSON e retorna um Value JSONCPP
 */
Json::Value retornaValueJSON(const char* arquivo) {
    Json::Value valor;
    ifstream ifs_perfil(arquivo);
    ifs_perfil >> valor;
    return valor;
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
			   if(key == "fumante")
				{
					val[key] == 1 ? perfil.setFumante(perfil.SIM) : perfil.setFumante(perfil.NAO);
				}
				else if(key == "idd") {
					perfil.setIdade(val[key].asLargestInt());
				}
				else if(key == "codigoProfissao") {
					Profissao profissao;
					profissao.setId(val[key].asLargestInt());
					perfil.setGrupo(profissao.verificaGrupoDIT(profissao.getId()));
				}
				else if(key == "profCLT") {
					val[key] == "S" ? perfil.setRegime(perfil.CLT) : perfil.setRegime(perfil.PJ);
				}
				else if(key == "sexo") {
					val[key] == 1 ? perfil.setSexo(perfil.MASCULINO) : perfil.setSexo(perfil.FEMININO);
				}
				else if(key == "vigenciaInicial") {
					perfil.setVigencia(val[key].asLargestInt());
				}
			}
			perfil.determinaFatorDeSegregacao();
		}
	}
}

/*!
 *  Cria uma lista de produtos disponíveis num arquivo JSON
 */
void listaProdutosIndicados(const char* input_file, const char* output_file) {

	Perfil perfil = inicializaPerfil();
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
}

/*!
 *  Inicializa e retorna um objeto Produto
 */
Produto inicializaProduto() {
	Produto produto;
	return produto;
}

inline bool verificaValorCobertura(double valor, const double tabela[], int linhas, bool dobro) {

	int mult;

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
void criaProduto(const Json::Value& val, Produto& produto) {

	if(val.type() == Json::objectValue) {
		if (val.empty())
			return;
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
	//Verifica ERROS nos valores passados
	std::ofstream file_id;
	file_id.open("erro_output.json");
	Json::Value event;
    switch (produto.getTipo()) {
		case VMS:
			if(!verificaValorCobertura(produto.getCobertura_morte(), tabela_cobertura_morte_MAP_IPA_VMS, OPCOES_COBERTURA, false))
				event["Erros"]["Vida Mais Simples"]["Morte"].append(Json::Value(produto.getCobertura_morte()).asDouble());
			if(!verificaValorCobertura(produto.getCobertura_map(), tabela_cobertura_morte_MAP_IPA_VMS, OPCOES_COBERTURA, true))
				event["Erros"]["Vida Mais Simples"]["MAP"].append(Json::Value(produto.getCobertura_map()).asDouble());
			if(!verificaValorCobertura(produto.getCobertura_ipa(), tabela_cobertura_morte_MAP_IPA_VMS, OPCOES_COBERTURA, false))
				event["Erros"]["Vida Mais Simples"]["IPA"].append(Json::Value(produto.getCobertura_ipa()).asDouble());
			break;
		case VMM:
			if(!verificaValorCobertura(produto.getCobertura_morte(), tabela_cobertura_morte_IPA_VMM, OPCOES_COBERTURA, false))
				event["Erros"]["Vida Mais Mulher"]["Morte"].append(Json::Value(produto.getCobertura_morte()).asDouble());
			if(!verificaValorCobertura(produto.getCobertura_map(), tabela_cobertura_CANCER_VMM, OPCOES_COBERTURA, false))
				event["Erros"]["Vida Mais Mulher"]["Câncer"].append(Json::Value(produto.getCobertura_map()).asDouble());
			if(!verificaValorCobertura(produto.getCobertura_ipa(), tabela_cobertura_morte_IPA_VMM, OPCOES_COBERTURA, false))
				event["Erros"]["Vida Mais Mulher"]["IPA"].append(Json::Value(produto.getCobertura_ipa()).asDouble());
			break;
		/*case VI:
			for (int i = 0; i < OPCOES_COBERTURA_VI; ++i) {
				if(produto.getCobertura_morte() == tabela_cobertura_morte_MAP_IPA_AED_DG_VI[i])
				if(produto.getCobertura_map() == (tabela_cobertura_morte_MAP_IPA_AED_DG_VI[i] * 2.0))
				if(produto.getCobertura_ipa() == (tabela_cobertura_morte_MAP_IPA_AED_DG_VI[i] * 2.0))
			}
			//Cobre valor mínimo se não for em DOBRO
			if(produto.getCobertura_map() == tabela_cobertura_morte_MAP_IPA_AED_DG_VI[0])
			//Cobre valor mínimo se não for em DOBRO
			if(produto.getCobertura_ipa() == tabela_cobertura_morte_MAP_IPA_AED_DG_VI[0])
			for (int i = 0; i < OPCOES_COBERTURA_DIT_VI; ++i) {
				if(produto.getCobertura_morte() == tabela_cobertura_DIT_VI[i])
			}
			for (int i = 0; i < OPCOES_COBERTURA_VI-19; ++i) {
				if(produto.getCobertura_morte() == tabela_cobertura_morte_MAP_IPA_AED_DG_VI[i])
			}
			break;
		case APIP:
			break;
		case APICP:
			break;
		default:
			//ERRO
			break;*/
	}
    Json::StyledStreamWriter styledStream;
    styledStream.write(file_id,event);
    file_id.close();
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

			taxa_majoracao = tabela_VI_MAJ[perfil.getIdade()][perfil.getSegregacao()-1];

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
		    premio =  ( roundf( produto.getCobertura_morte() * taxa_morte * 100 ) / 100 +
		             	 	   roundf( produto.getCobertura_map() / 2.0 * taxa_map * 100 ) / 100 +
							   roundf( produto.getCobertura_ipa() * taxa_ipa * 100 ) / 100 +
							   roundf( produto.getCobertura_aff() * taxa_af * 100 ) / 100 ) * ( 1.0 + produto.getIof());
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
		    premio =  ( roundf( produto.getCobertura_morte() * taxa_morte * taxa_operacao * 100 ) / 100 +
		             	 	   roundf( produto.getCobertura_map() * taxa_map * taxa_operacao * 100 ) / 100 +
							   roundf( produto.getCobertura_ipa() * taxa_ipa * taxa_operacao * 100 ) / 100 +
							   roundf( produto.getCobertura_aff() * taxa_af * taxa_operacao * 100 ) / 100 ) * ( 1.0 + produto.getIof());
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

			taxa_majoracao = tabela_VI_MAJ[perfil.getIdade()][perfil.getSegregacao()-1];
			if(produto.getMajoracao() == SIM)
			{
				cobertura_majoracao = produto.getCobertura_ipa();
			}
			else {
				cobertura_majoracao = 0.0;
			}

			if(produto.getFranquia_reduzida() == NAO) {
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
				premio = (
					roundf	( produto.getCobertura_morte()		*	taxa_morte				*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
					roundf	( produto.getCobertura_map()		*	taxa_map / 2.0			*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
					roundf	( produto.getCobertura_ipa()		*	taxa_ipa				*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
					roundf	( produto.getCobertura_aed()		*	taxa_aed				*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
					roundf	( produto.getCobertura_aff()		*	taxa_af					*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
					roundf	( produto.getDiaria_it()			*	taxa_franquia_dit 		*	( 1 + 0.21 )		*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
					roundf	( produto.getDespesas_medicas()		*	taxa_despesas_medicas	*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
					roundf	( produto.getDoencas_graves()		*	taxa_doencas_graves		*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
					roundf	( cobertura_majoracao				*	taxa_majoracao			*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100
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
					roundf	( produto.getCobertura_map()		*	taxa_map				*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
					roundf	( produto.getCobertura_ipa()		*	taxa_ipa				*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
					roundf	( produto.getDespesas_medicas()		*	taxa_despesas_medicas	*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
					roundf	( cobertura_majoracao				*	taxa_majoracao			*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
					roundf	( produto.getDiaria_it()			*	taxa_franquia_dit		*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100
				);
			}
			else {
				premio = (
					roundf	( produto.getCobertura_map()		*	taxa_map				*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
					roundf	( produto.getCobertura_ipa()		*	taxa_ipa				*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
					roundf	( produto.getDespesas_medicas()		*	taxa_despesas_medicas	*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
					roundf	( cobertura_majoracao				*	taxa_majoracao			*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100 +
					roundf	( produto.getDiaria_it()			*	taxa_franquia_dit	*	( 1 + 0.21 )		*	taxa_operacao	*	100 * ( 1.0 + produto.getIof() ) ) / 100
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
				roundf 	( produto.getCobertura_map()	*	taxa_map				*	taxa_percentual_premio 	*	taxa_operacao	*	100	*	( 1.0 + produto.getIof() ) ) / 100 +
				roundf	( produto.getCobertura_ipa()	*	taxa_ipa				*	taxa_percentual_premio	*	taxa_operacao	*	100	*	( 1.0 + produto.getIof() ) ) / 100 +
				roundf	( produto.getDespesas_medicas()	*	taxa_despesas_medicas	*	taxa_percentual_premio	*	taxa_operacao	*	100	*	( 1.0 + produto.getIof() ) ) / 100
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

/*!
 *  Recebe arquivo JSON com dados do perfil, JSON com dados do produto e JSON de saída
 */
double gravaValorPremio(const char* perfil_input_file, const char* produto_input_file, const char* output_file) {

	Perfil perfil = inicializaPerfil();
	Json::Value valor_perfil = retornaValueJSON(perfil_input_file);
	criaPerfil(valor_perfil,perfil);

	Produto produto = inicializaProduto();
	Json::Value valor_produto = retornaValueJSON(produto_input_file);
	criaProduto(valor_produto,produto);

	double premio = calculaPremio(produto,perfil);
	montaJSONPremio(premio,output_file);
	return premio;
}
