#include "Profissao.h"

Profissao::Profissao() : id(0) {

}

Profissao::~Profissao() {

}

int Profissao::verificaGrupoDIT(int val_id) {
    for (int i = 0; i < TOTAL_PROFISSOES; ++i) {
    	if(tabela_profissoes[i][0] == val_id)
    		return tabela_profissoes[i][1];
	}
    return 0;
}
