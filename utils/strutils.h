
#ifndef STRUTILS_H
#define STRUTILS_H

#include <iostream>
#include <cstring>

using namespace std;

/**
 * Divide una cadena de caracteres en un array de subcadenas. 
 * El último elemento del array de subcadenas tendrá el valor NULL (esto es necesario para 
 * usar después con execvp).
 * Para determinar las subcadenas se utiliza cualquiera de los caracteres contenidos en otra 
 * cadena recibida como segundo parámetro. 
 *  
 * La función reserva memoria dinámicamente para almacenar el array de subcadenas.
 * La memoria reservada debería liberarse cuando no se vaya a necesitar más.
 * 
 * ATENCION !! La cadena original se destruye. Si es necesario conservarla, hay que hacer 
 * una copia antes de invocar a la función.
 * 
 * @param str   (in)  Cadena de caracteres a dividir (se destruye)
 * @param delim (in)  Cadena que especifica los carácteres a usar para la división de la cadena
 * @param parts (out) Array de cadenas con las partes (subcdenas) en la que se divide la cadena
 * @return Número de trozos en los que se descompone la cadena original 
 * 
 * Ejemplo: El siguiente código trocea la cadena "Hola, que tal estás" en cuatro trozos, ya que 
 * usamos el espacio en blanco o la coma como delimitadores (" ,"). Los trozos son "Hola", 
 * "que", "tal" y "estás". Notar que tras la ejecución de la función la cadena inicial ya no será 
 * nunca más "Una línea   de texto\t cualquiera", sino que ha queda reducida a "Una".
 * 
 *   char cad [] = "Hola, que tal estas";
 *   char ** trozos;
 *   int n = strsplit(cad, " ,", trozos); 
 *   cout << "Cadena dividida en " << n << " trozos" << endl;
 *   // Recorrido con notación de vectores
 *   int i=0;
 *   while (trozos[i] != NULL) { // El último trozo del array es siempre NULL
 *     cout << trozos[i] << endl;
 *     i++;
 *   }
 *   ...
 *   // Cuando la variable trozos no se necesite
 *   free(trozos);
 */
int strsplit(char * str, const char * delim, char ** & parts);

/**
 * VERSIÓN notación de punteros a char
 * La función recibe una cadena de caracteres str, que puede estar precedida o terminada 
 * por uno o varios espacios en blanco o tabuladores ('\t'). 
 * Devuelve otra cadena que no tiene esos espacios ni tabuladores iniciales ni finales.
 * 
 * ATENCION !! La cadena original se modifica por el final (todos los espacios o tabuladores finales 
 * se pierden). Si es necesario conservarla, hay que hacer una copia antes de invocar a la función.
 * 
 * @param str (in)  Cadena de caracteres a procesar (es modificada)
 * @return puntero a char apuntando al primer caracter de la cadena str que no es un espacio o tabulador 
 */
char* strtrim(char* str);

#endif