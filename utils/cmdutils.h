/** 
 * cmdutils.h
 * @author: Yeray Alcala Paz
 * 
 * Archivo header con funciones
 *  útiles respecto a los comandos.
 */

#ifndef CMDUTILS_H
#define CMDUTILS_H

#include <iostream>
#include <cstring>

using namespace std;

/**
 * Gets an input from command line and separated command,
 *  in, and out redirections.
 * 
 * IMPORTANT!! inputted str will be converted into just the command part.
 *              Make sure to pass a copy if you want to keep it untouched.
 * 
 * @param str (in & out)  Command line input. Will be converted into just the command.
 * @param in (out)  trimmed in redirection of inputted raw command.
 * @param out (out) trimmed out redirection of inputted raw command.
 * @param err (out) trimmed err redirection of inputted raw command.
 * @param appout (out) trimmed appout redirection of inputted raw command.
 */
void getredir(char* str, char* & in, char* & out, char* & err, char* & appout);

/**
 * Executes a given string as if it was a command.
 * 
 * IMPORTANT!! Command is destructed. Consider making at copy before.
 * 
 * @param cmd (in) Command that will be executed.
 */
void ejecutar(char cmd[]);

#endif