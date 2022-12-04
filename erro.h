#include <stdio.h>
#include <stdlib.h>

void ThrowError(int errnum, int row, char* currentchar){
    FILE* flog;
    if ((flog = fopen("log.txt", "w")) == NULL) {
        printf("Deu ruim!!!");
        exit(1);
    }
	switch (errnum){
		case 1:
			fprintf(flog,"ERRO: falta de \"=\" apos \"!\" na linha %d.\n",row);
			break;
		case 2:
			fprintf(flog,"ERRO: caracter \"%c\" nao reconhecido na linha %d.\n", currentchar[0], row);
			
			break;
		case 3:
        	fprintf(flog,"ERRO: comentario aberto sem fechamento na linha %d.",row);
        	
        	break;
        case 4:
        	fprintf(flog,"ERRO: simbolo de inicio de programa esperado na linha %d",row);
        	
        	break;
        case 5:
        	fprintf(flog,"ERRO: nome do programa esperado apos a declaracao de programa na linha %d",row);
        	
        	break;
        case 6:
        	fprintf(flog,"ERRO: caracter \"%c\" inesperado na linha %d. O caracter esperado era ';'",currentchar[0],row);

        	break;
        case 7:
        	fprintf(flog,"ERRO: esperado nome identificador apos palavra reservada \"var\" na linha %d",row);
        	
        	break;
        case 8:
        	fprintf(flog,"ERRO: caracter \"%c\" inesperado durante declaracao de variaveis na linha %d",currentchar[0],row);
        	
        	break;
        case 9:
        	fprintf(flog,"ERRO: caracter \":\" encontrado logo apos \",\" na linha %d",row);
        	
        	break;
        case 10:
        	fprintf(flog,"ERRO: variavel na linha %d ja declarada: ",row);
        	fputs(currentchar,flog);
        	
        	break;
        case 11:
        	fprintf(flog,"ERRO: tipo nao reconhecido na linha %d: \"",row);
        	fputs(currentchar, flog);
        	fprintf(flog,"\". Os tipos esperados sao: inteiro e booleano");
        	
        	break;
        case 12:
        	fprintf(flog,"ERRO: procedimento na linha %d ja declarado: ",row);
        	fputs(currentchar,flog);
        	
        	break;
        case 13:
        	fprintf(flog,"ERRO: esperado nome identificador apos palavra reservada \"procedimento\" na linha %d",row);
        	
        	break;
        case 14:
        	fprintf(flog,"ERRO: esperado nome identificador apos palavra reservada \"funcao\" na linha %d",row);
        	
        	break;
        case 15:
        	fprintf(flog,"ERRO: funcao na linha %d ja declarada: ",row);
        	fputs(currentchar,flog);
        	
        	break;
        case 16:
        	fprintf(flog,"ERRO: caracter \"%c\" inesperado na linha %d. O caracter esperado era ':'",currentchar[0],row);
        	
        	break;
        case 17:
        	fprintf(flog,"ERRO: esperado palavra reservada \"inicio\" na linha %d.\nPalavra encontrada foi: ",row);
        	fputs(currentchar,flog);
        	
        	break;
        case 18:
        	fprintf(flog,"ERRO: funcao ou variavel \"");
        	fputs(currentchar,flog);
        	fprintf(flog,"\" nao declarada, encontrada na linha %d",row);
        	
        	break;
        case 19:
        	fprintf(flog,"ERRO: esperado caracter ')' apos expressao ou identificador em %d",row);
        	
        	break;
        case 20:
        	fprintf(flog,"ERRO: palavra ");
        	fputs(currentchar, flog);
        	fprintf(flog,", encontrada na linha %d, nao e reconhecida como um termo",row);
        	
        	break;
        case 21:
        	fprintf(flog,"ERRO: palavra ");
        	fputs(currentchar, flog);
        	fprintf(flog,", encontrada na linha %d, nao foi declarada como procedimento no escopo atual",row);
        	
        	break;
        case 22:
        	fprintf(flog,"ERRO: procedimento ");
        	fputs(currentchar, flog);
        	fprintf(flog,", encontrado na linha %d, nao declarado anteriormente", row);
        	
        	break;
        case 23:
        	fprintf(flog,"ERRO: esperada a palavra reservada \"entao\" durante comando condicional na linha %d\nA palavra encontrada foi: ",row);
        	fputs( currentchar,flog);
        	
        	break;
        case 24:
        	fprintf(flog,"ERRO: esperada a palavra reservada \"faca\" durante comando de repeticao na linha %d\nA palavra encontrada foi: ",row);
        	fputs(currentchar, flog);
        	
        	break;
        case 25:
        	fprintf(flog,"ERRO: \"(\" esperado apos chamada de comando na linha %d",row);
        	
        	break;
        case 26:
        	fprintf(flog,"ERRO: identificador esperado apos \"(\" na linha %d",row);
        	
        	break;
        case 27:
        	fprintf(flog,"ERRO: esperado \";\" apos comando nao final na linha %d",row);
        	
        	break;
    	case 28:
    		fprintf(flog,"ERRO: '.' esperado na linha %d",row);
    		
    		break;
    	case 29:
    		fprintf(flog,"ERRO: codigo %s encontrado apos fim do programa na linha %d",currentchar,row);
    		
    		break;
    	case 30:
        	fprintf(flog,"ERRO: esperado \";\" apos fim de bloco de procedimento ou funcao na linha %d",row);
        	
        	break;
        case 31:
        	fprintf(flog,"ERRO: tipos incompativeis na linha %d",row);
        	
        	break;
        case 32:
        	fprintf(flog,"ERRO: utilizacao de operador nao unario para apenas um operando na linha %d",row);
        	
        	break;
		default:
			fprintf(flog,"Erro: erro desconhecido na linha %d", row);
			
	}
    exit(errnum);
}
