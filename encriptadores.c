#include <string.h>
#include <stdio.h>
#include "encriptadores.h"
#include <stdlib.h>
#define TAMANIO_VECTOR_S 255
#define EXITO 1

//hay mucho codigo repetido en los cifrados de cesar y vigenere
int cifrado_de_cesar(char* cadena,int key){
  unsigned char* cadena_aux = (unsigned char*)cadena;
  while(*cadena_aux != '\0'){
    *cadena_aux = (unsigned char)(*cadena_aux + key) /*% 256*/;
    cadena_aux++;
  }
  return EXITO;
}

int descifrado_de_cesar(char*cadena,int key){
  unsigned char* cadena_aux = (unsigned char*)cadena;
  while(*cadena_aux != '\0'){
    *cadena_aux = (unsigned char)(*cadena_aux - key) /*% 256*/;
    cadena_aux++;
  }
  return EXITO;
}

int cifrado_de_vigenere(char* cadena,char* key){
  unsigned char* cadena_aux = (unsigned char*)cadena;
  unsigned char* key_aux = (unsigned char*)key;
  while(*cadena_aux != '\0'){
    *cadena_aux = (unsigned char)(*cadena_aux + *key_aux)/* % 256*/;
    printf("|%i|",(int)*cadena_aux);
    cadena_aux++;
    key_aux++;
    if(*key_aux == '\0'){
      key_aux = (unsigned char*)key;
    }
  }
  printf("\n");
  return EXITO;
}

int descifrado_de_vigenere(char* cadena,char* key){
  unsigned char* cadena_aux = (unsigned char*)cadena;
  unsigned char* key_aux = (unsigned char*)key;
  while(*cadena_aux != '\0'){
    *cadena_aux = (unsigned char)(*cadena_aux - *key_aux)/* % 256*/;
    cadena_aux++;
    key_aux++;
    if(*key_aux == '\0'){
      key_aux = (unsigned char*)key;
    }
  }
  return EXITO;
}
/*
static void swap(unsigned char *s, unsigned int i, unsigned int j) {
    unsigned char temp = s[i];
    s[i] = s[j];
    s[j] = temp;
}

static void ksa(unsigned char *key, size_t key_length,unsigned char* vector_s) {
    unsigned int i,j;
    for (i = 0; i < 256; i++)
        vector_s[i] = (unsigned char)i;

    for (i = j = 0; i < 256; i++) {
        j = (j + key[i % key_length] + vector_s[i]) & TAMANIO_VECTOR_S;
        swap(vector_s, i, j);
    }


}

static unsigned char rc4_output(unsigned char* vector_s) {
  unsigned int i = 0,j = 0;

    i = (i + 1) & TAMANIO_VECTOR_S;
    j = (j + vector_s[i]) & TAMANIO_VECTOR_S;

    swap(vector_s, i, j);

    return vector_s[(vector_s[i] + vector_s[j]) & TAMANIO_VECTOR_S];
}

int cifrado_de_rc4(char* cadena,char* key){
  unsigned char vector_s[TAMANIO_VECTOR_S];
  unsigned char* cadena_aux = (unsigned char*)cadena;
  ksa((unsigned char*)key,strlen(key),vector_s);
  while(*cadena_aux != '\0'){
    *cadena_aux = (unsigned char)(*cadena_aux ^ rc4_output(vector_s));
    printf("|%i|",(int)*cadena_aux);

    cadena_aux++;

    }
    return EXITO;

}
*/