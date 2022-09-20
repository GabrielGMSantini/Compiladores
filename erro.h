#include <stdio.h>
#include <stdlib.h>

void ThrowError(int errnum, int row, char currentchar){
	switch (errnum){
		case 1:
			printf("ERRO: falta de \"=\" apos \"!\" na linha %d.\n",row);
			exit (1);
			break;
		case 2:
			printf("ERRO: caracter \"%c\" nao reconhecido na linha %d.\n", currentchar, row);
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
        	printf("ERRO: caracter \"%c\" inesperado na linha %d. O caracter esperado era ';'",currentchar,row);
        	exit(6);
        	break;
        case 7:
        	printf("ERRO: esperado nome identificador apos palavra reservada \"var\" na linha %d",row);
        	exit(7);
        	break;
        case 8:
        	printf("ERRO: caracter \"%c\" inesperado durante declaracao de variaveis na linha %d",currentchar,row);
        	exit(8);
        	break;
        case 9:
        	printf("ERRO: caracter \":\" encontrado logo apos \",\" na linha %d",row);
        	exit(9);
        	break;
		default:
			printf("Erro: erro desconhecido na linha %d", row);
			exit (404); 
	}
}
