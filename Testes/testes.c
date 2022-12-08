#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char *argv[]){
	FILE *arq1, *arq2, *arq3, *arq4;
	arq1=fopen(argv[1], "r");
	arq2=fopen(argv[2], "r");
	arq3=fopen(argv[1], "r");
	arq4=fopen(argv[2], "r");

	if(arq1 == NULL || arq2 == NULL){
		printf("Deu B.O!!!\n");
		return 0;
	}
	while(!feof(arq1)){
		char simbolo;
		printf("Expected:\n");
		while(!feof(arq2)){
			simbolo=fgetc(arq2);
			printf("%c", simbolo);
		}
		printf("\nGot:\n");
		while(!feof(arq1)){
			simbolo=fgetc(arq1);
			printf("%c", simbolo);
		}
		while(!feof(arq3)){
			if(fgetc(arq3)!=fgetc(arq4)){
				printf("\n          --->Arquivos diferentes!<---\n");
				printf("\n------------------------------------------------");
				fclose(arq1);
				fclose(arq2);
				fclose(arq3);
				fclose(arq4);
				return 0;
			}
		}
		printf("\n            --->Arquivos iguais!<---\n");
		printf("\n------------------------------------------------");
	}
	fclose(arq1);
	fclose(arq2);
	fclose(arq3);
	fclose(arq4);
	return 0;
}