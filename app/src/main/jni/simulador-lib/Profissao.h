#ifndef PROFISSAO_H_
#define PROFISSAO_H_

#include "Tabela.h"

class Profissao {
public:
	Profissao();
	virtual ~Profissao();

    //Rules
    int verificaGrupoDIT(int val_id);

    //Getters and setters
    int getId() { return id; }
    void setId(int val) { id = val; }

protected:

private:
    int id;
};

#endif /* PROFISSAO_H_ */
