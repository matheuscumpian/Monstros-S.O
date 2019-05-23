#include <stdio.h>
#include <stdlib.h>

int main (int argc, char* argv[]){



  FILE* fptr; //Arquivo de Entrada
  FILE* rot; //Arquivo de Saída  
  int linha = atoi(argv[1]); //numero de linhas
  int coluna = atoi(argv[2]); //numero de colunas
  int num_threads = atoi(argv[3]); //numero de threads
  rot=fopen(argv[5],"w"); //arq1 recebe o nome do arquivo .root que vai ser criado
  fptr=fopen(argv[4],"r"); //arq1 recebe o nome do arquivo .dat que vai ser lido
  int num;
  int i,j;
  int mat[linha][coluna];
  int girada[linha][coluna];

  printf("I = %d J = %d T = %d\n", linha,coluna,num_threads);

  printf("Arquivo.root = %s\n", argv[argc-1]);

  printf("Arquivo.dat = %s\n", argv[argc-2]);

        for(i = 0; i < linha; i++)//linha
          {
              for(j = 0; j < coluna; j++)//coluna
              {
                   fscanf(fptr, "%d", &mat[i][j]);

              }
          }
  printf("matriz lida \n");

  printf("Girando matriz\n");

        for(j = 0; j < coluna; j++)//coluna aumenta 1
          {
                for(i=linha-1 ; i >=0; i--)//linha diminiu 1
                  {
                    fprintf(rot,"%d ",mat[i][j]);
                  }
                  fprintf(rot,"\n");
          }

fclose(fptr);
fclose(rot);

return(0);
}