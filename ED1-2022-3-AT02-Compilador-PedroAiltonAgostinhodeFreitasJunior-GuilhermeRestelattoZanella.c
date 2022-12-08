#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//Declara��o do ponteiro No de Pilha
typedef struct NoStack *PtrNoStack;

//Declara��o da estrutura No de Pilha que recebe elementos do tipo char
struct NoStack{
	char key;
	PtrNoStack next;
} NoStack;

//Declara��o da estrutura da Pilha Din�mica
typedef struct{
	PtrNoStack top;
	int qty;
} DynamicStack;

//Fun��o que inicia a pilha
void startDS(DynamicStack *stack){
	stack->top=NULL;
	stack->qty=0;
}

//Fun��o para verificar se pilha est� vazia
bool emptyDS(DynamicStack *stack){
	return (stack->top==NULL);
}

//Fun��o para empilhar caracteres na pilha
void pushDS(DynamicStack *stack, char c){
	PtrNoStack Assist=malloc(sizeof(NoStack));
	Assist->key=c;
	Assist->next=stack->top;
	stack->top=Assist;
	stack->qty++;
}

//Fun��o para desempilhar caracteres
void popDS(DynamicStack *stack){
	if(!emptyDS(stack)){
		PtrNoStack Assist;
		Assist=stack->top;
		stack->top= stack->top->next;
		free(Assist);
		stack->qty--;
	}
	else{
		printf("Erro: pilha vazia.");
	}
}

//Fun��o que retorna tamanho da pilha
int lenghtDS(DynamicStack *stack){
	return stack->qty;
}

//Fun��o que retorna topo da pilha
char topDS(DynamicStack *stack){
	return stack->top->key;
}

//Fun��o que desempilha e desaloca completamente a pilha
void deleteDS(DynamicStack *stack){
	while(!emptyDS(stack)){
		popDS(stack);
	}
}

int main(int argc, const char *argv[]){
	//Verifica��o do Argc
	if(argc!=3){
		printf("Quantidade de parametros invalidos: %d", argc);
		return 1;
	}
	//Abertura de arquivos de leitura e sa�da
	FILE *fl1, *fl2;
	fl1=fopen(argv[1], "r");
	fl2=fopen(argv[2], "w");
	if(fl1 == NULL || fl2 == NULL){
		printf("Deu B.O nos arquivos!!!\n");
		return 0;
	}
	//Declara��o e in�cio da pilha
	DynamicStack stack;
	startDS(&stack);
	//Declara��o de vari�veis para controle de erros
	int error=0;
	bool proceed=true;
	//String para armazenar linha
	char line[20];
	//Vari�vel para armazenar tamanho da linha
	int last;
	//La�o de repeti��o para leitura da linha
	for(int i=0; fscanf(fl1, "%s", line)!=EOF; i++){
		last=strlen(line);
		//Caso haja mais de uma linha preenchida, o programa retornar� erro
		if(i==1){
			error=2;
			proceed=false;
			break;
		}
		//La�o para verificar caracteres inv�lidos na linha
		for(int i=0; i<last; i++){
			//Retorna erro se n�o for um dos caracteres v�lidos: letras mai�sculas e operadores aritm�ticos
			if((line[i]<65 || line[i]>91) && (line[i]!=42 && line[i]!=43 && line[i]!=45 && line[i]!=47)){
				error=3;
				proceed=false;
			}
			//Verificar se �ltimo caractere � operador
			if(line[last-1]!=42 && line[last-1]!=43 && line[last-1]!=45 && line[last-1]!=47){
				error=4;
				proceed=false;
			}
		}
		//Caso algum caracter inv�lido for identificado, interromper la�o
		if(!proceed) break;
	}
	//Vari�veis e la�o para contar n�mero de operadores e operandos
	int operator=0, operating=0;
	for(int i=0; i<last; i++){
		if(line[i]>64 && line[i]<91){
			operating++;
		}
		else{
			operator++;
		}
	}
	//Por regra o n�mero de operadores � o de operando menos 1
	if(operating!=(operator+1)){
		error=5;
		proceed=false;
	}
	//Caso os caracteres sejam v�lidos, estruturar valores na pilha
	if(proceed==true){
		//Abrir novamente arquivo de leitura para fun��o fgetc
		fl1=fopen(argv[1], "r");
		//Vari�veis auxiliares no tratamento de caracteres
		char symbol, op1, op2;
		//Strings para vari�vel TEMP da atividade
		char temptext1[]="TEMP", temptext2[]="TEMP";
		//Vari�vel para armazenar TEMP
		int temp=0;
		//Vari�vel para checagem de erro relacionado a n�mero de operandos
		int lenghtcheck=0;
		for(int i=0; i<last; i++){
			symbol=fgetc(fl1);
			//Ignorar quebras de linhas e final do arquivo
			if(symbol!='\n' && symbol!=EOF){
				//Caso seja letra, empilhar
				if(symbol>64 && symbol<91){
					pushDS(&stack, symbol);
				}
				//Caso seja operador, realizar opera��o
				else{
					//Caso o n�mero de operando na pilha seja inferior a 2, retornar erro
					if(lenghtDS(&stack)<2){
						error=6;
						proceed=false;
						break;
					}
					//Armazenar operandos da pilha
					op1=topDS(&stack);
					popDS(&stack);
					op2=topDS(&stack);
					popDS(&stack);
					temptext1[0]='T';
					temptext2[0]='T';
					//Caso operando n�o for vari�vel TEMP(um n�mero na pilha), esvaziar string
					if(op1>64 && op1<91 && op2>64 && op2<91){
						temptext1[0]='\0';
						temptext2[0]='\0';
					}
					else if(op1>64 && op1<91){
						temptext1[0]='\0';
					}
					else if(op2>64 && op2<91){
						temptext2[0]='\0';
					}
					//Imprimir LDR no arquivo
					fprintf(fl2, "LDR %s%c\nLDR %s%c\n", temptext1, op1, temptext2, op2);
					//Imprimir opera��o no arquivo
					switch(symbol){
						case '+': fprintf(fl2, "ADD "); break;
						case '-': fprintf(fl2, "SUB "); break;
						case '*': fprintf(fl2, "MUL "); break;
						case '/': fprintf(fl2, "DIV "); break;
					}
					fprintf(fl2, "%s%c %s%c\n", temptext1, op1, temptext2, op2);
					if(i!=last-1){
						temp++;
						//Empilhar vari�vel TEMP(o n�mero)
						pushDS(&stack, temp+48);
						//Imprimir vari�vel TEMP no arquivo
						fprintf(fl2, "STR TEMP%d\n", temp);
					}
				}				
			}
		}
		//Limpar pilha
		deleteDS(&stack);
	}
	//Caso o programa falhe em alguma valida��o, imprimir erro no arquivo de sa�da
	if(proceed==false){
		fclose(fl2);
		fl2=fopen(argv[2], "w");
		fprintf(fl2,"Express�o inv�lida!");
	}
	//Fechar arquivos e retornar c�digo de controle de erros
	fclose(fl1);
	fclose(fl2);
	return error;
}