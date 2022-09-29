#include <stdlib.h>
#include <stdio.h>
#include <string.h>
typedef struct identifier{
	char nome[31];
	char escopo; //Escolhido pois tem só um Byte
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
//Printa a pilha inteira
int Printstack(stacknode* topo){
	while(topo != NULL){
		printf("nome: ");
		puts(topo->identificador.nome);
		printf("escopo: %d\n",topo->identificador.escopo);
		printf("tipo: ");
		puts(topo->identificador.tipo);
		printf("memoria: %d\n",topo->identificador.memoria);
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

//Procura uma variável cujo nome bate com o nome passado
identifier* DuplicvarSearch(char* nome, stacknode* topo){
	while(topo != NULL && (strcmp(topo->identificador.nome,nome) && (strcmp(topo->identificador.tipo,"inteiro") || strcmp(topo->identificador.tipo,"booleano"))) && topo->identificador.escopo == 0){
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

//Procura um procedimento cujo nome bate com o nome passado
identifier* DuplicprocSearch(char* nome, stacknode* topo){
	while(topo != NULL && (strcmp(topo->identificador.nome,nome) && strcmp(topo->identificador.tipo,"procedimento"))){
		topo = topo->next;
		if(topo==NULL){
			return NULL;
		}
	}
	return &(topo->identificador);
}

//Procura uma funcao cujo nome bate com o nome passado
identifier* DuplicfuncSearch(char* nome, stacknode* topo){
	while(topo != NULL && (strcmp(topo->identificador.nome,nome) && (strcmp(topo->identificador.tipo,"funcao inteiro") || strcmp(topo->identificador.tipo,"funcao booleano")))){
		topo = topo->next;
		if(topo==NULL){
			return NULL;
		}
	}
	return &(topo->identificador);
}

//Coloca o tipo definido em todas as variáveis cujo tipo atual eh "variavel"
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
	printf("oi");
	while((*topo)->identificador.escopo == 0){
		printf("%d ",(*topo)->identificador.escopo);
		stacknode* temp = (*topo);
		(*topo) = (*topo)->next;
		free(temp);
	}
	return 0;
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
