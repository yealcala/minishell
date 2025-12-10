/** 
 * strutils.cpp
 * @author: Yeray Alcala Paz
 * 
 * Implementación de algunas funciones
 *  útiles para cadenas de carácteres.
 */
#include "strutils.h"

using namespace std;

int strsplit(char * str, const char * delim, char ** & parts) {  
  parts = (char **) malloc(sizeof(char*));
  parts[0] = NULL;

  if (str == NULL) return 0;  // caso que no recibamos str

  int numParts = 0;
  
  int i = 0;
  while (str[i] != '\0') {
    int j = i;
	  // recorrer str hasta encontrar un delimitador o el final
    while (str[j] != '\0' && strchr(delim, str[j]) == NULL) {
      j++;
    }

    if (str[j] != '\0') { // Si no se ha llegado al final
      str[j] = '\0'; // terminar la subcadena, sustituir el delimitador por un '\0'
      j++;           // Avanzar al siguiente caracter de la cadena str
    }

    if (str[i] != '\0') { // Si la subcadena tiene caracteres
      // actualizar trozos
      numParts++;
      parts = (char **) realloc(parts, (numParts+1)*sizeof(char*));
      parts[numParts-1] = &str[i];
      parts[numParts] = NULL; // necesario para usar después con execvp
    }

    i = j; // Para continuar con el siguiente trozo
  }

  return numParts;
}

char* strtrim(char* str) {
  if (str == NULL)
    return NULL;
   
 // Avanza por la izquierda
  while (*str == ' ' || 
         *str == '\t') {
    str++;   
  }
     
  // Avanza por la derecha  
  char * auxi = str + strlen(str) - 1;
  while (auxi > str && 
         (*auxi == ' ' || *auxi == '\t')) { 
    auxi--;
  }
  
  *(auxi+1)='\0'; // Poner el carácter de fin de cadena
  
  // Devuelve la dirección del nuevo string
  return str;
}

int strsch(char* str, char c) {
  if(str == NULL) return -1;

  int i = 0;
  while(str[i] != '\0')
    if(str[i] == c)
      return i;
    else ++i;

  return -1;
}