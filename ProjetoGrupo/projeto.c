#include <stdio.h>

#include <stdlib.h>

#include <pthread.h>

FILE * fptr; //Arquivo de Entrada
FILE * rot; //Arquivo de Saída

pthread_mutex_t mutex_leitura = PTHREAD_MUTEX_INITIALIZER;
typedef struct arg {
   //double **matriz;
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

int main(int argc, char * argv[]) {

   linha = atoi(argv[1]); //numero de linhas
   coluna = atoi(argv[2]); //numero de colunas
   rot = fopen(argv[5], "w"); //arq1 recebe o nome do arquivo .root que vai ser criado
   fptr = fopen(argv[4], "r"); //arq1 recebe o nome do arquivo .dat que vai ser lido
   int num_threads = atoi(argv[3]); //numero de threads
   int primeira_coluna;
   int ultima_coluna;
   int resto;

   int i = 0;
   printf("Chegou aqui\n");
   rotacionadaaa = (double ** ) malloc(coluna * sizeof(double * ));
   for (i = 0; i < coluna; i++)
      rotacionadaaa[i] = (double* ) malloc(linha * sizeof(double)); //struct colunaxlinha
	printf("Chegou aqui\n");
   matriz = (double** ) malloc(linha * sizeof(double * ));
   for (i = 0; i < linha; i++)
      matriz[i] = (double * ) malloc(coluna * sizeof(double)); //linhaxcoluna struct
	printf("Chegou aqui\n");
   armazena(); //roda a funcao que grava os valores do arquivo na matriz
	printf("Chegou aqui\n");
   pthread_t tids[num_threads];
   printf("Chegou aqui\n");
   thread_args arguments[num_threads];
   primeira_coluna = 0;
   int divisao = coluna / num_threads;
   resto = coluna % num_threads;
   printf("Chegou aqui\n");
   printf("%d\n", resto);
   arguments[0].primeira_coluna = 0;
   printf("Passou\n");
   arguments[0].ultima_coluna = 0;
	printf("Chegou aqui\n");
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

   for (int i = 0; i < linha; i++) //linha
   {
      for (int j = 0; j < coluna; j++) //coluna
      {
         //printf("(%d %d ) ",i,j );
         //fscanf(fptr, "%lf", &matriz[i][j]);
         printf("%.2lf	", rotacionadaaa[i][j]);
         fprintf(rot, "%.2lf ", rotacionadaaa[i][j]);

      }
      printf("\n");
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

   for (int j = 0; j < coluna; j++) //coluna
   {
      for (int i = 0; i < linha; i++) //linha
      {

         //printf("%d %d\n", i, j);
         //pthread_mutex_trylock(&mutex_leitura);
         fscanf(fptr, "%lf", & matriz[j][i]);
         //printf("%.2lf	", matriz[j][i]);

         //pthread_mutex_unlock(&mutex_leitura);
      }
      //printf("\n");
   }
   //printf("matriz lida \n");
}

void * rotaciona(void * arg) {

   struct arg * struct_pointer = (struct arg * ) arg;

   //printf("preparando para girar\n");

   // printf("primeira e ultima %d %d\n", struct_pointer->primeira_coluna, struct_pointer->ultima_coluna);

   pthread_mutex_lock( & mutex_leitura);
   //printf(" 							bloqueou\n");

   for (int contcol = struct_pointer->primeira_coluna; contcol <= struct_pointer-> ultima_coluna; contcol++) {
      for (int contlin = 0; contlin < linha; contlin++) {
         //printf("\n ta no for da tread %d 	", struct_pointer->threadx);

         fscanf(fptr, "%lf", & matriz[contlin][contcol]);
         printf("%.2lf	[%d] [%d] ", matriz[contlin][contcol], contlin, contcol);

         //fprintf(rot, "%lf ",matriz[contlin][contcol] );

         int novalinha, novacoluna;
         novalinha = contcol;
         novacoluna = linha - contlin - 1;

         //printf("%d %d \n", novalinha, novacoluna);
         double aux;
         aux = matriz[contlin][contcol];
         // printf("chegou aqui?\n");
         rotacionadaaa[novalinha][novacoluna] = aux;
      }
      //printf("mudou de coluna\n");
      //fprintf(rot, "\n");

   }
   // printf("saiu\n");

   //printf("			desbloqueou\n");
   pthread_mutex_unlock( & mutex_leitura);

   pthread_exit(0);

}
