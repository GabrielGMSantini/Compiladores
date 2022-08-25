#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct tokennode {
 char* lexema;
 char* simbolo;
 struct tokennode* next;
} tokennode;

int appendtoken(tokennode** TL,char* lexema, char* simbolo){
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
	//puts((*TL)->next->lexema);
	return 1;
}
int main(){
	tokennode* THead = malloc(sizeof(tokennode));
	THead->next = NULL;
	appendtoken(&THead,"oi","soi");
	puts(THead->next->lexema);
	return 1;
}
