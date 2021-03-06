#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

void Reset_Color() {
    printf("\033[0m");
}

void Bold_Blue() {
    printf("\033[1;34m");
}

void Bold_Red() {
    printf("\033[1;31m");
}

void impCloud(int pid, int ppid, int N, int i){
    Bold_Red();
    printf("                       __________________________________________________\n");
    printf("                      ( ");
    Bold_Blue();
    printf("HE FINALIZADO! pid: %d, ppid: %d, N: %d, i: %d", pid, ppid, N, i);
    Bold_Red();
    printf(" )\n");
    printf("                      (__  ______________________________________________)\n");
    printf("                         )/\n");
    Reset_Color();
}

void impMeeseekBox(){
    printf("\n*********************\n\n");
    Bold_Blue();
    printf("       _____\n");
    printf(" _____|");
    Bold_Red();
    printf("█████");
    Bold_Blue();
    printf("|_____\n");
    printf("|\\               /|\n");
    printf("| \\_____________/ |\n");
    printf("| |             | |\n");
    printf("| | ");
    Reset_Color();
    printf("Mr Meeseeks");
    Bold_Blue();
    printf(" | |\n");
    printf("| |     ");
    Reset_Color();
    printf("BOX");
    Bold_Blue();
    printf("     | |\n");
    printf("| |_____________| |\n");
    printf("| /             \\ |\n");
    printf("|/_______________\\|\n");
    Reset_Color();
}

void impMeeseek(){
    Bold_Red();
    printf("                  \\)(/\n");
    Bold_Blue();
    printf("                .:/:soo\n");   
    printf("               /mMMMMMMM-\n");
    printf("              oMM");
    Reset_Color();
    printf("▄");
    Bold_Blue();
    printf("MMM");
    Reset_Color();
    printf("▄");
    Bold_Blue();
    printf("MM+\n");
    printf("              hMMMMMMMMMs\n");
    printf("              :MMMMMMMMm.\n");
    printf("              .yMMMMMd+`\n");
    printf("           :smNNMMMNmms/`\n");
    printf("        .smh+--MMMMN .+hNmo`\n");
    printf("        :mNs. yMMMMM-  -yMN-\n");
    printf("          +mMhNMMMMMdsNMd+`\n");
    printf("           `NMMMMMMMMMM:\n");
    printf("            :hmMMMMMMy.\n");
    printf("              `dMmMN/\n");
    printf("               dM-NM`\n");
    printf("               NM`hM/\n");
    printf("              `Mm +Ms\n");
    printf("              .Mm `Md\n");
    printf("              .MM` NM`\n");
    printf("              .MM: NM/\n");
    printf("           `+hMMMo:MMm:\n");
    Reset_Color();
} 

void impFailedTask(){
    printf("       ███۞███████ ]▄▄▄▄▄▄▄▄▄▄▄▄▃ ");
    Bold_Red();
    printf("* * * * *\n");
    Reset_Color();
    printf("       ▂▄▅█████████▅▄▃▂\n");
    printf("       I███████████████████].\n");
    printf("       ◥⊙▲⊙▲⊙▲⊙▲⊙▲⊙▲⊙◤...\n");
}

int realizarOperacion(char operador, int op1, int op2, int* fd){
    double resultado;
    int resuelto = 0;
    switch (operador)
    {
        case '+':
            resultado = op1 + op2;
            break;
        case '-':
            resultado = op1 - op2;
            break;
        case '*':
            resultado = op1 * op2;
            break;
        case '/':
                if (op2 == 0){
                    resuelto = 1;
                }
                else{
                    resultado = op1 / op2;
                }
            break;
        default:
            break;
    }
    if(resuelto == 0){
        printf("I'm %d, the result is %f \n", getpid(), resultado);

        char* str_resultado = malloc(sizeof(char)*50);
        snprintf(str_resultado, 50, "%f", resultado);

        close(fd[0]);
        write(fd[1], str_resultado, sizeof(str_resultado));
        close(fd[1]);
    }
    return resuelto;
}

char* operacionAritmetica(){
    char* operacion = malloc(sizeof(char)*10000);

    printf("Los Meeseeks resuelven operaciones aritmeticas binarias como '2 + 37' \n");
    printf("Operaciones + - * /\n");
    printf("Ingrese la operacion a resolver:  ");

    scanf("%[^\n]s", operacion);
    getchar();
    int op1, op2;
    int resultado = 0;
    char operador;

    sscanf(operacion, "%d %c %d", &op1, &operador, &op2);

    clock_t inicio = clock();
    double tiempoTotal = 0.0;

    int* fd = malloc(sizeof(int)*2);
    pipe(fd);
    char* output = malloc(sizeof(char)*50);

    int pid = fork();

    if(pid == 0){
        printf("\nHi I'm Mr Meeseeks! Look at Meeeee. Pid: %d \n\n", getpid());
        resultado = realizarOperacion(operador, op1, op2, fd);
        if(resultado == 0){
            impMeeseek();
            Bold_Blue();
            printf("I'm Mr Meeseeks %d! I achieved the task. \n", getpid());
            Reset_Color();
        }
        else{
            impFailedTask();
            Bold_Red();
            printf("Oh God, I'm Mr Meeseeks %d! I failed.  \n", getpid());
            Reset_Color();

            output = "ERROR";
            close(fd[0]);
            write(fd[1], output, sizeof(output));
            close(fd[1]);
        }
    }
    else{
        if(pid < 0) { //ocurrio un error
            fprintf(stderr, " Existence is Pain ");
        }
        else{
            wait(NULL);
            tiempoTotal = (double)(clock() - inicio) / CLOCKS_PER_SEC;
            printf("The Mr Meeseek %d lasted %f \n", pid, tiempoTotal);

            close(fd[1]);
            read(fd[0], output, sizeof(output));
            close(fd[0]);
        }
    }

    char* mensaje = malloc(sizeof(char)*1000);
    
    strcat(mensaje, "Operacion Aritmetica: ");
    strcat(mensaje, operacion);
    strcat(mensaje, ", Resultado: ");
    strcat(mensaje, output);
    strcat(mensaje, "\n");

    return mensaje;
}

char* ejecutarPrograma(){

    char* programa = malloc(sizeof(char)*10000);

    printf("Ingrese el programa a ejecutar (por ejemplo 'gedit'):  ");
    scanf("%[^\n]s", programa);
    getchar();

    clock_t inicio = clock();
    double tiempoTotal = 0.0;
    int resultado = 0;

    int fd[2];
    pipe(fd);
    char* exito = malloc(sizeof(char)*5);

    int pid = fork();

    if(pid == 0){
        printf("\nHi I'm Mr Meeseeks! Look at Meeeee. Pid: %d \n\n", getpid());
        resultado = system(programa);
        if(resultado == 0){
            impMeeseek();
            Bold_Blue();
            printf("I'm Mr Meeseeks %d! I achieved the task. \n", getpid());
            Reset_Color();

            exito = "TRUE";
        }
        else{
            impFailedTask();
            Bold_Red();
            printf("Oh God, I'm Mr Meeseeks %d! I failed.  \n", getpid());
            Reset_Color();

            exito = "FALSE";
        }
        close(fd[0]);
        write(fd[1], exito, sizeof(exito));
        close(fd[1]);
    }
    else{
        if(pid < 0) { //ocurrio un error
            fprintf(stderr, " Existence is Pain ");
        }
        else{
            wait(NULL);
            tiempoTotal = (double)(clock() - inicio) / CLOCKS_PER_SEC;
            printf("The Mr Meeseek %d lasted %f \n", pid, tiempoTotal);

            close(fd[1]);
            read(fd[0], exito, sizeof(exito));
            close(fd[0]);
        }
    }

    char* mensaje = malloc(sizeof(char)*1000);
    
    strcat(mensaje, "Ejecutar Programa: ");
    strcat(mensaje, programa);
    strcat(mensaje, ", Exito: ");
    strcat(mensaje, exito);
    strcat(mensaje, "\n");

    return mensaje;
}