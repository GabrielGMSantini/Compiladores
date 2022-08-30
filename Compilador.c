#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//Estrutura que representa um nó de uma lista ligada de Tokens
typedef struct tokennode {
 char* lexema;
 char* simbolo;
 struct tokennode* next;
} tokennode;
tokennode* THead = NULL;
FILE *fptr;

//Dá append nos lexema e simbolo que foi reconhecido para a lista de tokens
int appendtoken(tokennode** TL,char* lexema, char* simbolo){
	if(THead == NULL){
		THead = malloc(sizeof(tokennode));
		THead->lexema = malloc(strlen(lexema));
		THead->simbolo = malloc(strlen(simbolo));
		strcpy(THead->lexema,lexema);
		strcpy(THead->simbolo,simbolo);
		THead->next = NULL;
		return 1;
	}
	while (THead->next != NULL){
		THead = (*TL)->next;
	}
	tokennode* TNew = malloc(sizeof(tokennode));
	TNew->lexema = malloc(strlen(lexema));
	TNew->simbolo = malloc(strlen(simbolo));
	strcpy(TNew->lexema,lexema);
	strcpy(TNew->simbolo,simbolo);
	TNew->next = NULL;
	THead->next = TNew;
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

int GetToken(char* currentchar){
	//se for digito	
	if(isdigit(*currentchar)){
		printf("DIGITO 0/\n");
	}
	else{
		//se for letra
		if(isalpha(*currentchar)){
			printf("LETRA \0\n");
		}
		else{
			//Se for atribuição
			if(*currentchar == ':'){
				printf("EH ATRIBUICAO!!!! (^_^)\n");	
			}
			else{
				//Se for operador aritmético
				if(*currentchar == '+' || *currentchar == '-' || *currentchar == '*'){
					printf("EH OPERADOR ARITMETICO <+u+>\n");
				}
				else{
					//Se for operador relacional
					if(*currentchar == '!' || *currentchar == '<' || *currentchar == '>' || *currentchar == '='){
						printf("EH OPERADOR RELACIONAL!!!! (UwU)\n");
					}
					else{
						//Se for pontuação
						if(*currentchar == ';' || *currentchar == ',' || *currentchar == '(' || *currentchar == ')' || *currentchar == '.'){
							printf("EH PONTUACAO!!!!!!!!!!!! [OwO]\n");
						}
						//Se não for nada disso, deu erro
						else{
							printf("CORRE NEGADAAAAA!!!!!!!!\n");
							exit(1);
						}
					}
				}
			}
		}
	}
	
	
	printf("%c",*currentchar);
	*currentchar = fgetc(fptr);
}

int main(){
	appendtoken(&THead,"oi","soi");
	appendtoken(&THead,"tchau","stchau");
	PrintTokenList(THead);
	
	if((fptr = fopen("./gera1.txt","r")) == NULL){
		printf("Deu ruim!!!");
		exit(1);
	}
	char currentchar;
	currentchar = fgetc(fptr);
	//Enquanto não terminar o arquivo fonte
	while(currentchar != EOF){
		while((currentchar == '{' || isspace(currentchar)) && currentchar != EOF){
			//Se o caracter lido for { significa que é começo de comentário, então ignora até terminar o comentário
			if(currentchar == '{'){
				while(currentchar != '}' && currentchar != EOF){
					
					currentchar = fgetc(fptr);
				}
				currentchar = fgetc(fptr);
			}
			//quando tiver espaços, ignorar eles
			while(isspace(currentchar) && currentchar != EOF){
				currentchar = fgetc(fptr);
			}
			
		}
		if(currentchar != EOF){
			//Gera token a partir da palavra atual
			GetToken(&currentchar);
		}
		
	}
	fclose(fptr);
	return 1;
}
