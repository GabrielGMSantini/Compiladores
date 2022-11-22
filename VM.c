#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_DE_MEMORIA 200

FILE* fptr;

char P[100][26];

int M[TAMANHO_DE_MEMORIA];
int s;
int i;

//Printa a Stack inteira
int PrintStack(){
	int j = TAMANHO_DE_MEMORIA;
	for(;j>=0;j--){
		printf("%d\n",M[j]);
	}
	return 0;
}


int LoadFile(char* filename){
	if ((fptr = fopen(filename, "r")) == NULL) {
    	printf("Deu ruim!!!");
    	exit(1);
	}
	char currentchar;
	currentchar = fgetc(fptr);
	int r = 0;
	int c = 0;
	while(currentchar != EOF){
		if(currentchar == '\n'){
			P[r][c] = '\0';
			r++;
			c=0;
		}
		else{
			P[r][c] = currentchar;
			c++;
		}
		currentchar = fgetc(fptr);
	}
	return r;
}


//Procura linha com rotulo
int FetchLabel(int label){
	char rotulo [5];
	char comando [9];
	int j,k;
	k=0;
	for(j=0;j<4;j++){
		rotulo[j] = P[k][j];
	}
	rotulo[4] = '\0';
	
	for(j=0;j<8;j++){
		comando[j] = P[k][j+4];	
	}
	comando[8] = '\0';
	int flag = 0;
	
	while(strcmp(comando,"HLT     ") && atoi(rotulo) != label){
		
		k++;
		for(j=0;j<4;j++){
			rotulo[j] = P[k][j];
		}
		rotulo[4] = '\0';
		
		for(j=0;j<8;j++){
			comando[j] = P[k][j+4];	
		}
		comando[8] = '\0';
		
		if(!strcmp(comando,"HLT     ")){
			flag = 1;
		}
	}
	if(flag){
		return -1;
	}
	else{
		return k;
	}
}

//Executa o comando na linha definida
int Exec(int l){
	char rotulo [5];
	char comando [9];
	char param1 [5];
	char param2 [5];
	
	int j;
	for(j=0;j<4;j++){
		rotulo[j] = P[l][j];
		param1[j] = P[l][j+12];
		param2[j] =	P[l][j+16];
	}
	rotulo[4] = '\0';
	param1[4] = '\0';
	param2[4] = '\0';
	
	for(j=0;j<8;j++){
		comando[j] = P[l][j+4];	
	}
	comando[8] = '\0';
	//Se o comando for START
	if(!strcmp(comando,"START   ")){
		printf("start");
		s=-1;
	}
	else{
		//Se for alocacao
		if(!strcmp(comando,"ALLOC   ")){
			int m,n,k;
			m=atoi(param1);
			n=atoi(param2);
			printf("alloc %d %d",m,n);
			for(k=0;k<n;k++){
				s++;
				M[s]=M[m+k];
			}
		}
		else{
			//Se for dealocacao
			if(!strcmp(comando,"DALLOC  ")){
				int m,n,k;
				m=atoi(param1);
				n=atoi(param2);
				for(k=n-1;k>=0;k--){
					M[m+k]= M[s];
					s--;
				}
			}
			else{
				//Se for chamada
				if(!strcmp(comando,"CALL    ")){
					s++;
					M[s]=i+1;
					i=atoi(param1);
				}
				else{
					//Se for retorno
					if(!strcmp(comando,"RETURN  ")){
						i=M[s];
						s--;
					}
					else{
						//Se for HALT
						if(!strcmp(comando,"HLT     ")){
							return -1;
						}
						else{
							//Se for leitura
							if(!strcmp(comando,"RD      ")){
								s++;
								scanf("%d",M[s]);
							}
							else{
								if(!strcmp(comando,"PRN     ")){
									printf("%d",M[s]);
									s--;
								}
							}
						}
					}
				}
			}
		}
	}
	
	
	return 0;	
}

int main(int argc, char** argv){
	int r = LoadFile("./gera1.obj");
	int j=0;
	for(;j<r;j++){
		puts(P[j]);
	}
	for(j=0;j<TAMANHO_DE_MEMORIA;j++){
		M[j]=0;
	}
	Exec(0);
	Exec(1);
}
