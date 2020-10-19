#ifndef __ENCRIPTADOR_CESAR_H__
#define __ENCRIPTADOR_CESAR_H__
#define ERROR -1
#define EXITO 0
#define DESCIFRAR 2
#define CIFRAR 3

#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

typedef struct {
  void* key;
}encriptador_cesar_t;

int encriptador_cesar_init(encriptador_cesar_t* encriptador,void*key);
int encriptador_cesar(encriptador_cesar_t* encriptador,char* cadena,
                      size_t tamanio,int modo);
#endif
