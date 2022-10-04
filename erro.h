#include <stdio.h>
#include <stdlib.h>

void ThrowError(int errnum, int row, char* currentchar){
	switch (errnum){
		case 1:
			printf("ERRO: falta de \"=\" apos \"!\" na linha %d.\n",row);
			exit (1);
			break;
		case 2:
			printf("ERRO: caracter \"%c\" nao reconhecido na linha %d.\n", currentchar[0], row);
			exit(2);
			break;
		case 3:
        	printf("ERRO: comentario aberto sem fechamento na linha %d.",row);
        	exit (3);
        	break;
        case 4:
        	printf("ERRO: simbolo de inicio de programa esperado na linha %d",row);
        	exit(4);
        	break;
        case 5:
        	printf("ERRO: nome do programa esperado apos a declaracao de programa na linha %d",row);
        	exit(5);
        	break;
        case 6:
        	printf("ERRO: caracter \"%c\" inesperado na linha %d. O caracter esperado era ';'",currentchar[0],row);
        	exit(6);
        	break;
        case 7:
        	printf("ERRO: esperado nome identificador apos palavra reservada \"var\" na linha %d",row);
        	exit(7);
        	break;
        case 8:
        	printf("ERRO: caracter \"%c\" inesperado durante declaracao de variaveis na linha %d",currentchar[0],row);
        	exit(8);
        	break;
        case 9:
        	printf("ERRO: caracter \":\" encontrado logo apos \",\" na linha %d",row);
        	exit(9);
        	break;
        case 10:
        	printf("ERRO: variavel na linha %d ja declarada: ",row);
        	puts(currentchar);
        	exit(10);
        	break;
        case 11:
        	printf("ERRO: tipo nao reconhecido na linha %d: \"",row);
        	fputs(currentchar, stdout);
        	printf("\". Os tipos esperados sao: inteiro e booleano");
        	exit(11);
        	break;
        case 12:
        	printf("ERRO: procedimento na linha %d ja declarado: ",row);
        	puts(currentchar);
        	exit(12);
        	break;
        case 13:
        	printf("ERRO: esperado nome identificador apos palavra reservada \"procedimento\" na linha %d",row);
        	exit(13);
        	break;
        case 14:
        	printf("ERRO: esperado nome identificador apos palavra reservada \"funcao\" na linha %d",row);
        	exit(14);
        	break;
        case 15:
        	printf("ERRO: funcao na linha %d ja declarada: ",row);
        	puts(currentchar);
        	exit(15);
        	break;
        case 16:
        	printf("ERRO: caracter \"%c\" inesperado na linha %d. O caracter esperado era ':'",currentchar[0],row);
        	exit(16);
        	break;
        case 17:
        	printf("ERRO: esperado palavra reservada \"inicio\" na linha %d.\nPalavra encontrada foi: ",row);
        	puts(currentchar);
        	exit(17);
        	break;
        case 18:
        	printf("ERRO: funcao ou variavel \"");
        	fputs(currentchar,stdout);
        	printf("\" nao declarada, encontrada na linha %d",row);
        	exit(18);
        	break;
        case 19:
        	printf("ERRO: esperado caracter ')' apos expressao ou identificador em %d",row);
        	exit(19);
        	break;
        case 20:
        	printf("ERRO: palavra ");
        	fputs(currentchar, stdout);
        	printf(", encontrada na linha %d, nao e reconhecida como um termo",row);
        	exit(20);
        	break;
        case 21:
        	printf("ERRO: palavra ");
        	fputs(currentchar, stdout);
        	printf(", encontrada na linha %d, nao foi declarada como procedimento no escopo atual",row);
        	exit(21);
        	break;
        case 22:
        	printf("ERRO: procedimento ");
        	fputs(currentchar, stdout);
        	printf(", encontrado na linha %d, nao declarado anteriormente", row);
        	exit(22);
        	break;
        case 23:
        	printf("ERRO: esperada a palavra reservada \"entao\" durante comando condicional na linha %d\nA palavra encontrada foi: ",row);
        	puts(currentchar);
        	exit(23);
        	break;
        case 24:
        	printf("ERRO: esperada a palavra reservada \"faca\" durante comando de repeticao na linha %d\nA palavra encontrada foi: ",row);
        	puts(currentchar);
        	exit(24);
        	break;
        case 25:
        	printf("ERRO: \"(\" esperado apos chamada de comando na linha %d",row);
        	exit(25);
        	break;
        case 26:
        	printf("ERRO: identificador esperado apos \"(\" na linha %d",row);
        	exit(26);
        	break;
        case 27:
        	printf("ERRO: esperado \";\" apos comando nao final na linha %d",row);
        	exit(27);
        	break;
    	case 28:
    		printf("ERRO: '.' esperado na linha %d",row);
    		exit(28);
    		break;
    	case 29:
    		printf("ERRO: codigo %s encontrado apos fim do programa na linha %d",currentchar,row);
    		exit(29);
    		break;
    	case 30:
        	printf("ERRO: esperado \";\" apos fim de bloco de procedimento ou funcao na linha %d",row);
        	exit(30);
        	break;
		default:
			printf("Erro: erro desconhecido na linha %d", row);
			exit (404); 
	}
}
