#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//Estrutura que representa um no de uma lista ligada de Tokens
typedef struct tokennode {
 char* lexema;
 char* simbolo;
 struct tokennode* next;
} tokennode;
tokennode* THead = NULL;
FILE *fptr;

//Da append nos lexema e simbolo que foi reconhecido para a lista de tokens
int AppendToken(char* lexema, char* simbolo){
	tokennode* TStart = THead;
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
		THead = THead->next;
	}
	tokennode* TNew = malloc(sizeof(tokennode));
	TNew->lexema = malloc(strlen(lexema));
	TNew->simbolo = malloc(strlen(simbolo));
	strcpy(TNew->lexema,lexema);
	strcpy(TNew->simbolo,simbolo);
	TNew->next = NULL;
	THead->next = TNew;
	THead = TStart;
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
//Segrega o lexema atual, redirecionando para a funcao que gerara o token
int GetToken(char* currentchar){
	printf("%c ",*currentchar);
	//Se for digito	
	if(isdigit(*currentchar)){
		printf("EH DIGITO 0/\n");
		ProcessNumber(currentchar);
	}
	else{
		//Se for letra
		if(isalpha(*currentchar)){
			printf("EH LETRA \\0\n");
			ProcessWord(currentchar);
		}
		else{
			//Se for atribuicao
			if(*currentchar == ':'){
				printf("EH ATRIBUICAO!!!! (^_^)\n");	
			}
			else{
				//Se for operador aritmetico
				if(*currentchar == '+' || *currentchar == '-' || *currentchar == '*'){
					printf("EH OPERADOR ARITMETICO <+u+>\n");
				}
				else{
					//Se for operador relacional
					if(*currentchar == '!' || *currentchar == '<' || *currentchar == '>' || *currentchar == '='){
						printf("EH OPERADOR RELACIONAL!!!! (UwU)\n");
					}
					else{
						//Se for pontuacao
						if(*currentchar == ';' || *currentchar == ',' || *currentchar == '(' || *currentchar == ')' || *currentchar == '.'){
							printf("EH PONTUACAO!!!!!!!!!!!! [OwO]\n");
						}
						//Se nao for nada disso, deu erro
						else{
							printf("CORRE NEGADAAAAA!!!!!!!!\n");
							exit(1);
						}
					}
				}
			}
		}
	}
	
	*currentchar = fgetc(fptr);
}
//Processa a detecao de um digito
int ProcessNumber(char* currentchar){
	char* lexema;
	lexema = malloc(sizeof(char));
	int size = 1;
	lexema[0] = *currentchar;
	*currentchar = fgetc(fptr);
	while(isdigit(*currentchar)){
		size++;
		lexema = (char *) realloc(lexema,size*sizeof(char));
		if(lexema == NULL){
			exit (1);
		}
		lexema[size-1] = *currentchar;
		*currentchar = fgetc(fptr);
	}
	lexema = realloc(lexema,(size+1)*sizeof(char));
	lexema[size] = '\0';
	AppendToken(lexema,"snumero");
	free(lexema);
	return 0;
}

//Processa identificador ou palavra reservada
int ProcessWord(char* currentchar){
	char* lexema;
	lexema = malloc(sizeof(char));
	int size = 1;
	lexema[0] = *currentchar;
	*currentchar = fgetc(fptr);

	while(isalpha(*currentchar) || isdigit(*currentchar) || *currentchar == '_'){
		size ++;
		lexema = (char *) realloc(lexema,size);
		if(lexema == NULL){
			exit (1);
		}
		lexema[size-1] = *currentchar;
		*currentchar = fgetc(fptr);
	}
	lexema = realloc(lexema,size+1);
	lexema[size] = '\0';
	puts(lexema);
	AppendToken(lexema,"spalavra");
	free(lexema);
	return 0;
}
int main(){
	if((fptr = fopen("./gera1.txt","r")) == NULL){
		printf("Deu ruim!!!");
		exit(1);
	}
	char currentchar;
	currentchar = fgetc(fptr);
	//Enquanto nao terminar o arquivo fonte
	while(currentchar != EOF){
		while((currentchar == '{' || isspace(currentchar)) && currentchar != EOF){
			//Se o caracter lido for { significa que eh comeco de comentario, entao ignora ate terminar o comentario
			if(currentchar == '{'){
				while(currentchar != '}' && currentchar != EOF){
					
					currentchar = fgetc(fptr);
				}
				currentchar = fgetc(fptr);
			}
			//quando tiver espacos, ignorar eles
			while(isspace(currentchar) && currentchar != EOF){
				currentchar = fgetc(fptr);
			}
			
		}
		if(currentchar != EOF){
			//Gera token a partir da palavra atual
			GetToken(&currentchar);
		}
		
	}
	PrintTokenList(THead);
	fclose(fptr);
	return 1;
}
