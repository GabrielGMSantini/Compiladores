#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pilha.h"
#include "erro.h"
// Estrutura que representa um no de uma lista ligada de Tokens
typedef struct token {
  char *lexema;
  char *simbolo;
} token;
//-------------------------------declaracoes-------------------------------------------------

FILE *fptr;

int currentrow = 1;

char lexvetglobal[31];

stacknode* topo;
//-------------------------------lexico-----------------------------------------------------

// retorna um token com o lexema e simbolo definidos
token* GenToken(char *lexema, char *simbolo) {
	token* T = malloc(sizeof(token));
  	T->lexema = malloc(strlen(lexema));
  	T->simbolo = malloc(strlen(simbolo));
	strcpy(T->lexema,lexema);
  	strcpy(T->simbolo,simbolo);
  	return T;
}

// Processa a detecao de um digito
token* ProcessNumber(char *currentchar) {
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
token* ProcessWord(char *currentchar) {
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
  // Dependendo do lexema, identifica uma das possÃƒÆ’Ã†â€™Ãƒâ€šÃ‚Â­veis palavras reservadas
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
  return NULL;
}

// Processa uma atribuicao
token* ProcessAttribution(char *currentchar) {
  lexvetglobal[0] = *currentchar;
  *currentchar = fgetc(fptr);
  // Caso possua um "=" apÃƒÆ’Ã†â€™Ãƒâ€šÃ‚Â³s ":", eh uma atribuicao
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
  return NULL;
}
// Processa um operador aritmetico
token* ProcessArithmeticOperator(char *currentchar) {
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
    return NULL;
  }
}

// Processa operador relacional
token* ProcessRelationalOperator(char *currentchar) {
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
      ThrowError(1,currentrow,*currentchar);
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
    return NULL;
  }
  return NULL;
}

// Processa pontuacao
token* ProcessPunctuation(char *currentchar) {
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
    return NULL;
  }
}
// Segrega o lexema atual, redirecionando para a funcao que gerara o token
token* GetToken(char *currentchar) {
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
              ThrowError(2,currentrow,*currentchar);
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
int PrintToken(token* Token){
 	printf("__________________\n\n");
    printf("Lexema:   ");
    puts(Token->lexema);
    printf("Simbolo:  ");
    puts(Token->simbolo);
    printf("\n");
    printf("__________________\n\n");
	return 0;

}

//Ignora os espaços, comentarios e quebras de linha
token* lexical(char* currentchar){
	
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
        	ThrowError(3,currentrow,*currentchar);
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
}

//---------------------------Sintatico----------------------------

int VariableAnalyzer(token** token,char* currentchar){
	//Enquanto nao for encontrado ":"
	do{
		//Se for um identificador
		if(!strcmp("sidentificador",(*token)->simbolo)){
			//-----pesquisa_duplicvar TODO
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
			 	ThrowError(8,currentrow,(*token)->lexema[0]);
			 }
		}
		//Se nao for um identificador
		else{
			ThrowError(8,currentrow,(*token)->lexema[0]);
		}
	} while(strcmp("sdoispontos",(*token)->simbolo));
	(*token) = lexical(currentchar);
	// ------------------------------------------++_______________Paramos no Analisa Tipo
	return 0;
}

//Analisa declaracao de variaveis
int VarDecAnalyzer(token** token,char* currentchar){
	//Se for uma variavel
	if(!strcmp("svar",(*token)->simbolo)){
		(*token) = 	lexical(currentchar);
		//Se tiver um identificador apos a palavra reservada "var"
		if(!strcmp("sidentificador",(*token)->simbolo)){
			while(!strcmp("sidentificador",(*token)->simbolo)){
				VariableAnalyzer(token,currentchar);
				//Se apos o tipo da variavel for encontrado um ";"
				if(!strcmp("sponto_virgula",(*token)->simbolo)){
					(*token) = lexical(currentchar);
				}
				//Se apos o tipo da variavel nao for encontrado um ";"
				else{
					ThrowError(6,currentrow,(*token)->lexema[0]);
				}
			}
		}
		
		//Se nao tiver um identificador apos a palavra reservada "var"
		else{
			ThrowError(7,currentrow,NULL);
		}
	}
	return 0;
}

//Analisa declaracao de subrotinas
int SubRoutineAnalyzer(token** token, char* currentchar){
	return 0;
}

//Analisa comandos
int CommandAnalyzer(token** token, char* currentchar){
	return 0;
}

//Analisa bloco
int BlockAnalyzer(char* currentchar){
	token* token;
	token = lexical(currentchar);
	VarDecAnalyzer(&token, currentchar);
	SubRoutineAnalyzer(&token, currentchar);
	CommandAnalyzer(&token, currentchar);
	
  	if(currentchar != EOF){
  		PrintToken(token);
	}
	return 0;
}

//Analisador sintatico
int Parser(){
	char currentchar;
  currentchar = fgetc(fptr);
  	//----int rotulo = 1;
	token* token;
  	token = lexical(&currentchar);
  	//Se comeca com a palavra reservada "programa"
  	if(!strcmp("sprograma",token->simbolo)){
	  	token = lexical(&currentchar);
	  	//Se declarar o identificador do programa
		if(!strcmp("sidentificador",token->simbolo)){
	  		Push(&topo,token->lexema,0,"nomedeprograma",0);
	  		token = lexical(&currentchar);
	  		//Se o proximo token for um ";"
	  		if(!strcmp("sponto_virgula",token->simbolo)){
				BlockAnalyzer(&currentchar);
			}
			//Se o proximo token nao for ";"
			else{
				ThrowError(6,currentrow,token->lexema[0]);
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
//----------------------------------------------------------------
int main() {
  if ((fptr = fopen("./gera1.txt", "r")) == NULL) {
    printf("Deu ruim!!!");
    exit(1);
  }
  Parser(); 
  fclose(fptr);
  return 1;
}
