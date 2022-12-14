#include <stdlib.h>
#include <stdio.h>
#include <string.h>
typedef struct identifier{
	char nome[31];
	char escopo; //Escolhido pois tem sÃ³ um Byte
	char tipo[31];
	int memoria;
}identifier;

typedef struct stacknode{
	identifier identificador;
	struct stacknode* next;
} stacknode;

//Empilha o identificador no topo da pilha
int Push(stacknode** topo, char* nome, char escopo, char* tipo, int memoria){
	if((*topo) == NULL){
		(*topo) = (stacknode*) malloc(sizeof(stacknode));
		strcpy((*topo)->identificador.nome, nome );
		strcpy((*topo)->identificador.tipo,tipo);
		(*topo)->identificador.escopo = escopo;
		(*topo)->identificador.memoria = memoria;
		(*topo)->next = NULL;
		return 0; 
	}
	else{
		stacknode * novotopo = (stacknode*)malloc(sizeof(stacknode));
		strcpy(novotopo->identificador.nome,nome);
		strcpy(novotopo->identificador.tipo,tipo);
		novotopo->identificador.escopo = escopo;
		novotopo->identificador.memoria = memoria;
		novotopo->next = (*topo);
		(*topo) = novotopo;
		return 0;
	}
	return 1;
}

//Desempilha o identificador do topo da pilha
identifier* Pop(stacknode** topo){
	stacknode * novotopo = (stacknode*)malloc(sizeof(stacknode));
	identifier * aux = (identifier*)malloc(sizeof(identifier));
	*aux = (*topo)->identificador;
	novotopo = (*topo)->next;
	
	*topo = novotopo;
	return aux;
}

//Printa a pilha inteira
int Printstack(stacknode* topo){
	while(topo != NULL){
		printf("nome: ");
		puts(topo->identificador.nome);
		printf("escopo: %d\n",topo->identificador.escopo);
		printf("tipo: ");
		puts(topo->identificador.tipo);
		printf("memoria: %d\n\n",topo->identificador.memoria);
		topo = topo->next;
	}
	return 0;
}

/* 	Consulta a pilha buscando por um identificador com o nome especificado
   	Uso recomendado:
   	
	identifier* check;
   	check = Consultstack("teste",topo);
   	if(check == NULL){
	 	//nao foi encontrado nenhum identificador com o nome especificado
  	}
  	else{
  		//foi encontrado um identificador com o nome especificado
  	}
  	
*/
identifier* Consultstack(char* nome, stacknode* topo){
	while(topo != NULL && strcmp(topo->identificador.nome,nome)){
		topo = topo->next;
		if(topo==NULL){
			return NULL;
		} 
		
	}
	return &(topo->identificador);
}

//Procura uma variavel cujo nome bate com o nome passado no escopo atual
identifier* DuplicvarSearch(char* nome, stacknode* topo){
	//Ve no escopo da variavel se existe outra variavel ou 
	while(topo != NULL && (strcmp(topo->identificador.nome,nome)) && topo->identificador.escopo == 0){
		topo = topo->next;
		if(topo==NULL){
			return NULL;
		}
	}
	//Se ele nao encontrou nada no escopo, deve verificar no nivel inferior
	if(topo->identificador.escopo != 0){	
		while(topo != NULL && (strcmp(topo->identificador.nome,nome))){
			topo = topo->next;
			if(topo==NULL){
				return NULL;
			}
		}
		if(topo == NULL){
			return NULL;
		}
		else{
			//Se encontrou uma variavel fora do escopo, retorna NULL para identificar que nao foi encontrada nenhuma duplicata
			if(!strcmp(topo->identificador.tipo,"inteiro") || !strcmp(topo->identificador.tipo,"booleano")){
				return NULL;
			}
			//Se for encontrado um identificador e ele nao for variavel, eh procedimento ou funcao
			else{
				return &(topo->identificador);
			}
			
		}
	}
	else{
		return &(topo->identificador);
	}
	
	return NULL;
}

//Pesquisa a tabela inteira em busca de uma variavel
identifier* DuplicvarSearchFull(char* nome, stacknode* topo){
	while(topo != NULL && (strcmp(topo->identificador.nome,nome) || (strcmp(topo->identificador.tipo,"inteiro") && strcmp(topo->identificador.tipo,"booleano")))){
		topo = topo->next;
		if(topo==NULL){
		}
	}
	return &(topo->identificador);
}

//Procura um procedimento cujo nome bate com o nome passado
identifier* DuplicprocSearch(char* nome, stacknode* topo){
	while(topo != NULL && (strcmp(topo->identificador.nome,nome) && strcmp(topo->identificador.tipo,"procedimento")) && topo->identificador.escopo == 0){
		topo = topo->next;
		if(topo==NULL){
			return NULL;
		}
	}
	if(topo->identificador.escopo != 0){
		return NULL;
	}
	return &(topo->identificador);
}

//Procura uma funcao cujo nome bate com o nome passado
identifier* DuplicfuncSearch(char* nome, stacknode* topo){
	while(topo != NULL && (strcmp(topo->identificador.nome,nome) && (strcmp(topo->identificador.tipo,"funcao inteiro") || strcmp(topo->identificador.tipo,"funcao booleano"))) && topo->identificador.escopo == 0){
		topo = topo->next;
		if(topo==NULL){
			return NULL;
		}
	}
	if(topo->identificador.escopo != 0){
        if(strcmp(topo->identificador.nome,nome))
		    return NULL;
        else
            return &(topo->identificador);  
	}
	return &(topo->identificador);
}

//Coloca o tipo definido em todas as variÃ¡veis cujo tipo atual eh "variavel"
int Identifytype(char* tipo, stacknode** topo){
	stacknode* topobuf = *topo;
	while((*topo)!= NULL){
		if(!strcmp((*topo)->identificador.tipo,"variavel")){
			strcpy((*topo)->identificador.tipo,tipo);
		}
		(*topo) = (*topo)->next;
	}
	(*topo) = topobuf;
	return 0;
}

//Quebra o galho atual
int Getoff(stacknode** topo){
	int cont = 0;
	while((*topo)->identificador.escopo == 0 && strcmp((*topo)->identificador.tipo,"nomedeprograma")){
		stacknode* temp = (*topo);
		if(temp!= NULL){
			if(!strcmp(temp->identificador.tipo,"inteiro") || !strcmp(temp->identificador.tipo,"booleano")){
				cont++;
			}
		}
		(*topo) = (*topo)->next;
		free(temp);
	}
	(*topo)->identificador.escopo = 0;
	return cont;
}

/*
Testes feitos:
	Push(&topo,"oi",0,"tipooi",1234154);
  	Push(&topo,"tchau",1,"variavel",151542);
  	Push(&topo,"teste",0,"variavel",31245);
  	identifier* check;
  	check = Consultstack("teste",topo);
  	if(check == NULL){
		//nao foi encontrado nenhum identificador com o nome especificado
  	}
  	else{
  		//foi encontrado um identificador com o nome especificado
  		puts(check->tipo);
  	}
  	Printstack(topo);
  	Identifytype("tteste",&topo);
 	Printstack(topo);	
*/
