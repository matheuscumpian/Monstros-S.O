#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


void* rotaciona(void* arg)
{
	
}

struct arg
{
	double **matriz;
	int linhas;
	int colunas;
};

int main (int argc, char* argv[]){

	FILE* fptr; //Arquivo de Entrada
	FILE* rot; //Arquivo de Saída
	int linha = atoi(argv[1]); //numero de linhas
	int coluna = atoi(argv[2]); //numero de colunas
	int num_threads = atoi(argv[3]); //numero de threads
	rot=fopen(argv[5],"w"); //arq1 recebe o nome do arquivo .root que vai ser criado
	fptr=fopen(argv[4],"r"); //arq1 recebe o nome do arquivo .dat que vai ser lido
	int num;
	struct arg t_arg;
	
	double **mat = (double **)malloc(linha * sizeof(double *)); 
   	for (int i=0; i<coluna; i++) 
         	mat[i] = (double *)malloc(coluna * sizeof(double)); 
	
	t_arg.matriz = mat;

	printf("I = %d J = %d T = %d\n", linha,coluna,num_threads);

	printf("Arquivo.root = %s\n", argv[argc-1]);

	printf("Arquivo.dat = %s\n", argv[argc-2]);

	pthread_t tids[num_threads];
	for (int i = 0; i < num_threads; i++)
	{
		pthread_create(&tids[i], NULL, rotaciona, &t_arg);
	}
	for(int i = 0; i < linha; i++)//linha
	{
		for(int j = 0; j < coluna; j++)//coluna
		{

			fscanf(fptr, "%lf", &mat[i][j]);

		}
	}
	printf("matriz lida \n");
	printf("Girando matriz\n");

	for(int j = 0; j < coluna; j++)//coluna aumenta 1
	{
		for(int i=linha-1 ; i >=0; i--)//linha diminiu 
		{
			fprintf(rot,"%.2lf ",mat[i][j]);
		}
		fprintf(rot,"\n");
	}

	for(int i = 0; i < num_threads; i++)
	{
		pthread_join(tids[i], NULL);
	}

	fclose(fptr);
	fclose(rot);

	return(0);
	}
