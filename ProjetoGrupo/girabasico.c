#include <stdio.h>
#include <stdlib.h>
#define tt 4//linhas
#define yy 5//colunas
int main (int argc, char* argv[]){
int num;
FILE* fptr;
FILE* rot;
int i,j;
double mat[tt][yy];

int girada [yy][tt];
fptr=fopen("file.txt","r");
rot=fopen("rot.txt","w");
if (fptr==NULL)
{
printf("Erro ftpr");
exit(1);
}
if (rot==NULL)
{
printf("Erro rot");
exit(1);
}



        for(i = 0; i < tt; i++)//linha
          {
              for(j = 0; j < yy; j++)//coluna
              {
                   if (!fscanf(fptr, "%lf ", &mat[i][j]))
                       break;
                  // mat[i][j] -= '0';
                   printf("%.2lf ",mat[i][j]);
              }
         printf("\n");
          }
printf("matriz lida \n");
printf("Girando matriz\n");


        for(j = 0; j <yy; j++)//coluna aumenta 1
          {
                for(i=tt-1 ; i >=0; i--)//linha diminiu 1
                  {
                    printf("%.2lf ",mat[i][j]);
                    fprintf(rot,"%.2lf ",mat[i][j]);

                  }
            printf("\n");
           fprintf(rot,"\n");
          }

fclose(fptr);
fclose(rot);

return(0);
}
