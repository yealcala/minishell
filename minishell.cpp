/** 
 * minishell_2.cpp
 * @copyright esquema-shell.cpp
 * @copyright Eloy J. Mata & Francisco J. García Izquierdo
 * @author: Yeray Alcala Paz
 * 
 * CÓMO COMPILAR EL PROGRAMA:
 *    g++ -Iutils minishell.cpp utils/*.cpp -o minishell
 * 
 * CÓMO EJECUTAR EL PROGRAMA :
 *  - Versión interactiva: Ejecutar ./minishell.out
 *  - Versión fichero de comandos: Ejecutar ./minishell.out fichero
 */

#include <iostream>   // Necesario para cout
#include <fstream>    // Necesario para ifstream
#include <cstdlib>    // Necesario para la función exit

#include <unistd.h> // Necesario para chdir
#include <sys/wait.h> // Neceasario para la función wait

#include "utils/ioutils.h" // Utilidades para io.
                            // Necesario para la función read
#include "utils/strutils.h"  // Utilidades para string.
                            // Necesario para la función strsplit
#include "utils/cmdutils.h"  // Utilidades para string.
                            // Necesario para la función ejecutar
using namespace std;

void ejecutar(char cmd[]);

const int INPUT_MAX_SIZE = 255;
static char EXIT[] = "salir";
static char CD[] = "cd";

int main(int argc, char* argv[]) {
    char cmd[INPUT_MAX_SIZE+1];

    if (argc == 1) { // Interactive.
        char cwd[INPUT_MAX_SIZE+1];
        
        cout << (getcwd(cwd, sizeof(cwd)) != NULL ? cwd : "NULL") << ": ";

        read(cmd, INPUT_MAX_SIZE);

        char **trozos;
        while(strcmp(cmd, EXIT) != 0) {
            int num = strsplit(cmd, ";", trozos);
            for(int i = 0; i < num; i++) 
                ejecutar(*(trozos+i));
            
            if (getcwd(cwd, sizeof(cwd)) != NULL)
                cout << cwd << ": ";
            else cout << "/route/error" << ": ";

            read(cmd, INPUT_MAX_SIZE);
        }
        free(trozos);
    } else if (argc == 2) { // Execute script.
        ifstream entrada;
        entrada.open(argv[1]);
        if (entrada.fail()) {
            cerr << "Couldn't open file " << argv[1] << "!" << endl;
            exit(2);
        } else {
            char **trozos;
            entrada.getline(cmd, INPUT_MAX_SIZE, '\n'); 
            while (! entrada.eof() && strcmp(cmd, EXIT) != 0 ) { // While can read line and not exit command.
                int num = strsplit(cmd, ";", trozos);
                for(int i = 0; i < num; i++)
                    ejecutar(*(trozos+i));
                entrada.getline(cmd, INPUT_MAX_SIZE, '\n');
            }
            free(trozos);
            entrada.close();
        }
    } else { // More than two arguments or no args.
        cerr << "Número de argumentos incorrecto" << endl;
        exit(1);
    }
    
    exit(0);
}