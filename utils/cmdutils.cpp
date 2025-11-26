
#include "strutils.h"

#include <unistd.h>   // Proporciona acceso a las llamadas al sistema 
                      // a través de la API POSIX, como fork, chdir
#include <cstdlib>    // Necesario para exit                      
#include <cstring>    // Necesario para la función strcpy
#include <sys/wait.h> // Necesario para la función wait

using namespace std;

static char CD[] = "cd";

void getredir(char* str, char* & in, char* & out) {
    int i = 0;
    in = NULL; out = NULL;
    const int n = strlen(str);
    for(int i = 0; i < n; i++)
        switch(*(str+i)) {
            case '<': {
                *(str+i) = '\0';
                in = str+i+1;
                break;
            }
            case '>': {
                *(str+i) = '\0';
                out = str+i+1;
                break;
            }
        }
    in = strtrim(in);
    if (in && !in[0]) in = NULL;
    out = strtrim(out);
    if (out && !out[0]) out = NULL;
}

void ejecutar(char cmd[]) {
    char** trozos_cmd; 
    int count = strsplit(cmd, " \t", trozos_cmd);

    if(strcmp(trozos_cmd[0], CD) == 0) {
        // Aquí no usamos fork porque si no sólo se
        // movería el proceso hijo, no el padre...
        if(count > 2)
            cerr << "cd: Too many arguments" << endl;
        else {
            if(chdir(trozos_cmd[1]) == 0) {
                cout << "cd: Moved to " << trozos_cmd[1] << endl;
            } else {
                perror("cd: Cannot move to directory");
            }
        }
    } else {
        pid_t pid = fork();
        if (pid == -1)
        perror("Error al crear un proceso\n");

        else if (pid == 0) {  // Proceso hijo
        execvp(trozos_cmd[0], trozos_cmd);
        perror("Comando desconocido");
        free(trozos_cmd);
        exit(-1);
        }
    }

    
    
    wait(NULL);
    free(trozos_cmd); 
}