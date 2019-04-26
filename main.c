#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <time.h>

#include "meeseeks_forks.c"
//#include "operaciones.c"

int main(){
    srand (time(NULL)); //Inicializa el seed del rand

    char n;
    char* tarea;
    double dificultad;


    int MeeseekBox = getpid();
    printf("Meeseeks Box    %d \n", MeeseekBox);
    
    while(n != '4'){
        impMeeseekBox();
        printf("Seleccione la accion que desea realizar:\n    1. Accion Textual\n    2. Operaciones Aritmeticas\n    3. Ejecutar Programa\n    4. Salir\n\nIngrese su opcion: ");

        scanf("%c", &n);
        getchar();
        
        switch (n)
        {
        case '1':
            tarea = leerSolicitud();
            dificultad = leerDificultad();

            iniciar(tarea, dificultad);
            break;
        case '2':
            operacionAritmetica();
            break;
        case '3':
            ejecutarPrograma();
            break;
        case '4':
            printf("Concluyendo Mr. Meeseeks Box, hasta luego\n");
            break;
        default:
            printf("Opcion inexistente en la Mr. Meeseeks Box  %c %d\n", n, getpid());
            break;
        }

        if(MeeseekBox != getpid()){
            break;
        }
    }
    return 0;
}