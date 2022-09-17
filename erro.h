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
		default:
			printf("Erro: erro desconhecido na linha %d", row);
			exit (404); 
	}
}
