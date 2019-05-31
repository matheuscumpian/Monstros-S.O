#include <stdio.h>

#include <stdlib.h>

#include <pthread.h>

FILE * fptr; //Arquivo de Entrada
FILE * rot; //Arquivo de Saída

pthread_mutex_t mutex_escrita = PTHREAD_MUTEX_INITIALIZER;
typedef struct arg {
  int primeira_coluna;
  int ultima_coluna;
  int threadx;
}
thread_args;

double ** rotacionadaaa;
double ** matriz;
int linha;
int coluna;
void armazena();

void * rotaciona(void * arg);

int main(int argc, char **argv) {

	if(argc < 6)
 	 {

    		printf("\nNumero de argumentos invalido\nTente como o exemplo a seguir:\n./giramatriz <numLinhas> <numColunas> <numThreads> <arquivoDeEntrada> <arquivoDeSaida> \n");
  		exit(1);
  
 	 }

	linha = atoi(argv[1]); //numero de linhas
	coluna = atoi(argv[2]); //numero de colunas
	rot = fopen(argv[5], "w"); //arq1 recebe o nome do arquivo .root que vai ser criado
	fptr = fopen(argv[4], "r"); //arq1 recebe o nome do arquivo .dat que vai ser lido

  
  int num_threads = atoi(argv[3]); //numero de threads
  int primeira_coluna;
  int ultima_coluna;
  int resto;
  int i = 0;
  
  if(num_threads != 2 && num_threads != 4 && num_threads != 8 && num_threads != 16)
  {  
    	printf("\nNumero de threads invalido\nTente com uma das opcoes a seguir:\n2,\t 4,\t 8,\t 16.\n");
  	exit(1);
  }
  

  rotacionadaaa = (double ** ) malloc(coluna * sizeof(double * ));
  for (i = 0; i < coluna; i++)
    rotacionadaaa[i] = (double * ) malloc(linha * sizeof(double)); //struct colunaxlinha

  matriz = (double ** ) malloc(linha * sizeof(double * ));
  for (i = 0; i < linha; i++)
    matriz[i] = (double * ) malloc(coluna * sizeof(double)); //linhaxcoluna struct

  armazena(); //roda a funcao que grava os valores do arquivo na matriz

  pthread_t tids[num_threads];

  thread_args arguments[num_threads];
  primeira_coluna = 0;
  int divisao = coluna / num_threads;
  resto = coluna % num_threads;

  arguments[0].primeira_coluna = 0;

  arguments[0].ultima_coluna = 0;

  for (int i = 0; i < num_threads; i++) //for que cria varias threadds
  {
    printf("criando thread: %d\n", i);
    if (i == 0) {
      arguments[i].primeira_coluna = 0;
      arguments[i].ultima_coluna = divisao - 1;
      printf("Primeira_Coluna Thread[%d]: %d\n", i, arguments[i].primeira_coluna);
      printf("Ultima_Coluna Thread[%d]: %d\n", i, arguments[i].ultima_coluna);
    } else if (i == num_threads - 1) {
      arguments[i].primeira_coluna = arguments[i - 1].ultima_coluna + 1;
      arguments[i].ultima_coluna = arguments[i - 1].ultima_coluna + divisao + resto;
      printf("Primeira_Coluna Thread[%d]: %d\n", i, arguments[i].primeira_coluna);
      printf("Ultima_Coluna Thread[%d]: %d\n", i, arguments[i].ultima_coluna);
    } else {

      arguments[i].primeira_coluna = arguments[i - 1].ultima_coluna + 1;
      arguments[i].ultima_coluna = arguments[i - 1].ultima_coluna + divisao;
      printf("Primeira_Coluna Thread[%d]: %d\n", i, arguments[i].primeira_coluna);
      printf("Ultima_Coluna Thread[%d]: %d\n", i, arguments[i].ultima_coluna);

    }

    pthread_create( & tids[i], NULL, rotaciona, & arguments[i]);

  }

  for (int i = 0; i < num_threads; i++) {
    pthread_join(tids[i], NULL);
    printf("Terminando thread: %d\n", i);
  }

  for (int i = 0; i < coluna; i++) //linha
  {
    for (int j = 0; j < linha; j++) //coluna
    {

      fprintf(rot, "%.2lf ", rotacionadaaa[i][j]);

    }

    fprintf(rot, "\n");
  }

  fclose(fptr);
  fclose(rot);
  return (0);
}

void armazena() //funcao que armazena na matriz os dados do arquivo
{
  if (fptr == NULL) {
    printf("erro ao abir arquivo\n");
    exit(0);
  }

  for (int i = 0; i < linha; i++) //coluna
  {
    for (int j = 0; j < coluna; j++) //linha
    {

      fscanf(fptr, "%lf", & matriz[i][j]);

    }

  }

}

void * rotaciona(void * arg) {

  struct arg * struct_pointer = (struct arg * ) arg;

  pthread_mutex_lock( & mutex_escrita);

  for (int contcol = struct_pointer->primeira_coluna; contcol <= struct_pointer->ultima_coluna; contcol++) {
    for (int contlin = 0; contlin < linha; contlin++) {

      fscanf(fptr, "%lf", & matriz[contlin][contcol]);

      int novalinha, novacoluna;
      novalinha = contcol;
      novacoluna = linha - contlin - 1;

      double aux;
      aux = matriz[contlin][contcol];

      rotacionadaaa[novalinha][novacoluna] = aux;
    }

  }

  pthread_mutex_unlock( & mutex_escrita);

  pthread_exit(0);

}
