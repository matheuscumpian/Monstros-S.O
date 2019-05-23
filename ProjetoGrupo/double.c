#include <stdio.h>
#include <math.h>
main(){
    double fl;
    double y,z;
    int a,b;
    int d,e;

    scanf("%d %d",&d ,&e);//valores a serem passados como primeiro e ultimo

    fl=((double)d)+((double)e/1000);//juntando os valores em uma double só
    a=(int)fl;//pegando o valor do número antes da vírgula (primeira linha)

    y=fmod(fl,1.0);
    z=y*1000;
    b =round(z);//pegando o valor depois da virgula (ultima linha)

    printf("fl =%f",fl);//double
    printf("a =%i ",a);//valor antes da virgula
    printf("b =%i ",b);//valor depois da virgula
   
   }

