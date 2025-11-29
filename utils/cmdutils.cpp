
#include "strutils.h"

#include <unistd.h>   // Proporciona acceso a las llamadas al sistema 
                      // a través de la API POSIX, como fork, chdir
#include <cstdlib>    // Necesario para exit                      
#include <cstring>    // Necesario para la función strcpy
#include <sys/wait.h> // Necesario para la función wait
#include <fcntl.h>  // Necesario para llamadas sistema archivos: open, creat...
#include <pwd.h> // Necesario para obtener el Home del usuario.
using namespace std;

static char CD[] = "cd";

void getredir(char* str, char* & in, char* & out, char* & err, char* & appout) {
    int i = 0;
    in = NULL; out = NULL; err = NULL; appout = NULL;
    const int n = strlen(str);
    for(int i = 0; i < n; i++) {
        switch(*(str+i)) {
            case '<': {
                *(str+i) = '\0';
                in = str+i+1;
                break;
            }
            case '>': {
                *(str+i) = '\0';
                if(i > 0 && *(str+i-1) == '2') {
                    *(str+i-1) = '\0';
                    err = str+i+1;
                } else if(i < n-1 && *(str+i+1) == '>') {
                    *(str+i+1) = '\0';
                    appout = str+i+2;
                    out = NULL;
                } else {
                    out = str+i+1;
                    appout = NULL;
                }

                break;
            }
        }
    }

    in = strtrim(in);
    if (in && !in[0]) in = NULL;
    out = strtrim(out);
    if (out && !out[0]) out = NULL;
    err = strtrim(err);
    if (err && !err[0]) err = NULL;
    appout = strtrim(appout);
    if (appout && !appout[0]) appout = NULL;
}

void ejecutar(char command[]) {
    char** cmdSplitComments;
    strsplit(command, "#", cmdSplitComments);
    char *pathin, *pathout, *patherr, *pathappout;
    getredir(cmdSplitComments[0], pathin, pathout, patherr, pathappout); // Considerar y eliminar redirecciones.
    char** trozos_cmd; 
    int count = strsplit(cmdSplitComments[0], " \t", trozos_cmd); // Considerar y separar argumentos.

    if(strcmp(trozos_cmd[0], CD) == 0) {
        // Aquí no usamos fork porque si no sólo se
        // movería el proceso hijo, no el padre...
        if(count > 2)
            cerr << "cd: Too many arguments" << endl;
        else {
            if(count == 1 || (count == 2 && trozos_cmd[1][0] == '~')){
                // Miralos la varable $HOME. Como podría ser NULL, por si acaso miramos también
                // su directorio HOME en su fila del passwd, aunque esté desaconsejado (es como un fallback).
                if(chdir(getenv("HOME") != NULL ? getenv("HOME") : getpwuid(getuid())->pw_dir) != 0) {
                    perror("cd: Cannot move to home");
                }
            } else {
                if(chdir(trozos_cmd[1]) != 0) {
                    perror("cd: Cannot move to directory");
                }
            }
            
        }
    } else {
        pid_t pid = fork();
        if (pid == -1) perror("Error whilst creating child process.\n");
        else if (pid == 0) {  // Proceso hijo
            if(pathin != nullptr) {
                close(0);
                int fd = open(pathin, O_RDONLY);
                if ( fd == -1 ){
                    cerr << "I can't open file " << pathin << ". Is it readable?" << endl;
                    exit(-1);
                }
            }
            if(pathout != nullptr) {
                close(1);
                int fd = open(pathout, O_WRONLY|O_CREAT|O_TRUNC, 0644);
                if ( fd == -1 ){
                    cerr << "I can't open file " << pathin << ". Is it writeable?" << endl;
                    exit(-1);
                }
            } else if(pathappout != nullptr) {
                close(1);
                int fd = open(pathappout, O_WRONLY|O_CREAT|O_APPEND, 0644);
                if ( fd == -1 ){
                    cerr << "I can't open file " << pathappout << ". Is it writeable?" << endl;
                    exit(-1);
                }
            }
            if(patherr != nullptr) {
                close(2);
                int fd = open(patherr, O_WRONLY|O_CREAT|O_TRUNC, 0644);
                if ( fd == -1 ){
                    cerr << "I can't open file " << patherr << ". Is it writeable?" << endl;
                    exit(-1);
                }
            }
            execvp(trozos_cmd[0], trozos_cmd);
            perror("Unknown command!\n");
            free(trozos_cmd);
            exit(-1);
        }
    }

    wait(NULL);
    free(trozos_cmd); 
}