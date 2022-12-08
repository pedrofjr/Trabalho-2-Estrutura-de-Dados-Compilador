#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//Declaração do ponteiro No de Pilha
typedef struct NoStack *PtrNoStack;

//Declaração da estrutura No de Pilha que recebe elementos do tipo char
struct NoStack{
	char key;
	PtrNoStack next;
} NoStack;

//Declaração da estrutura da Pilha Dinâmica
typedef struct{
	PtrNoStack top;
	int qty;
} DynamicStack;

//Função que inicia a pilha
void startDS(DynamicStack *stack){
	stack->top=NULL;
	stack->qty=0;
}

//Função para verificar se pilha está vazia
bool emptyDS(DynamicStack *stack){
	return (stack->top==NULL);
}

//Função para empilhar caracteres na pilha
void pushDS(DynamicStack *stack, char c){
	PtrNoStack Assist=malloc(sizeof(NoStack));
	Assist->key=c;
	Assist->next=stack->top;
	stack->top=Assist;
	stack->qty++;
}

//Função para desempilhar caracteres
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

//Função que retorna tamanho da pilha
int lenghtDS(DynamicStack *stack){
	return stack->qty;
}

//Função que retorna topo da pilha
char topDS(DynamicStack *stack){
	return stack->top->key;
}

//Função que desempilha e desaloca completamente a pilha
void deleteDS(DynamicStack *stack){
	while(!emptyDS(stack)){
		popDS(stack);
	}
}

int main(int argc, const char *argv[]){
	//Verificação do Argc
	if(argc!=3){
		printf("Quantidade de parametros invalidos: %d", argc);
		return 1;
	}
	//Abertura de arquivos de leitura e saída
	FILE *fl1, *fl2;
	fl1=fopen(argv[1], "r");
	fl2=fopen(argv[2], "w");
	if(fl1 == NULL || fl2 == NULL){
		printf("Deu B.O nos arquivos!!!\n");
		return 0;
	}
	//Declaração e início da pilha
	DynamicStack stack;
	startDS(&stack);
	//Declaração de variáveis para controle de erros
	int error=0;
	bool proceed=true;
	//String para armazenar linha
	char line[20];
	//Variável para armazenar tamanho da linha
	int last;
	//Laço de repetição para leitura da linha
	for(int i=0; fscanf(fl1, "%s", line)!=EOF; i++){
		last=strlen(line);
		//Caso haja mais de uma linha preenchida, o programa retornará erro
		if(i==1){
			error=2;
			proceed=false;
			break;
		}
		//Laço para verificar caracteres inválidos na linha
		for(int i=0; i<last; i++){
			//Retorna erro se não for um dos caracteres válidos: letras maiúsculas e operadores aritméticos
			if((line[i]<65 || line[i]>91) && (line[i]!=42 && line[i]!=43 && line[i]!=45 && line[i]!=47)){
				error=3;
				proceed=false;
			}
			//Verificar se último caractere é operador
			if(line[last-1]!=42 && line[last-1]!=43 && line[last-1]!=45 && line[last-1]!=47){
				error=4;
				proceed=false;
			}
		}
		//Caso algum caracter inválido for identificado, interromper laço
		if(!proceed) break;
	}
	//Variáveis e laço para contar número de operadores e operandos
	int operator=0, operating=0;
	for(int i=0; i<last; i++){
		if(line[i]>64 && line[i]<91){
			operating++;
		}
		else{
			operator++;
		}
	}
	//Por regra o número de operadores é o de operando menos 1
	if(operating!=(operator+1)){
		error=5;
		proceed=false;
	}
	//Caso os caracteres sejam válidos, estruturar valores na pilha
	if(proceed==true){
		//Abrir novamente arquivo de leitura para função fgetc
		fl1=fopen(argv[1], "r");
		//Variáveis auxiliares no tratamento de caracteres
		char symbol, op1, op2;
		//Strings para variável TEMP da atividade
		char temptext1[]="TEMP", temptext2[]="TEMP";
		//Variável para armazenar TEMP
		int temp=0;
		//Variável para checagem de erro relacionado a número de operandos
		int lenghtcheck=0;
		for(int i=0; i<last; i++){
			symbol=fgetc(fl1);
			//Ignorar quebras de linhas e final do arquivo
			if(symbol!='\n' && symbol!=EOF){
				//Caso seja letra, empilhar
				if(symbol>64 && symbol<91){
					pushDS(&stack, symbol);
				}
				//Caso seja operador, realizar operação
				else{
					//Caso o número de operando na pilha seja inferior a 2, retornar erro
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
					//Caso operando não for variável TEMP(um número na pilha), esvaziar string
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
					//Imprimir operação no arquivo
					switch(symbol){
						case '+': fprintf(fl2, "ADD "); break;
						case '-': fprintf(fl2, "SUB "); break;
						case '*': fprintf(fl2, "MUL "); break;
						case '/': fprintf(fl2, "DIV "); break;
					}
					fprintf(fl2, "%s%c %s%c\n", temptext1, op1, temptext2, op2);
					if(i!=last-1){
						temp++;
						//Empilhar variável TEMP(o número)
						pushDS(&stack, temp+48);
						//Imprimir variável TEMP no arquivo
						fprintf(fl2, "STR TEMP%d\n", temp);
					}
				}				
			}
		}
		//Limpar pilha
		deleteDS(&stack);
	}
	//Caso o programa falhe em alguma validação, imprimir erro no arquivo de saída
	if(proceed==false){
		fclose(fl2);
		fl2=fopen(argv[2], "w");
		fprintf(fl2,"Expressão inválida!");
	}
	//Fechar arquivos e retornar código de controle de erros
	fclose(fl1);
	fclose(fl2);
	return error;
}