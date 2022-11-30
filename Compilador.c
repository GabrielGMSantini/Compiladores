#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pilha.h"
#include "erro.h"

// Estrutura que representa um no de uma lista ligada de Tokens
typedef struct Token {
  char *lexema;
  char *simbolo;
} Token;
//-------------------------------declaracoes-------------------------------------------------
//Ponteiro para o arquivo de entrada
FILE* fptr;
//Ponteiro para o arquivo de saida
FILE* fptr2;

int rotulo = 1;

int memoria = 1;
int currentrow = 1;

char lexvetglobal[31];

stacknode* topo;

//Nome do arquivo de saida
char saidaname[31];

//Para a analise de expressao
char strings[100][100];
int stringsrow = 0;
identifier idlist[100];
int idcounter;

int Gera(char*, char* , char* , char* );
//-------------------------------lexico-----------------------------------------------------

// retorna um token com o lexema e simbolo definidos
Token* GenToken(char *lexema, char *simbolo) {
	Token* T = malloc(sizeof(Token));
  	T->lexema = malloc(strlen(lexema));
  	T->simbolo = malloc(strlen(simbolo));
	strcpy(T->lexema,lexema);
  	strcpy(T->simbolo,simbolo);
  	return T;
}

// Processa a detecao de um digito
Token* ProcessNumber(char *currentchar) {
  int size = 1;
  lexvetglobal[size - 1] = *currentchar;
  *currentchar = fgetc(fptr);
  while (isdigit(*currentchar)) {
    size++;
    lexvetglobal[size - 1] = *currentchar;
    *currentchar = fgetc(fptr);
  }
  lexvetglobal[size] = '\0';
  return (GenToken(lexvetglobal, "snumero"));
}

// Processa identificador ou palavra reservada
Token* ProcessWord(char *currentchar) {
  char simbolo[20];
  int size = 1;
  lexvetglobal[size - 1] = *currentchar;
  *currentchar = fgetc(fptr);

  while (isalpha(*currentchar) || isdigit(*currentchar) ||
         *currentchar == '_') {
    size++;
    lexvetglobal[size - 1] = *currentchar;
    *currentchar = fgetc(fptr);
  }
  lexvetglobal[size] = '\0';
  // Dependendo do lexema, identifica uma das possiveis palavras reservadas
  if (!strcmp(lexvetglobal, "programa") || !strcmp(lexvetglobal, "se") ||
      !strcmp(lexvetglobal, "entao") || !strcmp(lexvetglobal, "senao") ||
      !strcmp(lexvetglobal, "enquanto") || !strcmp(lexvetglobal, "faca") ||
      !strcmp(lexvetglobal, "inicio") || !strcmp(lexvetglobal, "fim") ||
      !strcmp(lexvetglobal, "escreva") || !strcmp(lexvetglobal, "leia") ||
      !strcmp(lexvetglobal, "var") || !strcmp(lexvetglobal, "inteiro") ||
      !strcmp(lexvetglobal, "booleano") ||
      !strcmp(lexvetglobal, "verdadeiro") || !strcmp(lexvetglobal, "falso") ||
      !strcmp(lexvetglobal, "procedimento") ||
      !strcmp(lexvetglobal, "funcao") || !strcmp(lexvetglobal, "div") ||
      !strcmp(lexvetglobal, "e") || !strcmp(lexvetglobal, "ou") ||
      !strcmp(lexvetglobal, "nao")) {
    simbolo[0] = 's';
    simbolo[1] = '\0';
    strcat(simbolo, lexvetglobal);
    return(GenToken(lexvetglobal, simbolo));
  }
  // Caso nao seja nenhuma das palavras reservadas, eh um identificador
  else {
    return(GenToken(lexvetglobal, "sidentificador"));
  }
   return(GenToken("Erro", "serro"));
}

// Processa uma atribuicao
Token* ProcessAttribution(char *currentchar) {
  lexvetglobal[0] = *currentchar;
  *currentchar = fgetc(fptr);
  // Caso possua um "=" apos um ":", eh uma atribuicao
  if (*currentchar == '=') {
    lexvetglobal[1] = *currentchar;
    lexvetglobal[2] = '\0';
    *currentchar = fgetc(fptr);
   	return(GenToken(lexvetglobal, "satribuicao"));
  }
  // Senao, eh dois pontos
  else {
    lexvetglobal[1] = '\0';
    return(GenToken(lexvetglobal, "sdoispontos"));
  }
  return(GenToken("Erro", "serro"));
}
// Processa um operador aritmetico
Token* ProcessArithmeticOperator(char *currentchar) {
  lexvetglobal[0] = *currentchar;
  lexvetglobal[1] = '\0';
  *currentchar = fgetc(fptr);
  // separa os simbolos e gera os tokens
  switch (lexvetglobal[0]) {
  case '+':
    return(GenToken(lexvetglobal, "smais"));
    break;
  case '-':
    return(GenToken(lexvetglobal, "smenos"));
    break;
  case '*':
    return(GenToken(lexvetglobal, "smult"));
    break;
  default:
    return(GenToken("Erro", "serro"));
  }
}

// Processa operador relacional
Token* ProcessRelationalOperator(char *currentchar) {
  lexvetglobal[0] = *currentchar;
  switch (lexvetglobal[0]) {
  case '=':
    lexvetglobal[1] = '\0';
    *currentchar = fgetc(fptr);
    return(GenToken(lexvetglobal, "sig"));
    break;
  case '!':
    *currentchar = fgetc(fptr);
    if (*currentchar == '=') {
      lexvetglobal[1] = *currentchar;
      *currentchar = fgetc(fptr);
      lexvetglobal[2] = '\0';
      return(GenToken(lexvetglobal, "sdif"));
      break;
    }
    else {
      ThrowError(1,currentrow,currentchar);
    }
  case '<':
    *currentchar = fgetc(fptr);
    if (*currentchar == '=') {
      lexvetglobal[1] = *currentchar;
      lexvetglobal[2] = '\0';
      *currentchar = fgetc(fptr);
      return(GenToken(lexvetglobal, "smenorig"));
    } else {
      lexvetglobal[1] = '\0';
      return(GenToken(lexvetglobal, "smenor"));
    }
    break;
  case '>':
    *currentchar = fgetc(fptr);
    if (*currentchar == '=') {
      lexvetglobal[1] = *currentchar;
      lexvetglobal[2] = '\0';
      *currentchar = fgetc(fptr);
      return(GenToken(lexvetglobal, "smaiorig"));
    } else {
      lexvetglobal[1] = '\0';
      return(GenToken(lexvetglobal, "smaior"));
    }
    break;

  default:
    return(GenToken("Erro", "serro"));
  }
  return(GenToken("Erro", "serro"));
}

// Processa pontuacao
Token* ProcessPunctuation(char *currentchar) {
  lexvetglobal[0] = *currentchar;
  lexvetglobal[1] = '\0';
  *currentchar = fgetc(fptr);
  // separa os simbolos e gera os tokens
  switch (lexvetglobal[0]) {
  case '.':
    return(GenToken(lexvetglobal, "sponto"));
    break;
  case ';':
  	return(GenToken(lexvetglobal, "sponto_virgula"));
    break;
  case ',':
  	return(GenToken(lexvetglobal, "svirgula"));
    break;
  case '(':
	return(GenToken(lexvetglobal, "sabre_parenteses"));
    break;
  case ')':
  	return(GenToken(lexvetglobal, "sfecha_parenteses"));
    break;

  default:
    return(GenToken("Erro", "serro"));
  }
}
// Segrega o lexema atual, redirecionando para a funcao que gerara o token
Token* GetToken(char *currentchar) {
  // Se for digito
  if (isdigit(*currentchar)) {
    return (ProcessNumber(currentchar));
  } else {
    // Se for letra
    if (isalpha(*currentchar)) {
      return (ProcessWord(currentchar));
    } else {
      // Se for atribuicao
      if (*currentchar == ':') {
        return(ProcessAttribution(currentchar));
      } else {
        // Se for operador aritmetico
        if (*currentchar == '+' || *currentchar == '-' || *currentchar == '*') {
          return(ProcessArithmeticOperator(currentchar));
        } else {
          // Se for operador relacional
          if (*currentchar == '!' || *currentchar == '<' ||
              *currentchar == '>' || *currentchar == '=') {
            return(ProcessRelationalOperator(currentchar));
          } else {
            // Se for pontuacao
            if (*currentchar == ';' || *currentchar == ',' ||
                *currentchar == '(' || *currentchar == ')' ||
                *currentchar == '.') {
              return(ProcessPunctuation(currentchar));
            }
            // Se nao for nada disso, deu erro
            else {
              ThrowError(2,currentrow,currentchar);
              *currentchar = fgetc(fptr);
			  //exit(1);
            }
          }
        }
      }
    }
  }
}

//Printa um token 
int PrintToken(Token* Token){
 	printf("__________________\n\n");
    printf("Lexema:   ");
    puts(Token->lexema);
    printf("Simbolo:  ");
    puts(Token->simbolo);
    printf("\n");
    printf("__________________\n\n");
	return 0;

}

//Ignora os espacos, comentarios e quebras de linha
Token* lexical(char* currentchar){
	
    while (((*currentchar) == '{' || isspace((*currentchar))) && (*currentchar)!= EOF) {
      // Se o caracter lido for { significa que eh comeco de comentario, entao
      // ignora ate terminar o comentario
      if ((*currentchar) == '{') {
      	
      	int rowbreaks = 0;
        while ((*currentchar) != '}' && (*currentchar) != EOF) {	
		  if((*currentchar) == '\n'){
			rowbreaks++;
		  }
          (*currentchar) = fgetc(fptr);
        }
        
        if((*currentchar) == EOF){
        	ThrowError(3,currentrow,currentchar);
		}
		else{
			currentrow += rowbreaks;
		   	(*currentchar) = fgetc(fptr);
		}
      }
      // quando tiver espacos, ignorar eles
      while (isspace((*currentchar)) && (*currentchar) != EOF) {
      	if((*currentchar) == '\n'){
      		currentrow++;
		  }
        (*currentchar) = fgetc(fptr);
      }
    }
    if ((*currentchar) != EOF) {
      // Gera token a partir da palavra atual
	  return(GetToken(currentchar));
    }
    else{
    	return(GenToken("erro","serro"));
	}
}

//---------------------------Sintatico----------------------------
int BlockAnalyzer(Token**, char*);
int CommandAnalyzer(Token** , char* );
//Analisa o tipo da variavel
int TypeAnalyzer(Token** token, char* currentchar){
	//Se nao for booleano ou inteiro
	if(strcmp("sinteiro",(*token)->simbolo) && strcmp("sbooleano",(*token)->simbolo)){
		ThrowError(11,currentrow,(*token)->lexema);
	}
	else{
		Identifytype((*token)->lexema,&topo);
	}
	(*token) = 	lexical(currentchar);
	return 0;
}

//Analisa a declaracao das variavel que compartilham um tipo
int VariableAnalyzer(Token** token,char* currentchar, int* cont){
	//Enquanto nao for encontrado ":"
	do{
		//Se for um identificador
		if(!strcmp("sidentificador",(*token)->simbolo)){
			//Se nao tiver duplicata
			*cont = *cont + 1;
			if(DuplicvarSearch((*token)->lexema,topo) == NULL){
				Push(&topo, (*token)->lexema,0,"variavel",memoria);
				memoria++;
				(*token) = lexical(currentchar);
				//Se apos a variavel for encontrada "," ou ":"
				if(!strcmp("svirgula",(*token)->simbolo)
				 || !strcmp("sdoispontos",(*token)->simbolo)){
				 	//Se o simbolo for virgula
				 	if(!strcmp("svirgula",(*token)->simbolo)){
				 		(*token) = lexical(currentchar);
				 		//Se apos a virgula for encontrado ':'
				 		if(!strcmp("sdoispontos",(*token)->simbolo)){
				 			ThrowError(9,currentrow,NULL);
						 }
					 }
				 }
				//Se apos a variavel nao for encontrada "," ou ":"
				else{
				 	ThrowError(8,currentrow,(*token)->lexema);
				}
			}
			else{
				ThrowError(10,currentrow,(*token)->lexema);
			}
		}
		//Se nao for um identificador
		else{
			ThrowError(8,currentrow,(*token)->lexema);
		}
	} while(strcmp("sdoispontos",(*token)->simbolo));
	(*token) = lexical(currentchar);
	TypeAnalyzer(token,currentchar);
	return 0;
}

//Analisa declaracao de variaveis
int VarDecAnalyzer(Token** token,char* currentchar){
	//Se for uma variavel
	
	if(!strcmp("svar",(*token)->simbolo)){
		int cont = 0;
		int maux = memoria;
		(*token) = 	lexical(currentchar);
		
		//Se tiver um identificador apos a palavra reservada "var"
		if(!strcmp("sidentificador",(*token)->simbolo)){
			while(!strcmp("sidentificador",(*token)->simbolo)){
				VariableAnalyzer(token,currentchar,&cont);
				//Se apos o tipo da variavel for encontrado um ";"
				if(!strcmp("sponto_virgula",(*token)->simbolo)){
					(*token) = lexical(currentchar);
				}
				//Se apos o tipo da variavel nao for encontrado um ";"
				else{
					ThrowError(6,currentrow,(*token)->lexema);
				}
			}
		}
		//Se nao tiver um identificador apos a palavra reservada "var"
		else{
			ThrowError(7,currentrow,NULL);
		}
		char aux[5] = "   \0";
		sprintf(aux,"%d",maux);
		int i=0;
		for(;i<5;i++){
			if(!isdigit(aux[i]))
				aux[i] = ' ';
		}
		char aux2[5] = "   \0";
		sprintf(aux2,"%d",cont);
		for(i=0;i<5;i++){
			if(!isdigit(aux2[i]))
				aux2[i] = ' ';
		}
		aux[4] = '\0';
		aux2[4] = '\0';
		Gera("    ","ALLOC   ",aux,aux2);
	}
	
	return 0;
}

//Analisa declaracao de procedimento
int AnalyzeProcDeclaration(Token** token, char* currentchar){
	(*token) = lexical(currentchar);
	//inicia um novo galho
	char nivel = 'L';
	//Se for encontrado um identificador
	if(!strcmp((*token)->simbolo, "sidentificador")){
		//Se o procedimento nao foi declarado antes
		if(Consultstack((*token)->lexema,topo) == NULL){
			Push(&topo,(*token)->lexema,nivel,"procedimento",rotulo);
			char aux[5] = "   \0";
			sprintf(aux,"%d",rotulo);
			int i=0;
			for(;i<5;i++){
				if(!isdigit(aux[i]))
					aux[i] = ' ';
			}
			aux[4] = '\0';
			Gera(aux,"NULL    ","    ","    ");
			rotulo++;
			(*token) = lexical(currentchar);
			//Se o proximo token for ponto e virgula
			if(!strcmp((*token)->simbolo,"sponto_virgula")){
				BlockAnalyzer(token,currentchar);
			}
			// Se nao for ;
			else{
				ThrowError(6,currentrow,(*token)->lexema);
			}
		//Se o procedimento ja foi declarado anteriormente
	}
		else{
			ThrowError(12,currentrow,(*token)->lexema);
		}
		
	}
	// Se nao for encontrado um identificador do procedimento
	else{
		ThrowError(13,currentrow,NULL);
	}
	int cont = Getoff(&topo);
	if(cont > 0){
		char aux[5] = "    \0";
		char aux2[5]= "    \0";
		memoria = memoria - cont;
		sprintf(aux,"%d",cont);
		sprintf(aux2,"%d",memoria);
		int i=0;
		for(;i<5;i++){
			if(!isdigit(aux[i]))
				aux[i] = ' ';
			if(!isdigit(aux2[i]))
				aux2[i] = ' ';
		}
		aux[4] = '\0';
		aux2[4] = '\0';
		Gera("    ","DALLOC  ",aux2,aux);
	}
	Gera("    ","RETURN  ","    ","    ");
	return 0;
}

int AnalyzeFuncDeclaration(Token** token, char* currentchar){
	(*token) = lexical(currentchar);
	//inicia um novo galho
	char nivel = 'L';
	//Se for encontrado um identificador
	if(!strcmp((*token)->simbolo, "sidentificador")){
		//Se a funcao nao foi declarada antes
		if(Consultstack((*token)->lexema,topo) == NULL){
			Push(&topo,(*token)->lexema,nivel,"",rotulo);
			char aux[5] = "   \0";
			sprintf(aux,"%d",rotulo);
			int i=0;
			for(;i<5;i++){
				if(!isdigit(aux[i]))
					aux[i] = ' ';
			}
			aux[4] = '\0';
			Gera(aux,"NULL    ","    ","    ");
			rotulo++;
			(*token) = lexical(currentchar);
			//Se for encontrado :
			if(!strcmp((*token)->simbolo,"sdoispontos")){
				(*token) = lexical(currentchar);
				//Se for inteiro ou booleano
				if(!strcmp((*token)->simbolo,"sinteiro")|| !strcmp((*token)->simbolo,"sbooleano")){
					//Se for inteiro
					if(!strcmp((*token)->simbolo,"sinteiro")){
						strcpy(topo->identificador.tipo,"funcao inteiro");
					}
					//Se nao for inteiro, eh booleano
					else{
						strcpy(topo->identificador.tipo,"funcao booleano");
					}
					(*token) = lexical(currentchar);
					//Se o proximo token for ponto e virgula
					if(!strcmp((*token)->simbolo,"sponto_virgula")){
						
						BlockAnalyzer(token,currentchar);
					}
					// Se nao for ;
					else{
						ThrowError(6,currentrow,(*token)->lexema);
					}
				}
				//Se nao for inteiro ou booleano
				else{
					ThrowError(11, currentrow,(*token)->lexema);
				}
			}
			else{
				ThrowError(16,currentrow,(*token)->lexema);
			}
			
		}
		else{
			ThrowError(15,currentrow,(*token)->lexema);
		}
	}
	// Se nao for encontrado um identificador da funcao
	else{
		ThrowError(14,currentrow,NULL);
	}
	int cont = Getoff(&topo);
	if(cont > 0){
		char aux[5] = "    \0";
		char aux2[5]= "    \0";
		memoria = memoria - cont;
		sprintf(aux,"%d",cont);
		sprintf(aux2,"%d",memoria);
		int i=0;
		for(;i<5;i++){
			if(!isdigit(aux[i]))
				aux[i] = ' ';
			if(!isdigit(aux2[i]))
				aux2[i] = ' ';
		}
		aux[4] = '\0';
		aux2[4] = '\0';
		Gera("    ","DALLOC  ",aux2,aux);
		Gera("    ","RETURN  ","    ","    ");
	}
	return 0;
}

//Analisa declaracao de subrotinas
int SubRoutineAnalyzer(Token** token, char* currentchar){
	int auxrot,flag;
	flag = 0;
	if(!strcmp((*token)->simbolo,"sprocedimento") || !strcmp((*token)->simbolo,"sfuncao")){
		auxrot = rotulo;
		char aux[5];
		sprintf(aux,"%d",rotulo);
		int i;
		for(i=0;i<5;i++){
			if(!isdigit(aux[i]))
				aux[i] = ' ';
		}
		aux[4] = '\0';
		Gera("    ","JMP     ",aux,"    ");
		rotulo++;
		flag = 1;
	}
	//Enquanto forem encontradas declaracoes de procedimentos ou funcoes
	while(!strcmp((*token)->simbolo,"sprocedimento") || !strcmp((*token)->simbolo,"sfuncao")){
		//Se for procedimento
		if(!strcmp((*token)->simbolo,"sprocedimento")){
			AnalyzeProcDeclaration(token,currentchar);
		}
		else{
			AnalyzeFuncDeclaration(token,currentchar);
		}
		//Verifica se tem ponto e virgula
		if(!strcmp((*token)->simbolo,"sponto_virgula")){
			(*token) = lexical(currentchar);
		}
		//Se o fim nao tiver ponto e virgula
		else{
			ThrowError(30,currentrow,(*token)->lexema);
		}
	}
	if(flag){
		char aux[5];
		sprintf(aux,"%d",auxrot);
		int i;
		for(i=0;i<5;i++){
			if(!isdigit(aux[i]))
				aux[i] = ' ';
		}
		aux[4] = '\0';
		Gera(aux,"NULL    ","    ","    ");
	}
	return 0;
}

//Analisa chamada de funcao
int FunctionAnalyzer(Token** token, char* currentchar, Token* identificador){
	(*token) = lexical(currentchar);
	strcpy(strings[stringsrow], (*token)->lexema);
	stringsrow++;
	return 0; 
}


//Analisa Fator
int FactorAnalyzer(Token** token, char* currentchar, Token* identificador){
	//Se for um identificador
	if(!strcmp((*token)->simbolo,"sidentificador")){
		identifier* check;
   		check = Consultstack((*token)->lexema,topo);
   		//Se encontrar um identificador de mesmo nome
		if(check != NULL){
		//Se o identificador for funcao inteiro ou booleano
		if(!strcmp(check->tipo,"funcao inteiro") || !strcmp(check->tipo,"funcao booleano")){
			FunctionAnalyzer(token,currentchar,identificador);
		}
		else{
			(*token) = lexical(currentchar);
			strcpy(strings[stringsrow], (*token)->lexema);
			stringsrow++;
		}
		
		}
		//Se nao for encontrado o identificador na tabela de simbolos
		else{
			ThrowError(18,currentrow,(*token)->lexema);
		}
	}
	//Se nao for um identificador
	else{
		//Se o token for um numero
		if(!strcmp((*token)->simbolo,"snumero")){
			(*token) = lexical(currentchar);
			strcpy(strings[stringsrow], (*token)->lexema);
			stringsrow++;
		}
		//Se nao for um numero
		else{
			//se for uma negacao
			if(!strcmp((*token)->simbolo,"snao")){
				(*token) = lexical(currentchar);
				strcpy(strings[stringsrow], (*token)->lexema);
				stringsrow++;
				FactorAnalyzer(token,currentchar,identificador);
			}
			//se nao for uma negacao
			else{
				//Se for sabre parenteses
				if(!strcmp((*token)->simbolo, "sabre_parenteses")){
					(*token) = lexical(currentchar);
					strcpy(strings[stringsrow], (*token)->lexema);
					stringsrow++;
					PhraseAnalyzer(token,currentchar,identificador);
					//Verifica se fecha parenteses
					if(!strcmp((*token)->simbolo, "sfecha_parenteses")){
						(*token) = lexical(currentchar);
						strcpy(strings[stringsrow], (*token)->lexema);
						stringsrow++;
					}
					//Se nao encontrar fecha parenteses
					else{
						ThrowError(19,currentrow,(*token)->lexema);
					}
				}
				//Se nao for sabre parenteses
				else{
					//Se for verdadeiro ou falso
					if(!strcmp((*token)->lexema,"verdadeiro") || !strcmp((*token)->lexema,"falso")){
						(*token) = lexical(currentchar);
						strcpy(strings[stringsrow], (*token)->lexema);
						stringsrow++;
					}
					//Se nao for verdadeiro nem falso
					else{
						ThrowError(20,currentrow,(*token)->lexema);
					}
				}
			}
		}
	}
	return 0;
}


//Analisa termo
int WordAnalyzer(Token** token, char* currentchar, Token* identificador){
	FactorAnalyzer(token,currentchar,identificador);
	while(!strcmp((*token)->simbolo,"smult") || !strcmp((*token)->simbolo,"sdiv") || !strcmp((*token)->simbolo,"se")){
		(*token) = lexical(currentchar);
		strcpy(strings[stringsrow], (*token)->lexema);
		stringsrow++;
		FactorAnalyzer(token,currentchar,identificador);
	}
	return 0;
}

//Analisa expressao simples
int SimplePhraseAnalyzer(Token** token, char* currentchar, Token* identificador){
	//Se for mais ou menos
	if(!strcmp((*token)->simbolo,"smais") || !strcmp((*token)->simbolo,"smenos")){
		(*token) = lexical(currentchar);
		strcpy(strings[stringsrow], (*token)->lexema);
		stringsrow++;
	}
	WordAnalyzer(token,currentchar,identificador);
	while(!strcmp((*token)->simbolo,"smais") || !strcmp((*token)->simbolo,"smenos") || !strcmp((*token)->simbolo,"sou")){
		(*token) = lexical(currentchar);
		strcpy(strings[stringsrow], (*token)->lexema);
		stringsrow++;
		WordAnalyzer(token,currentchar,identificador);
	}
	return 0;
}

//Analisa expressao
int PhraseAnalyzer(Token** token, char* currentchar, Token* identificador){
	strcpy(strings[stringsrow], (*token)->lexema);
	stringsrow++;
	SimplePhraseAnalyzer(token,currentchar,identificador);
	if(!strcmp((*token)->simbolo,"smaior") || !strcmp((*token)->simbolo,"smaiorig") || !strcmp((*token)->simbolo,"sig") || !strcmp((*token)->simbolo,"smenor") || !strcmp((*token)->simbolo,"smenorig") || !strcmp((*token)->simbolo,"sdif")){
		(*token) = lexical(currentchar);
		strcpy(strings[stringsrow], (*token)->lexema);
		stringsrow++;
		SimplePhraseAnalyzer(token,currentchar,identificador);
	}
	return 0;
}

//Analisa atribuicao
int AttAnalyzer(Token** token, char* currentchar, Token* identificador){
	int i;
	identifier* check;
	check = Consultstack(identificador->lexema,topo);
	if (check == NULL){
		ThrowError(18,currentrow,identificador->lexema);
	}
	//Ve se foi declarado
	if(strcmp(check->tipo,"procedimento") && strcmp(check->tipo,"nomedeprograma")){
	
		(*token) = lexical(currentchar);
		PhraseAnalyzer(token,currentchar,identificador);
		printf("linha : %d\n",currentrow);
		stringsrow--;
		printf("\ninfix: \n");
		for(i = 0; i < stringsrow; i++){
			puts(strings[i]);
		}
		printf("\nposfixa:\n");
		posfix();
		for(i = 0; i < idcounter; i++){
			puts(idlist[i].nome);
			puts(idlist[i].tipo);
			printf("\n");
		}
		printf("\n\n");
		int aux;
		aux = ExpressionTypeAnalyzer(); //a funcao retorna 1 se a expressao for inteira e 2 se for booleana
		if(!strcmp(check->tipo,"inteiro")|| !strcmp(check->tipo,"funcao inteiro")){
			if(aux != 1){
				ThrowError(31,currentrow,NULL);
			}
		}
		else{
			if(aux != 2){
				ThrowError(31,currentrow,NULL);
			}
		}
		idcounter = 0;
		stringsrow = 0;
		//Se o tipo comecar com f, eh funcao
		if(check->tipo[0] == 'f' && check->escopo == 'L'){
             
            if(DuplicfuncSearch(check->nome,topo)){
			    Gera("    ","STR     ","0   ","    ");
            }
		}
		//Eh variavel
		else{
			char aux[5];
			sprintf(aux,"%d",check->memoria);
			int i;
			for(i=0;i<5;i++){
				if(!isdigit(aux[i]))
					aux[i] = ' ';
			}
			aux[4] = '\0';
			Gera("    ","STR     ",aux,"    ");
		}
	}
	//Se nao foi declarado
	else{
		ThrowError(18,currentrow,identificador->lexema);
	}
	return 0;
}

//Analisa chamada de procedimento
int ProcedureAnalyzer (Token** token, char* currentchar, Token* identificador){
	//checa se o procedimento foi declarado
	identifier* check;
   	check = Consultstack(identificador->lexema,topo);
   	//Se encontrar um identificador de mesmo nome
	if(check != NULL){
		//Se o identificador for funcao inteiro ou booleano
		if(!strcmp(check->tipo,"procedimento")){
			int i;
			char aux[5] = "    \0";
			sprintf(aux,"%d",check->memoria);
			for(i=0;i<5;i++){
				if(!isdigit(aux[i]))
					aux[i] = ' ';
			}
			aux[4] = '\0';
			Gera("    ","CALL    ",aux,"    ");
		}
		else{
			ThrowError(21,currentrow,identificador->lexema);
		}
	}
	else{
		ThrowError(22,currentrow,identificador->lexema);
	}
	
	return 0; 
}

//Analisa atribuicao ou chamada de procedimento
int AnalyzeAttChProcedure(Token** token, char* currentchar){
	//Salva o identificador
	Token* tidentificador = (*token);
	(*token) = lexical(currentchar);
	//Se for atribuicao
	if(!strcmp((*token)->simbolo,"satribuicao")){
		AttAnalyzer(token,currentchar,tidentificador);
	}
	//Se nao for atribuicao, eh chamada de procedimento
	else{
		ProcedureAnalyzer(token,currentchar,tidentificador);
		
		
	}
	return 0;
}

//Analisa "se"
int Analyzeif(Token** token, char* currentchar){
	int i;
	(*token) = lexical(currentchar);
	PhraseAnalyzer(token,currentchar,NULL);
	printf("linha : %d\n",currentrow);
	stringsrow--;
	printf("\ninfix: \n");
	for(i = 0; i < stringsrow; i++){
		puts(strings[i]);
	}
	printf("\nposfixa:\n");
	posfix();
	for(i = 0; i < idcounter; i++){
		puts(idlist[i].nome);
		puts(idlist[i].tipo);
		printf("\n");
	}
	printf("\n\n");
	if(ExpressionTypeAnalyzer() != 2){
		ThrowError(31,currentrow,NULL);
	}
	idcounter = 0;
	stringsrow = 0;
	int flag = 0;
	char aux[5] = "    \0";
	char aux2[5] = "    \0";
	sprintf(aux,"%d",rotulo);
	for(i=0;i<5;i++){
		if(!isdigit(aux[i]))
			aux[i] = ' ';
	}
	aux[4] = '\0';
	Gera("    ","JMPF    ",aux,"    ");
	rotulo++;
	//Se for entao
	if(!strcmp((*token)->simbolo,"sentao")){
		(*token) = lexical(currentchar);
		AnalyzeSimpleCommand(token,currentchar);
		//Se for senao
		if(!strcmp((*token)->simbolo,"ssenao")){
			flag = 1;
			sprintf(aux2,"%d",rotulo);
			for(i=0;i<5;i++){
				if(!isdigit(aux2[i]))
					aux2[i] = ' ';
			}
			aux2[4] = '\0';
			Gera("    ","JMP     ",aux2,"    ");
			rotulo++;
			Gera(aux,"NULL    ","    ","    ");
			(*token) = lexical(currentchar);
			AnalyzeSimpleCommand(token,currentchar);
		}
	}
	//Se nao for entao
	else{
		ThrowError(23,currentrow,(*token)->lexema);
	}
	if(flag == 1){
		Gera(aux2,"NULL    ","    ","    ");
	}
	else{
		Gera(aux,"NULL    ","    ","    ");
	}
	return 0;
}

//Analisa comando "enquanto"
int Analyzewhile(Token** token, char* currentchar){
	int i;
	(*token) =lexical(currentchar);
	char aux[5] = "    \0";
	char aux2[5] = "    \0";
	sprintf(aux,"%d",rotulo);
	for(i=0;i<5;i++){
		if(!isdigit(aux[i]))
			aux[i] = ' ';
	}
	aux[4] = '\0';
	Gera(aux,"NULL    ","    ","    ");
	rotulo++;
	PhraseAnalyzer(token,currentchar,NULL);
	printf("linha : %d\n",currentrow);
	stringsrow--;
	printf("\ninfix: \n");
	for(i = 0; i < stringsrow; i++){
		puts(strings[i]);
	}
	printf("\nposfixa:\n");
	posfix();
	for(i = 0; i < idcounter; i++){
		puts(idlist[i].nome);
		puts(idlist[i].tipo);
		printf("\n");
	}
	printf("\n\n");
	if(ExpressionTypeAnalyzer() != 2){
		ThrowError(31,currentrow,NULL);
	}
	
	idcounter = 0;
	stringsrow = 0;
	
	//Se for faca
	if(!strcmp((*token)->simbolo,"sfaca")){
		sprintf(aux2,"%d",rotulo);
		for(i=0;i<5;i++){
			if(!isdigit(aux2[i]))
				aux2[i] = ' ';
		}
		aux2[4] = '\0';
		rotulo++;
		Gera("    ","JMPF    ",aux2,"    ");
		(*token) = lexical(currentchar);
		AnalyzeSimpleCommand(token,currentchar);
		Gera("    ","JMP     ",aux,"    ");
		Gera(aux2,"NULL    ","    ","    ");
	}
	else{
		ThrowError(24,currentrow,(*token)->lexema);
	}
	
}

//Analisa comando "leia"
int Analyzeread(Token** token,char* currentchar){
	(*token) = lexical(currentchar);
	//Se tiver sabre parenteses
	if(!strcmp((*token)->simbolo,"sabre_parenteses")){
		(*token) = lexical(currentchar);
		//Se for um identificador
		if(!strcmp((*token)->simbolo,"sidentificador")){
			//Se a variavel esta declarada
			identifier* check = DuplicvarSearchFull((*token)->lexema,topo);
			if(check != NULL){
				//Se for inteiro
				if(!strcmp(check->tipo,"inteiro")){
					(*token) = lexical(currentchar);
				
				//Se encontrar um fecha parenteses
				if(!strcmp((*token)->simbolo,"sfecha_parenteses")){
					char aux[5] = "    \0";
				    sprintf(aux,"%d",check->memoria);
					int i;
					for(i=0;i<5;i++){
						if(!isdigit(aux[i]))
							aux[i] = ' ';
					}
					aux[4] = '\0';
					Gera("    ","RD      ","    ","    ");
					Gera("    ","STR     ",aux,"    ");
					(*token) = lexical(currentchar);
				}
				//Se nao encontrar ")"
				else{
					ThrowError(19,currentrow,(*token)->lexema);
				}
				}
				//Se nao for inteiro
				else{
					ThrowError(31,currentrow,NULL);
				}
			}
			//Se a variavel nao foi declarada
			else{
				ThrowError(18,currentrow,(*token)->lexema);
			}
		}
		//Se nao for um identificador
		else{
			ThrowError(26,currentrow,(*token)->lexema);
		}
	}
	//Se nao encontrar (
	else{
		ThrowError(25,currentrow,(*token)->lexema);
	}
}

//Analisa comando escreva
int Analyzewrite(Token** token, char* currentchar){
	(*token) = lexical(currentchar);
	//Se tiver sabre parenteses
	if(!strcmp((*token)->simbolo,"sabre_parenteses")){
		(*token) = lexical(currentchar);
		//Se for um identificador
		if(!strcmp((*token)->simbolo,"sidentificador")){
			//Se a variavel esta declarada
			identifier* check = Consultstack((*token)->lexema,topo);
			if(check == NULL){
				ThrowError(18,currentrow,(*token)->lexema);
			}
			if(!strcmp(check->tipo,"inteiro")){
				(*token) = lexical(currentchar);
				//Se encontrar um fecha parenteses
				if(!strcmp((*token)->simbolo,"sfecha_parenteses")){
					char aux[5] = "    \0";
				    sprintf(aux,"%d",check->memoria);
					int i;
					for(i=0;i<5;i++){
						if(!isdigit(aux[i]))
							aux[i] = ' ';
					}
					aux[4] = '\0';
					Gera("    ","LDV     ",aux,"    ");
					Gera("    ","PRN     ","    ","    ");
					(*token) = lexical(currentchar);
				}
				//Se nao encontrar ")"
				else{
					ThrowError(19,currentrow,(*token)->lexema);
				}
			}
			//Se a variavel ou funcao nao foi declarada
			else{
				ThrowError(31,currentrow,(*token)->lexema);
			}
		}
		//Se nao for um identificador
		else{
			ThrowError(26,currentrow,(*token)->lexema);
		}
	}
	//Se nao encontrar (
	else{
		ThrowError(25,currentrow,(*token)->lexema);
	}
}

//Analisa comando simples
int AnalyzeSimpleCommand(Token** token, char* currentchar){
	//Se for um identificador
	
	if(!strcmp((*token)->simbolo,"sidentificador")){
		AnalyzeAttChProcedure(token,currentchar);
	}
	//Se nao for identificador
	else{
		//Se for um "se"
		if(!strcmp((*token)->simbolo,"sse")){
			Analyzeif(token,currentchar);
		}
		//Se nao for "se"
		else{
			//Se for enquanto
			if(!strcmp((*token)->simbolo,"senquanto")){
				Analyzewhile(token,currentchar);
			}
			//Se nao for enquanto
			else{
				//Se for leia
				if(!strcmp((*token)->simbolo,"sleia")){
					Analyzeread(token,currentchar);
				}
				//Se nao for leia
				else{
					//Se for escreva
					if(!strcmp((*token)->simbolo,"sescreva")){
						Analyzewrite(token,currentchar);
					}
					//Se nao for escreva
					else{
						CommandAnalyzer(token,currentchar);
					}
				}
			}
		}
	}
	return 0;
}

//Analisa comandos
int CommandAnalyzer(Token** token, char* currentchar){
	//Se for inicio de comando
	if(!strcmp((*token)->simbolo, "sinicio")){
		(*token) = lexical(currentchar);
		
		AnalyzeSimpleCommand(token,currentchar);
		//Enquanto nao encontrar fim
		while(strcmp((*token)->simbolo,"sfim")){
			//Se for ;
			if(!strcmp((*token)->simbolo,"sponto_virgula")){
				
				(*token) = lexical(currentchar);
				//Se nao for fim
				if(strcmp((*token)->simbolo,"sfim")){
					AnalyzeSimpleCommand(token,currentchar);
				}
				
			}
			//Se nao for ;
			else{
				ThrowError(27,currentrow,(*token)->lexema);
			}
		}
		(*token) = lexical(currentchar);
		
	}
	//Se nao for encontrado inicio de comando
	else{
		ThrowError(17,currentrow,(*token)->lexema);
	}
	return 0;
}

//Analisa bloco
int BlockAnalyzer(Token** token, char* currentchar){
	(*token) = lexical(currentchar);
	VarDecAnalyzer(token, currentchar);
	SubRoutineAnalyzer(token, currentchar);
	CommandAnalyzer(token, currentchar);
  	
	return 0;
}

//Analisador sintatico
int Parser(){
	char currentchar;
  currentchar = fgetc(fptr);
  	//----int rotulo = 1;
	Token* token;
  	token = lexical(&currentchar);
  	//Se comeca com a palavra reservada "programa"
  	if(!strcmp("sprograma",token->simbolo)){
	  	token = lexical(&currentchar);
	  	//Se declarar o identificador do programa
		if(!strcmp("sidentificador",token->simbolo)){
	  		Push(&topo,token->lexema,0,"nomedeprograma",0);
	  		Gera("    ","START   ","    ","    ");
	  		token = lexical(&currentchar);
	  		//Se o proximo token for um ";"
	  		if(!strcmp("sponto_virgula",token->simbolo)){
	  			Gera("    ","ALLOC   ","0   ","1    ");
				BlockAnalyzer(&token,&currentchar);
				//Se o arquivo tiver acabado sem ponto final
				if(!strcmp(token->simbolo,"serro")){
					ThrowError(28,currentrow,NULL);
				}
				//Se for ponto final
				if(!strcmp(token->simbolo,"sponto")){
					token = lexical(&currentchar);
					//Se for soh comentario ou fim do arquivo
					if(currentchar == NULL || currentchar == EOF){
						int cont = Getoff(&topo);
						printf("%d\n\n",cont);
						if(cont > 0){
							char aux[5] = "    \0";
							char aux2[5]= "    \0";
							memoria = memoria - cont;
							sprintf(aux,"%d",cont);
							sprintf(aux2,"%d",memoria);
							int i=0;
							for(;i<5;i++){
								if(!isdigit(aux[i]))
									aux[i] = ' ';
								if(!isdigit(aux2[i]))
									aux2[i] = ' ';
							}
							aux[4] = '\0';
							aux2[4] = '\0';
							Gera("    ","DALLOC  ",aux2,aux);
						}
						Gera("    ","DALLOC  ","0   ","1   ");
						Gera("    ","HLT     ","    ","    ");
						
						printf("Compilacao bem sucedida\n\n");
					}
					//Se ainda tiver codigo
					else{
						ThrowError(29,currentrow,token->lexema);
					}
				}
				//Se nao for encontrado ponto final
				else{
					ThrowError(28,currentrow,token->lexema);
				}
			}
			//Se o proximo token nao for ";"
			else{
				ThrowError(6,currentrow,token->lexema);
			}
		}
		//Se nao declarar o identificador do programa
		else{
		  	ThrowError(5,currentrow,NULL);
		}
	}
	//Se nao for a palavra reservada "programa"
	else{
		ThrowError(4,currentrow,NULL);
	}
	return 0;
}

//--------------------------Auxilio-------------------------------

//Transforma em posfixa
int posfix(){
	stacknode* aux = NULL;
	int ids = 0;
	int strcounter = 0;
	if(!strcmp(strings[0],"-") || !strcmp(strings[0],"+")){
		Push(&aux,strings[0],0,"u",0);
		strcounter++;
	}	
	for(;strcounter<stringsrow;strcounter++){
		//Se for variavel ou numero
		if(isalnum(strings[strcounter][0]) && strcmp(strings[strcounter],"div")  && strcmp(strings[strcounter],"e") && strcmp(strings[strcounter],"ou")  && strcmp(strings[strcounter],"nao")){
			if(isdigit(strings[strcounter][0])){
				idlist[ids].escopo = 0;
				idlist[ids].memoria = 0;
				strcpy(idlist[ids].nome,strings[strcounter]);
				strcpy(idlist[ids].tipo,"snumero");
				ids++;
			}
			else{
				identifier* auxid;
				auxid = Consultstack(strings[strcounter],topo);
				idlist[ids].escopo = auxid->escopo;
				idlist[ids].memoria = auxid->memoria;
				strcpy(idlist[ids].nome,strings[strcounter]);
				strcpy(idlist[ids].tipo,auxid->tipo);
				ids++;
			}
		}
		else{
			//Se for abre parenteses
			if(!strcmp(strings[strcounter],"(")){
				Push(&aux,strings[strcounter],0,"sabre_parenteses",strcounter);
				strcounter++;
				if(!strcmp(strings[strcounter],"+") || !strcmp(strings[strcounter],"-")){
					Push(&aux,strings[strcounter],0,"u",0);
				}
				else{
					strcounter--;
				}
			}
			else{
			//Se for fecha parenteses
				if(!strcmp(strings[strcounter],")")){
					identifier* auxid;
					auxid = Pop(&aux);
					
					while(strcmp(auxid->tipo,"sabre_parenteses")){
						idlist[ids] = (*auxid);
						ids++;
						auxid = Pop(&aux);
					}
				}
				//Se nao for nenhum dos acima, vai ser operador
				else{
					if(!strcmp(strings[strcounter],"*") || !strcmp(strings[strcounter],"div")){
						identifier* auxid;
						if(aux!=NULL){
							auxid = Pop(&aux);
							while(auxid!=NULL && (!strcmp(auxid->tipo,"u") || !strcmp(auxid->tipo,"smult") || !strcmp(auxid->tipo,"sdiv"))){
								idlist[ids] = (*auxid);
								ids++;
								if(aux !=NULL){
									auxid = Pop(&aux);
								}
								else{
									auxid=NULL;
								}
							}
							if(auxid){
								Push(&aux,auxid->nome,0,auxid->tipo,auxid->memoria);
							}
							if(!strcmp(strings[strcounter],"*")){
								Push(&aux,strings[strcounter],0,"smult",strcounter);
							}
							else{
								Push(&aux,strings[strcounter],0,"sdiv",strcounter);
							}
						}
						else{
							if(!strcmp(strings[strcounter],"*")){
								Push(&aux,strings[strcounter],0,"smult",strcounter);
							}
							else{
								Push(&aux,strings[strcounter],0,"sdiv",strcounter);
							}
						}
					}
					else{
						//Se for mais ou menos
						if(!strcmp(strings[strcounter],"+") || !strcmp(strings[strcounter],"-")){
							identifier* auxid;
							if(aux != NULL){
							auxid = Pop(&aux);
							while(auxid!=NULL && (!strcmp(auxid->tipo,"u") || !strcmp(auxid->tipo,"smult") || !strcmp(auxid->tipo,"sdiv") || !strcmp(auxid->tipo,"smais") || !strcmp(auxid->tipo,"smenos"))){
								idlist[ids] = (*auxid);
								ids++;
								if(aux !=NULL){
									auxid = Pop(&aux);
								}
								else{
									auxid=NULL;
								}
							}
							if(auxid){
								Push(&aux,auxid->nome,0,auxid->tipo,auxid->memoria);
							}
							if(!strcmp(strings[strcounter],"+")){
								Push(&aux,strings[strcounter],0,"smais",strcounter);
							}
							else{
								Push(&aux,strings[strcounter],0,"smenos",strcounter);
							}	
							}
							else{
								if(!strcmp(strings[strcounter],"+")){
								Push(&aux,strings[strcounter],0,"smais",strcounter);
								}
								else{
								Push(&aux,strings[strcounter],0,"smenos",strcounter);
								}
							}
						}
						else{
							//Se for relacional
							if(!strcmp(strings[strcounter],"<") || !strcmp(strings[strcounter],"<=")|| !strcmp(strings[strcounter],">")|| !strcmp(strings[strcounter],">=")|| !strcmp(strings[strcounter],"=")|| !strcmp(strings[strcounter],"!=")){
								identifier* auxid;
								if(aux!=NULL){
									auxid = Pop(&aux);
									while(auxid!=NULL && (!strcmp(auxid->tipo,"u") || !strcmp(auxid->tipo,"smult") || !strcmp(auxid->tipo,"sdiv") || !strcmp(auxid->tipo,"smais") || !strcmp(auxid->tipo,"smenos") || !strcmp(auxid->tipo,"smaior")|| !strcmp(auxid->tipo,"smaiorig")|| !strcmp(auxid->tipo,"sig")|| !strcmp(auxid->tipo,"smenor")|| !strcmp(auxid->tipo,"smenorig")|| !strcmp(auxid->tipo,"sdif"))){
										idlist[ids] = (*auxid);
										ids++;
										if(aux !=NULL){
											auxid = Pop(&aux);
										}
										else{
											auxid=NULL;
								        }
							        }
							        if(auxid){
								        Push(&aux,auxid->nome,0,auxid->tipo,auxid->memoria);
							        }
									if(!strcmp(strings[strcounter],"<")){
										Push(&aux,strings[strcounter],0,"smenor",strcounter);
									}
									else{
										if(!strcmp(strings[strcounter],"<=")){
											Push(&aux,strings[strcounter],0,"smenorig",strcounter);
										}
										else{
											if(!strcmp(strings[strcounter],">")){
												Push(&aux,strings[strcounter],0,"smaior",strcounter);
											}
											else{
												if(!strcmp(strings[strcounter],">=")){
													Push(&aux,strings[strcounter],0,"smaiorig",strcounter);
												}
												else{
													if(!strcmp(strings[strcounter],"=")){
														Push(&aux,strings[strcounter],0,"sig",strcounter);
													}
													else{
														Push(&aux,strings[strcounter],0,"sdif",strcounter);

													}
												}			
											}
										}
									}
								}
								else{
									if(!strcmp(strings[strcounter],"<")){
										Push(&aux,strings[strcounter],0,"smenor",strcounter);
									}
									else{
										if(!strcmp(strings[strcounter],"<=")){
											Push(&aux,strings[strcounter],0,"smenorig",strcounter);
										}
										else{
											if(!strcmp(strings[strcounter],">")){
												Push(&aux,strings[strcounter],0,"smaior",strcounter);
											}
											else{
												if(!strcmp(strings[strcounter],">=")){
													Push(&aux,strings[strcounter],0,"smaiorig",strcounter);
												}
												else{
													if(!strcmp(strings[strcounter],"=")){
														Push(&aux,strings[strcounter],0,"sig",strcounter);
													}
													else{
														Push(&aux,strings[strcounter],0,"sdif",strcounter);

													}
												}			
											}
										}
									}	
								}
							}
							else{
								//Se for logico
								if(!strcmp(strings[strcounter],"e") || !strcmp(strings[strcounter],"ou")|| !strcmp(strings[strcounter],"nao")){
									identifier* auxid;
									if(aux!=NULL){
										auxid = Pop(&aux);
										while(auxid!=NULL && (!strcmp(auxid->tipo,"u") || !strcmp(auxid->tipo,"smult") || !strcmp(auxid->tipo,"sdiv") || !strcmp(auxid->tipo,"smais") || !strcmp(auxid->tipo,"smenos") || !strcmp(auxid->tipo,"smaior")|| !strcmp(auxid->tipo,"smaiorig")|| !strcmp(auxid->tipo,"sig")|| !strcmp(auxid->tipo,"smenor")|| !strcmp(auxid->tipo,"smenorig")|| !strcmp(auxid->tipo,"sdif")|| !strcmp(auxid->tipo,"sou")|| !strcmp(auxid->tipo,"se")|| !strcmp(auxid->tipo,"snao"))){
											idlist[ids] = (*auxid);
											ids++;
											if(aux !=NULL){
												auxid = Pop(&aux);
											}
											else{
												auxid=NULL;
											}
										}
										if(auxid){
											Push(&aux,auxid->nome,0,auxid->tipo,auxid->memoria);
										}
										if(!strcmp(strings[strcounter],"e")){
											Push(&aux,strings[strcounter],0,"se",strcounter);
										}
										else{
											if(!strcmp(strings[strcounter],"ou")){
												Push(&aux,strings[strcounter],0,"sou",strcounter);
											}
											else{
												Push(&aux,strings[strcounter],0,"snao",strcounter);
											}
										}
									}
									else{
										if(!strcmp(strings[strcounter],"e")){
											Push(&aux,strings[strcounter],0,"se",strcounter);
										}
										else{
											if(!strcmp(strings[strcounter],"ou")){
												Push(&aux,strings[strcounter],0,"sou",strcounter);
											}
											else{
												Push(&aux,strings[strcounter],0,"snao",strcounter);
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		
	}
	identifier* auxid;
	while(aux!=NULL){
		auxid = Pop(&aux);
		idlist[ids] = (*auxid);
		ids++;
	}
	idcounter = ids;
	printf("%d\n\n",ids);
	return 0;
}

//Retorna 1 em inteiro e 2 em booleano
int ExpressionTypeAnalyzer(){
	int i;
	stacknode* aux = NULL;
	for(i=0;i<idcounter;i++){
		//Se for numero, variavel ou funcao, coloca na pilha
		if(!strcmp(idlist[i].tipo,"snumero") || !strcmp(idlist[i].tipo,"inteiro") || !strcmp(idlist[i].tipo,"booleano") || !strcmp(idlist[i].tipo,"funcao inteiro") || !strcmp(idlist[i].tipo,"funcao booleano")){
			Push(&aux,idlist[i].nome,idlist[i].escopo,idlist[i].tipo,idlist[i].memoria);
			if(strcmp(idlist[i].tipo,"snumero")){
				if(idlist[i].tipo[0] != 'f'){
					char aux[5] = "    \0";
					sprintf(aux,"%d",idlist[i].memoria);
					int i;
					for(i=0;i<5;i++){
						if(!isdigit(aux[i]))
							aux[i] = ' ';
					}
					aux[4] = '\0';
					Gera("    ","LDV     ",aux,"    ");
			    }
			    else{
			    	char aux[5] = "    \0";
					sprintf(aux,"%d",idlist[i].memoria);
					int i;
					for(i=0;i<5;i++){
						if(!isdigit(aux[i]))
							aux[i] = ' ';
					}
					aux[4] = '\0';
					Gera("    ","CALL    ",aux,"    ");
			    	Gera("    ","LDV     ","0   ","    ");
				}
			}
			else{
				char aux[5] = "    \0";
				int j;
				strcpy(aux,idlist[i].nome);
				for(j=0;j<5;j++){
					if(!isdigit(aux[j])){
						aux[j] = ' ';
					}
				}
				aux[4] = '\0';
				puts(aux);
				Gera("    ","LDC     ",aux,"    ");
			}
		}
		else{
			//Se for operador aritmetico unario
			if(!strcmp(idlist[i].tipo,"u")){
                if(!strcmp(idlist[i].nome, "-")){
				    Gera("    ","INV     ","    ","    ");
                }
				if(aux != NULL){
					identifier* auxid;
					auxid = Pop(&aux);
					if(!strcmp(auxid->tipo,"inteiro") || !strcmp(auxid->tipo,"funcao inteiro") || !strcmp(auxid->tipo,"snumero")){
						Push(&aux,auxid->nome,auxid->escopo,auxid->tipo,auxid->memoria);
					}
					else{
						ThrowError(31,currentrow,NULL);
					}
				}
				else{
					ThrowError(404,currentrow,NULL);
				}
			}
			else{
				//Se for operador aritmetico
				if(!strcmp(idlist[i].tipo,"smais") || !strcmp(idlist[i].tipo,"smenos") || !strcmp(idlist[i].tipo,"smult") || !strcmp(idlist[i].tipo,"sdiv")){
					//Eh possivel separar o tipo da operacao observando apenas a terceira letra do tipo de cada uma
					switch(idlist[i].tipo[2]){
						case 'a':
							Gera("    ","ADD     ","    ","    ");
							break;
						case 'e':
							Gera("    ","SUB     ","    ","    ");
							break;
						case 'u':
							Gera("    ","MULT    ","    ","    ");
							break;
						default:
							Gera("    ","DIVI    ","    ","    ");
							break;
					}
					if(aux!= NULL){
						identifier* auxid;
						auxid = Pop(&aux);
						if(aux != NULL){
							if(!strcmp(auxid->tipo,"inteiro") || !strcmp(auxid->tipo,"funcao inteiro") || !strcmp(auxid->tipo,"snumero")){
								auxid = Pop(&aux);
								if(!strcmp(auxid->tipo,"inteiro") || !strcmp(auxid->tipo,"funcao inteiro") || !strcmp(auxid->tipo,"snumero")){
									Push(&aux,"op",0,"inteiro",0);
								}
								else{
									ThrowError(31,currentrow,NULL);
								}
							}
							else{
								ThrowError(31,currentrow,NULL);
							}
						}
						else{
							ThrowError(32,currentrow,NULL);
						}
					}
					else{
						ThrowError(404,currentrow,NULL);
					}
				}
				else{
					//Se for operador relacional
					if(!strcmp(idlist[i].tipo,"smenor") || !strcmp(idlist[i].tipo,"smenorig")|| !strcmp(idlist[i].tipo,"smaior")|| !strcmp(idlist[i].tipo,"smaiorig")|| !strcmp(idlist[i].tipo,"sig")|| !strcmp(idlist[i].tipo,"sdif")){
						//Se for a operacao de menor
						if(!strcmp(idlist[i].tipo,"smenor")){
							Gera("    ","CME     ","    ","    ");
						}
						else{
							//Se for a operacao de menor ou igual
							if(!strcmp(idlist[i].tipo,"smenorig")){
								Gera("    ","CMEQ    ","    ","    ");
							}
							else{
								//Se for a operacao de maior
								if(!strcmp(idlist[i].tipo,"smaior")){
									Gera("    ","CMA     ","    ","    ");
								}
								else{
									//Se for a operacao de maior ou igual
									if(!strcmp(idlist[i].tipo,"smaiorig")){
										Gera("    ","CMAQ    ","    ","    ");
									}
									else{
										//Se for a operacao de igual
										if(!strcmp(idlist[i].tipo,"sig")){
											Gera("    ","CEQ     ","    ","    ");
										}
										else{
											//Se nao for nenhum dos anteriores, eh a operacao de diferente
											Gera("    ","CDIF    ","    ","    ");
										}
									}
								}
							}
						}
						if(aux!= NULL){
							identifier* auxid;
							auxid = Pop(&aux);
							if(aux != NULL){
								if(!strcmp(auxid->tipo,"inteiro") || !strcmp(auxid->tipo,"funcao inteiro") || !strcmp(auxid->tipo,"snumero")){
									auxid = Pop(&aux);
									if(!strcmp(auxid->tipo,"inteiro") || !strcmp(auxid->tipo,"funcao inteiro") || !strcmp(auxid->tipo,"snumero")){
										Push(&aux,"op",0,"booleano",0);
									}
									else{
										ThrowError(31,currentrow,NULL);
									}
								}
								else{
									ThrowError(31,currentrow,NULL);
								}
							}
							else{
								ThrowError(32,currentrow,NULL);
							}
						}
						else{
							ThrowError(404,currentrow,NULL);
						}
					}
					else{
						//se for nao
						if(!strcmp(idlist[i].tipo,"snao")){
							Gera("    ","NEG     ","    ","    ");
							if(aux != NULL){
								identifier* auxid;
								auxid = Pop(&aux);
								if(!strcmp(auxid->tipo,"booleano") || !strcmp(auxid->tipo,"funcao booleano")){
									Push(&aux,auxid->nome,auxid->escopo,auxid->tipo,auxid->memoria);
								}	
								else{
									ThrowError(31,currentrow,NULL);
								}
							}
							else{
								ThrowError(404,currentrow,NULL);
							}
						}
						else{
							//Se nao for nenhum dos anteriores, eh "e" ou "ou"
							if(!strcmp(idlist[i].tipo,"se")){
								Gera("    ","AND     ","    ","    ");
							}
							else{
								Gera("    ","OR      ","    ","    ");
							}
							if(aux!= NULL){
								identifier* auxid;
								auxid = Pop(&aux);
								if(aux != NULL){
									if(!strcmp(auxid->tipo,"booleano") || !strcmp(auxid->tipo,"funcao booleano")){
										auxid = Pop(&aux);
										if(!strcmp(auxid->tipo,"booleano") || !strcmp(auxid->tipo,"funcao booleano")){
											Push(&aux,"op",0,"booleano",0);
										}
										else{
											ThrowError(31,currentrow,NULL);
										}
									}
									else{
										ThrowError(31,currentrow,NULL);
									}
								}
								else{
									ThrowError(32,currentrow,NULL);
								}
							}
							else{
								ThrowError(404,currentrow,NULL);
							}
						}
					}
				}
			}
		}
	}
	if(aux != NULL){
		identifier* auxid;
		auxid = Pop(&aux);
		if(!strcmp(auxid->tipo,"inteiro") || !strcmp(auxid->tipo,"funcao inteiro") || !strcmp(auxid->tipo,"snumero")){
			return 1;
		}
		else{
			return 2;
		}
	}

return 0;
}

int Gera(char* rotulo, char* comando, char* par1, char* par2){
	fprintf(fptr2, rotulo);
	fprintf(fptr2, comando);
	fprintf(fptr2, par1);
	fprintf(fptr2, par2);
	fprintf(fptr2,"\n");
	return 0;
}

//----------------------------------------------------------------
int main(int argc,char ** argv) {
  if ((fptr = fopen(argv[1], "r")) == NULL) {
    printf("Deu ruim!!!");
    exit(1);
  }
  strcpy(saidaname,argv[2]);
  strcat(saidaname,".obj");
  
  if ((fptr2 = fopen(saidaname, "w")) == NULL) {
    printf("Deu ruim!!!");
    exit(1);
  }
  FILE* flog;
  if ((flog = fopen("log.txt", "w")) == NULL) {
    printf("Deu ruim!!!");
    exit(1);
  }
  Parser(); 
  fprintf(flog,"Compilacao bem sucedida\n");
  fclose(fptr);
  fclose(fptr2);
  fclose(flog);
  return 0;
}
