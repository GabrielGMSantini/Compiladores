#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_DE_MEMORIA 50

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
		printf("start\n");
		s=-1;
		i++;
	}
	else{
		//Se for alocacao
		if(!strcmp(comando,"ALLOC   ")){
			int m,n,k;
			m=atoi(param1);
			n=atoi(param2);
			printf("alloc %d %d\n",m,n);
			for(k=0;k<n;k++){
				s++;
				M[s]=M[m+k];
			}
			i++;
		}
		else{
			//Se for dealocacao
			if(!strcmp(comando,"DALLOC  ")){
				int m,n,k;
				m=atoi(param1);
				n=atoi(param2);
				printf("dalloc %d %d\n",m,n);
				for(k=n-1;k>=0;k--){
					M[m+k]= M[s];
					s--;
				}
				i++;
			}
			else{
				//Se for chamada
				if(!strcmp(comando,"CALL    ")){
					int p;
					s++;
					M[s]=i+1;
					p=atoi(param1);
					i = FetchLabel(p);
					printf("call %d\n",p);
				}
				else{
					//Se for retorno
					if(!strcmp(comando,"RETURN  ")){
						printf("return %d\n",s);
						i=M[s];
						s--;
					}
					else{
						//Se for HALT
						if(!strcmp(comando,"HLT     ")){
							printf("Halt\n");
							return -1;
						}
						else{
							//Se for leitura
							if(!strcmp(comando,"RD      ")){
								printf("read\n");
								s++;
								scanf("%d",&M[s]);
								i++;
							}
							else{
								//Se for escrita
								if(!strcmp(comando,"PRN     ")){
									printf("print %d\n",M[s]);
									s--;
									i++;
								}
								else{
									//Se for carregar constante
									if(!strcmp(comando,"LDC     ")){
										int k = atoi(param1);
										printf("%d",k);
										s++;
										M[s] = k;
										i++;
									}
									else{
										//Se for carregar variavel
										if(!strcmp(comando,"LDV     ")){
											int n = atoi(param1);
											s++;
											M[s] = M[n];
											i++;
										}
										else{
											//Se for adicao
											if(!strcmp(comando,"ADD     ")){
												M[s-1] = M[s-1] + M[s];
												s--;
												i++;
											}
											else{
												//Se for subtracao
												if(!strcmp(comando,"SUB     ")){
													M[s-1] = M[s-1] - M[s];
													s--;
													i++;
												}
												else{
													//Se for multiplicacao
													if(!strcmp(comando,"MULT    ")){
														M[s-1] = M[s-1] * M[s];
														s--;
														i++;
													}
													else{
														//Se for divisao
														if(!strcmp(comando,"DIVI    ")){
															M[s-1] = M[s-1] / M[s];
															s--;
															i++;
														}
														else{
															//Se for inverter sinal
															if(!strcmp(comando,"INV     ")){
																M[s] = -(M[s]);
																i++;
															}
															else{
																//Se for "e"
																if(!strcmp(comando,"AND     ")){
																	if(M[s-1] == 1 && M[s] == 1){
																		M[s-1] = 1;
																	}
																	else{
																		M[s-1] = 0;
																	}
																	s--;
																	i++;
																}
																else{
																	//Se for "ou"
																	if(!strcmp(comando,"OR      ")){
																		if(M[s-1] == 1 || M[s] == 1){
																			M[s-1] = 1;
																		}
																		else{
																			M[s-1] = 0;
																		}
																		s--;
																		i++;
																	}
																	else{
																		//Se for negacao
																		if(!strcmp(comando,"NEG     ")){
																			M[s] = 1-M[s];
																			i++;
																		}
																		else{
																			//Se for menor
																			if(!strcmp(comando,"CME     ")){
																				if(M[s-1] < M[s]){
																					M[s-1] = 1;
																				}
																				else{
																					M[s-1] = 0;
																				}
																				s--;
																				i++;
																			}
																			else{
																				//Se for maior
																				if(!strcmp(comando,"CMA     ")){
																					if(M[s-1] > M[s]){
																						M[s-1] = 1;
																					}
																					else{
																						M[s-1] = 0;
																					}
																					s--;
																					i++;
																				}
																				else{
																					//Se for igual
																					if(!strcmp(comando,"CEQ     ")){
																						if(M[s-1] == M[s]){
																							M[s-1] = 1;
																						}
																						else{
																							M[s-1] = 0;
																						}
																						s--;
																						i++;
																					}
																					else{
																						//Se for nao igual
																						if(!strcmp(comando,"CDIF    ")){
																							if(M[s-1] != M[s]){
																								M[s-1] = 1;
																							}
																							else{
																								M[s-1] = 0;
																							}
																							s--;
																							i++;
																						}
																						else{
																							//Se for menor ou igual
																							if(!strcmp(comando,"CMEQ    ")){
																								if(M[s-1] <= M[s]){
																									M[s-1] = 1;
																								}
																								else{
																									M[s-1] = 0;
																								}
																								s--;
																								i++;
																							}
																							else{
																								//Se for maior ou igual
																								if(!strcmp(comando,"CMAQ    ")){
																									if(M[s-1] >= M[s]){
																										M[s-1] = 1;
																									}
																									else{
																										M[s-1] = 0;
																									}
																									s--;
																									i++;
																								}
																								else{
																									//Se for armazenamento
																									if(!strcmp(comando,"STR     ")){
																										int n;
																										n = atoi(param1);
																										M[n] = M[s];
																										s--;
																										i++;
																									}
																									else{
																										//Se for pulo
																										if(!strcmp(comando,"JMP     ")){
																											int p;
																											p = atoi(param1);
																											i = FetchLabel(p);
																										}
																										else{
																											//Se for pulo quando false
																											if(!strcmp(comando,"JMPF    ")){
																												if(M[s] == 0){
																													int p = atoi(param1);
																													i = FetchLabel(p);
																												}
																												else{
																													i++;
																												}
																												s--;
																											}
																											else{
																												//Se nao for null
																												if(strcmp(comando,"NULL    ")){
																													return 1;
																												}
																												else{
																													i++;
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
	int t;
	do{
		printf("\n\ni=%d\ns=%d\n\n",i,s);
		t=Exec(i);
		PrintStack();
	}while(t == 0);
}
