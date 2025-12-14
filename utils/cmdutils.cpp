/** 
 * cmdutils.cpp
 * @author: Yeray Alcala Paz
 * 
 * Implementación de algunas funciones
 *  útiles respecto a los comandos.
 */

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
    in = NULL; out = NULL; err = NULL; appout = NULL;i++;
    
    const int n = strlen(str);
    for(int i = 0; i < n; i++) {
        switch(*(str+i)) {
            case '<':{
                *(str+i) = '\0';
                in = str+i+1;
                break;
            }
            case '>':{
                *(str+i) = '\0';
                if(i > 0 && *(str+i-1) == '2') { // 2> ERR
                    *(str+i-1) = '\0';
                    err = str+i+1;
                } else if(i < n-1 && *(str+i+1) == '>') { // >> APPOUT
                    *(str+i+1) = '\0';
                    appout = str+i+2;
                    out = nullptr;
                } else { // > OUT
                    out = str+i+1;
                    appout = nullptr;
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

/**
 * Changes file descriptors of each parameter if non-null. If nullptr, nothing.
 */
void _doRedirections(char *pathin, char *pathout, char *patherr, char *pathappout, int exitCode) {
    if(pathin != nullptr) {
        close(0);
        int fd = open(pathin, O_RDONLY);
        if ( fd == -1 ){
            cerr << "ERROR: I can't open file " << pathin << ". Is it readable?" << endl;
            exit(exitCode);
        }
    }
    if(pathout != nullptr) { // truncate out is more important than append out.
        close(1);
        int fd = open(pathout, O_WRONLY|O_CREAT|O_TRUNC, 0644);
        if ( fd == -1 ){
            cerr << "ERROR: I can't open file " << pathout << ". Is it writeable?" << endl;
            exit(exitCode);
        }
    } else if(pathappout != nullptr) {
        close(1);
        int fd = open(pathappout, O_WRONLY|O_CREAT|O_APPEND, 0644);
        if ( fd == -1 ){
            cerr << "ERROR: I can't open file " << pathappout << ". Is it writeable?" << endl;
            exit(exitCode);
        }
    }
    if(patherr != nullptr) {
        close(2);
        int fd = open(patherr, O_WRONLY|O_CREAT|O_TRUNC, 0644);
        if ( fd == -1 ){
            cerr << "ERROR: I can't open file " << patherr << ". Is it writeable?" << endl;
            exit(exitCode);
        }
    }
}


/**
 * redirects process and executes command with exit code error given.
 * 
 * It's a private method, imagine how it works.
 */
void _execChild(char **splitCmd, char *pathin, char *pathout, char *patherr, char *pathappout, int exitCode) {
    _doRedirections(pathin, pathout, patherr, pathappout, exitCode);
    execvp(splitCmd[0], splitCmd);
    std::cerr << "ERROR: Couldn't execute command! Does it exist? " << endl 
        << splitCmd[0] << ": " << std::strerror(errno) << '\n';
    free(splitCmd);
    exit(exitCode);
}

/**
 * redirects process and executes command with default exit code error -1.
 * 
 * It's a private method, imagine how it works.
 */
void _execChild(char **splitCmd, char *pathin, char *pathout, char *patherr, char *pathappout) {
    _execChild(splitCmd, pathin, pathout, patherr, pathappout, -1);
}

void ejecutar(char command[]) {
    if(command == NULL || command[0] == '\0') return;

    char** trozosTuberia;
    int tuberiaCount = strsplit(command, "|", trozosTuberia);
    if(tuberiaCount < 0 || tuberiaCount > 2) {
        cerr << "ERROR: There has been a fatal error while trying to execute command!" << endl;
        exit(3);
    }

    char *pathin0, *pathout0, *patherr0, *pathappout0;
    getredir(trozosTuberia[0], pathin0, pathout0, patherr0, pathappout0);
    
    char** trozos_cmd0; 
    int count0 = strsplit(trozosTuberia[0], " \t", trozos_cmd0); // Separate argument.
    if(trozos_cmd0[0] == NULL || trozos_cmd0[0][0] == '\0') return;
    if(tuberiaCount == 1) { // Si no hay tuberias...
        if(strcmp(trozos_cmd0[0], CD) == 0) {
            // Aquí no usamos fork porque si no sólo se
            // movería el proceso hijo, no el padre...
            if(count0 > 2) cerr << "cd: Too many arguments" << endl;
            else {
                if(count0 == 1 || (count0 == 2 && trozos_cmd0[1][0] == '~')) {
                    // Miramos la varable $HOME. Como podría ser NULL, por si acaso miramos también
                    // su directorio HOME en su fila del passwd, aunque esté desaconsejado (lo uso como un fallback).
                    
                    if(chdir(getenv("HOME") != NULL ? getenv("HOME") : getpwuid(getuid())->pw_dir) != 0)
                        perror("cd: Cannot move to home");
                } else if(chdir(trozos_cmd0[1]) != 0)
                    perror("cd: Cannot move to directory");
                
                
            }
        } else {
            pid_t pid = fork();
            if (pid == -1) {
                perror("ERROR: Error whilst creating child process.\n");
                exit(1);
            } else if (pid == 0) 
                _execChild(trozos_cmd0, pathin0, pathout0, patherr0, pathappout0); // Proceso hijo
        }

        wait(NULL);
        free(trozos_cmd0);
    } else if(tuberiaCount == 2) {
        int unknownPipe[2];
        if (pipe(unknownPipe) == -1)  { 
            perror("ERROR: Couldn't create pipe!");
            exit(1);
        }

        char *pathin1, *pathout1, *patherr1, *pathappout1;
        getredir(trozosTuberia[1], pathin1, pathout1, patherr1, pathappout1);
        
        char** trozos_cmd1; 
        int count1 = strsplit(trozosTuberia[1], " \t", trozos_cmd1); // Separate arguments.

        pid_t pid1 = fork(); // First child (first command).
        if (pid1 == -1) {
            perror("ERROR: Couldn't create process with fork.");
            exit(1);
        }
        if (pid1 == 0) {
            close(unknownPipe[0]);  // Closes reading in pipe.
            if ( dup2(unknownPipe[1], 1) == -1 ) {  // Redirects write.
                perror("ERROR: Can't redirect stdout of pipe!");
                exit(2);
            }
            close(unknownPipe[1]); // stdout already redirected, closes descriptor. 
            
            _execChild(trozos_cmd0, pathin0, pathout0, patherr0, pathappout0, 3);
        }
        
        pid_t pid2=fork(); // Second child (second command).
        if (pid2 == -1) {
            perror("ERROR: Couldn't create process with fork.");
            exit(4);
        }
        if (pid2 == 0) {
            close(unknownPipe[1]);  // Closes writting in pipe.
            if (dup2(unknownPipe[0], 0) == -1) { // Redirects read.
                perror("ERROR: Can't redirect stdin of pipe!");
                exit(5);
            }
            close(unknownPipe[0]);  // stdin already redirected, closes descriptor.
            
            _execChild(trozos_cmd1, pathin1, pathout1, patherr1, pathappout1, 6);
        }
        

        // Parent closes pipe
        close(unknownPipe[0]);
        close(unknownPipe[1]);
        
        // Parent waits for children and frees memory.
        wait(NULL);
        wait(NULL);
        free(trozos_cmd0);
        free(trozos_cmd1);
    }
}