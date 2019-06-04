#include <stdio.h>

#include <stdlib.h>

#include <pthread.h>

#include <sys/time.h>


FILE * fptr; //Arquivo de Entrada
FILE * rot; //Arquivo de Saída



typedef struct arg {
    int primeira_coluna; // Primeira coluna que a thread ira rotacionar
    int ultima_coluna; // Ultima coluna que a thread ira rotacionar
}
thread_args;

double ** rotacionada;
double ** matriz;
int linha;
int coluna;

void armazena();
void printa_rotacionada();
void * rotaciona(void * arg);
float tempo;


int main(int argc, char ** argv) {

    if (argc < 6) // Se os argumentos passados na execução não satisfazerem os requisitos EXIT(1)
    {

        printf("\nNumero de argumentos invalido\nTente como o exemplo a seguir:\n./giramatriz <numLinhas> <numColunas> <numThreads> <arquivoDeEntrada> <arquivoDeSaida> \n");
        exit(1);

    }

    linha = atoi(argv[1]); //numero de linhas
    coluna = atoi(argv[2]); //numero de colunas
    rot = fopen(argv[5], "w"); //rot recebe o nome do arquivo .rot que vai ser criado
    fptr = fopen(argv[4], "r"); //fptr recebe o nome do arquivo que vai ser lido

    int num_threads = atoi(argv[3]); //numero de threads
    int resto;
    int i = 0;
    clock_t t;

 
    if (num_threads != 2 && num_threads != 4 && num_threads != 8 && num_threads != 16) {
        printf("\nNumero de threads invalido\nTente com uma das opcoes a seguir:\n2,\t 4,\t 8,\t 16.\n");
        exit(1);
    }

    // Alocação dinamica das duas matrizes
    rotacionada = (double ** ) malloc(coluna * sizeof(double * ));
    for (i = 0; i < coluna; i++)
        rotacionada[i] = (double * ) malloc(linha * sizeof(double));

    matriz = (double ** ) malloc(linha * sizeof(double * ));
    for (i = 0; i < linha; i++)
        matriz[i] = (double * ) malloc(coluna * sizeof(double));

    armazena(); //roda a funcao que grava os valores do arquivo na matriz

    pthread_t tids[num_threads]; // Vetor com os id's das Threads

    thread_args arguments[num_threads]; // Cria um vetor de structs de argumentos, um para cada thread.
    int divisao = coluna / num_threads; /* divisao vai ser o "gap" de cada thread, ou seja, quantas posicoes ela ira rotacionar*/
    resto = coluna % num_threads; // Se houver resto na divisao de coluna/thread.

    //Startando argumentos para primeira thread
    arguments[0].primeira_coluna = 0;
    arguments[0].ultima_coluna = 0;
	
    t = clock(); // começa a contar os clocks
    for (int i = 0; i < num_threads; i++) //for que cria varias threadds
    {
        printf("criando thread: %d\n", i);
        if (i == 0) {
            /* Se o i == 0, primeira coluna da primeira thread sempre será zero */
            arguments[i].primeira_coluna = 0;
            arguments[i].ultima_coluna = divisao - 1; // Divisao - 1 na primeira thread, pois começamos a contar do zero.
            printf("Primeira_Coluna Thread[%d]: %d\n", i + 1, arguments[i].primeira_coluna);
            printf("Ultima_Coluna Thread[%d]: %d\n", i + 1, arguments[i].ultima_coluna);
        } else if (i == num_threads - 1) {
            /* Se o num_threads - 1, ultima coluna da ultima thread sempre recebera as colunas que sobraram no resto */
            arguments[i].primeira_coluna = arguments[i - 1].ultima_coluna + 1;
            arguments[i].ultima_coluna = arguments[i - 1].ultima_coluna + divisao + resto;
            printf("Primeira_Coluna Thread[%d]: %d\n", i + 1, arguments[i].primeira_coluna);
            printf("Ultima_Coluna Thread[%d]: %d\n", i + 1, arguments[i].ultima_coluna);
        } else {
            /* Se não for a primeira nem a ultima thread, a primeira coluna da thread e igual a ultima coluna da outra thread + 1, isto é se a ultima thread terminou na coluna 30, a proxima thread começa na 31. A ultima coluna da thread vai ser a ultima coluna da thread anterior mais o gap, se o gap é 30 e a thread anterior terminou na coluna 20, a proxima thread ira começar na 21 e terminar na 50 */
            arguments[i].primeira_coluna = arguments[i - 1].ultima_coluna + 1;
            arguments[i].ultima_coluna = arguments[i - 1].ultima_coluna + divisao;
            printf("Primeira_Coluna Thread[%d]: %d\n", i + 1, arguments[i].primeira_coluna);
            printf("Ultima_Coluna Thread[%d]: %d\n", i + 1, arguments[i].ultima_coluna);

        }
        // Cria a thread[i] e manda a struct de argumentos[i].
        pthread_create( & tids[i], NULL, rotaciona, & arguments[i]);

    }

    for (int i = 0; i < num_threads; i++) {
        //Aguarda thread[i] finalizar.
        pthread_join(tids[i], NULL);
        printf("Terminando thread: %d\n", i + 1);
    }
    t = clock() - t; //diferença de clock da criação até finalização das threads
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // Tempo que se passou da criação a fianalização das threads

    printf("Tempo: %f segundos\n", time_taken);
    // Ao passo que cada thread já finalizou seu trabalho podemos printar no arquivo a matriz rotacionada.
    printa_rotacionada();

    fclose(fptr);
    fclose(rot);
    free(matriz);
    free(rotacionada);
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

    struct arg * struct_pointer = (struct arg * ) arg; // Coerção do argumento tipo void para o tipo da struct argumentos

    

    for (int contcol = struct_pointer -> primeira_coluna; contcol <= struct_pointer -> ultima_coluna; contcol++) {
        for (int contlin = 0; contlin < linha; contlin++) {

           
            int novalinha, novacoluna;

            novalinha = contcol; //Nova linha = coluna atual
            novacoluna = linha - contlin - 1; // linha = quantidade de linhas da matriz, contlin = linha atual

            double aux;
            aux = matriz[contlin][contcol];

            rotacionada[novalinha][novacoluna] = aux;
        }

    }
   

    pthread_exit(0);

}

void printa_rotacionada() {

    /* Perceba que estamos printando a matriz com argumentos 'invertidos' pois se rotacionarmos uma matriz não quadrada o número de linhas troca com o de colunas, caso seja quadrada não fará diferença */
    for (int i = 0; i < coluna; i++) {
        for (int j = 0; j < linha; j++) {

            fprintf(rot, "%.2lf ", rotacionada[i][j]);

        }

        fprintf(rot, "\n");
    }

}
