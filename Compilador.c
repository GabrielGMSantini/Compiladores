#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura que representa um no de uma lista ligada de Tokens
typedef struct tokennode {
  char *lexema;
  char *simbolo;
  struct tokennode *next;
} tokennode;

tokennode *THead = NULL;

FILE *fptr;

char lexvetglobal[31];
// Da append nos lexema e simbolo que foi reconhecido para a lista de tokens
int AppendToken(char *lexema, char *simbolo) {
  tokennode *TStart = THead;
  if (THead == NULL) {
    THead = malloc(sizeof(tokennode));
    THead->lexema = malloc(strlen(lexema));
    THead->simbolo = malloc(strlen(simbolo));
    strcpy(THead->lexema, lexema);
    strcpy(THead->simbolo, simbolo);
    THead->next = NULL;
    return 0;
  }
  while (THead->next != NULL) {
    THead = THead->next;
  }
  tokennode *TNew = malloc(sizeof(tokennode));
  TNew->lexema = malloc(strlen(lexema));
  TNew->simbolo = malloc(strlen(simbolo));
  strcpy(TNew->lexema, lexema);
  strcpy(TNew->simbolo, simbolo);
  TNew->next = NULL;
  THead->next = TNew;
  THead = TStart;
  return 0;
}

// Printa a lista de tokens
int PrintTokenList(tokennode *THeader) {
  int index = 1;
  printf("\n\nLISTA DE TOKENS: \n");
  printf("__________________\n\n");
  while (THeader != NULL) {
    printf("Token %d\n\n", index);
    printf("Lexema:   ");
    puts(THeader->lexema);
    printf("Simbolo:  ");
    puts(THeader->simbolo);
    printf("\n");
    printf("__________________\n\n");
    index++;
    THeader = THeader->next;
  }
  return 0;
}

// Processa a detecao de um digito
int ProcessNumber(char *currentchar) {
  int size = 1;
  lexvetglobal[size - 1] = *currentchar;
  *currentchar = fgetc(fptr);
  while (isdigit(*currentchar)) {
    size++;
    lexvetglobal[size - 1] = *currentchar;
    *currentchar = fgetc(fptr);
  }
  lexvetglobal[size] = '\0';
  AppendToken(lexvetglobal, "snumero");
  return 0;
}

// Processa identificador ou palavra reservada
int ProcessWord(char *currentchar) {
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
  puts(lexvetglobal);
  // Dependendo do lexema, identifica uma das possÃ­veis palavras reservadas
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
    AppendToken(lexvetglobal, simbolo);
  }
  // Caso nÃ£o seja nenhuma das palavras reservadas, Ã© um identificador
  else {
    AppendToken(lexvetglobal, "sidentificador");
  }
  return 0;
}

// Processa uma atribuicao
int ProcessAttribution(char *currentchar) {
  lexvetglobal[0] = *currentchar;
  *currentchar = fgetc(fptr);
  // Caso possua um "=" apÃ³s ":", eh uma atribuicao
  if (*currentchar == '=') {
    lexvetglobal[1] = *currentchar;
    lexvetglobal[2] = '\0';
    *currentchar = fgetc(fptr);
    AppendToken(lexvetglobal, "satribuicao");
  }
  // Senao, eh dois pontos
  else {
    lexvetglobal[1] = '\0';
    AppendToken(lexvetglobal, "sdoispontos");
  }
  return 0;
}
// Processa um operador aritmetico
int ProcessArithmeticOperator(char *currentchar) {
  lexvetglobal[0] = *currentchar;
  lexvetglobal[1] = '\0';
  *currentchar = fgetc(fptr);
  // separa os simbolos e gera os tokens
  switch (lexvetglobal[0]) {
  case '+':
    AppendToken(lexvetglobal, "smais");
    return 0;
    break;
  case '-':
    AppendToken(lexvetglobal, "smenos");
    return 0;
    break;
  case '*':
    AppendToken(lexvetglobal, "smult");
    return 0;
    break;
  default:
    return 1;
  }
}

// Processa operador relacional
int ProcessRelationalOperator(char *currentchar) {
  lexvetglobal[0] = *currentchar;
  switch (lexvetglobal[0]) {
  case '=':
    lexvetglobal[1] = '\0';
    AppendToken(lexvetglobal, "sig");
    *currentchar = fgetc(fptr);
    return 0;
    break;
  case '!':
    *currentchar = fgetc(fptr);
    if (*currentchar == '=') {
      lexvetglobal[1] = *currentchar;
      *currentchar = fgetc(fptr);
      lexvetglobal[2] = '\0';
      AppendToken(lexvetglobal, "sdif");
      return 0;
      break;
    }
    else {
      printf("ERRO, falta de \"=\" apos \"!\"\n");
      exit(1);
    }
  case '<':
    *currentchar = fgetc(fptr);
    if (*currentchar == '=') {
      lexvetglobal[1] = *currentchar;
      lexvetglobal[2] = '\0';
      *currentchar = fgetc(fptr);
      AppendToken(lexvetglobal, "smenorig");
      return 0;
    } else {
      lexvetglobal[1] = '\0';
      AppendToken(lexvetglobal, "smenor");
      return 0;
    }
    break;
  case '>':
    *currentchar = fgetc(fptr);
    if (*currentchar == '=') {
      lexvetglobal[1] = *currentchar;
      lexvetglobal[2] = '\0';
      *currentchar = fgetc(fptr);
      AppendToken(lexvetglobal, "smaiorig");
      return 0;
    } else {
      lexvetglobal[1] = '\0';
      AppendToken(lexvetglobal, "smaior");
      return 0;
    }
    break;

  default:
    return 1;
  }
  return 1;
}

// Processa pontuacao
int ProcessPunctuation(char *currentchar) {
  lexvetglobal[0] = *currentchar;
  lexvetglobal[1] = '\0';
  *currentchar = fgetc(fptr);
  // separa os simbolos e gera os tokens
  switch (lexvetglobal[0]) {
  case '.':
    AppendToken(lexvetglobal, "sponto");
    return 0;
    break;
  case ';':
    AppendToken(lexvetglobal, "sponto_virgula");
    return 0;
    break;
  case ',':
    AppendToken(lexvetglobal, "svirgula");
    return 0;
    break;
  case '(':
    AppendToken(lexvetglobal, "sabre_parenteses");
    return 0;
    break;
  case ')':
    AppendToken(lexvetglobal, "sfecha_parenteses");
    return 0;
    break;

  default:
    return 1;
  }
}
// Segrega o lexema atual, redirecionando para a funcao que gerara o token
int GetToken(char *currentchar) {
  printf("%c ", *currentchar);
  // Se for digito
  if (isdigit(*currentchar)) {
    printf("EH DIGITO 0/\n");
    ProcessNumber(currentchar);
  } else {
    // Se for letra
    if (isalpha(*currentchar)) {
      printf("EH LETRA \\0\n");
      ProcessWord(currentchar);
    } else {
      // Se for atribuicao
      if (*currentchar == ':') {
        printf("EH ATRIBUICAO!!!! (^_^)\n");
        ProcessAttribution(currentchar);
      } else {
        // Se for operador aritmetico
        if (*currentchar == '+' || *currentchar == '-' || *currentchar == '*') {
          printf("EH OPERADOR ARITMETICO <+u+>\n");
          ProcessArithmeticOperator(currentchar);
        } else {
          // Se for operador relacional
          if (*currentchar == '!' || *currentchar == '<' ||
              *currentchar == '>' || *currentchar == '=') {
            printf("EH OPERADOR RELACIONAL!!!! (UwU)\n");
            ProcessRelationalOperator(currentchar);
          } else {
            // Se for pontuacao
            if (*currentchar == ';' || *currentchar == ',' ||
                *currentchar == '(' || *currentchar == ')' ||
                *currentchar == '.') {
              printf("EH PONTUACAO!!!!!!!!!!!! [OwO]\n");
              ProcessPunctuation(currentchar);
            }
            // Se nao for nada disso, deu erro
            else {
              printf("CORRE NEGADAAAAA!!!!!!!!\n");
              exit(1);
            }
          }
        }
      }
    }
  }
}

int main() {
  if ((fptr = fopen("./gera1.txt", "r")) == NULL) {
    printf("Deu ruim!!!");
    exit(1);
  }
  char currentchar;
  currentchar = fgetc(fptr);
  // Enquanto nao terminar o arquivo fonte
  while (currentchar != EOF) {
    while ((currentchar == '{' || isspace(currentchar)) && currentchar != EOF) {
      // Se o caracter lido for { significa que eh comeco de comentario, entao
      // ignora ate terminar o comentario
      if (currentchar == '{') {
        while (currentchar != '}' && currentchar != EOF) {

          currentchar = fgetc(fptr);
        }
        currentchar = fgetc(fptr);
      }
      // quando tiver espacos, ignorar eles
      while (isspace(currentchar) && currentchar != EOF) {
        currentchar = fgetc(fptr);
      }
    }
    if (currentchar != EOF) {
      // Gera token a partir da palavra atual
      GetToken(&currentchar);
    }
  }
  PrintTokenList(THead);
  fclose(fptr);
  return 1;
}
