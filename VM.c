#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


FILE* fptr;

char program[100][26];

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
			program[r][c] = '\0';
			r++;
			c=0;
		}
		else{
			program[r][c] = currentchar;
			c++;
		}
		currentchar = fgetc(fptr);
	}
	return r;
}

int main(int argc, char** argv){
	int r = LoadFile("./t.obj");
	int i=0;
	for(;i<r;i++){
		int j;
		for(j=0;program[i][j]!='\0';j++){
			printf("%c",program[i][j]);
		}
		printf("\n");
	}	
	
}
