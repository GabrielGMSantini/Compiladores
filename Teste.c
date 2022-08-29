#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Estrutura que representa um nó de uma lista ligada de Tokens
typedef struct tokennode {
 char* lexema;
 char* simbolo;
 struct tokennode* next;
} tokennode;
tokennode* THead = NULL;

//Dá append nos lexema e simbolo que foi reconhecido para a lista de tokens
int appendtoken(tokennode** TL,char* lexema, char* simbolo){
	if((*TL) == NULL){
		(*TL) = malloc(sizeof(tokennode));
		(*TL)->lexema = malloc(strlen(lexema));
		(*TL)->simbolo = malloc(strlen(simbolo));
		strcpy((*TL)->lexema,lexema);
		strcpy((*TL)->simbolo,simbolo);
		(*TL)->next = NULL;
		return 1;
	}
	while ((*TL)->next != NULL){
		(*TL)= (*TL)->next;
	}
	tokennode* TNew = malloc(sizeof(tokennode));
	TNew->lexema = malloc(strlen(lexema));
	TNew->simbolo = malloc(strlen(simbolo));
	strcpy(TNew->lexema,lexema);
	strcpy(TNew->simbolo,simbolo);
	TNew->next = NULL;
	(*TL)->next = TNew;
	return 1;
}
//Printa a lista de tokens
void PrintTokenList(tokennode* THeader){
	while(THeader != NULL){
		puts(THeader->lexema);
		puts(THeader->simbolo);
		printf("\n");
		THeader = THeader->next;
	}
}

int main(){
	appendtoken(&THead,"oi","soi");
	appendtoken(&THead,"tchau","stchau");
	PrintTokenList(THead);
	FILE *fptr;
	if((fptr = fopen("./gera1.txt","r")) == NULL){
		printf("Deu ruim!!!");
		exit(1);
	}
	char currentchar;
	while(currentchar != EOF){
		currentchar = fgetc(fptr);
		printf("%c",currentchar);
	}
	fclose(fptr);
	return 1;
}
