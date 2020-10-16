#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "socket.h"
#define TAMANIO_MENSAJE 10
#define TAMANIO_RESPUESTA 10
//capaz sacar TAMANIO_RESPUESTA

static void hints_innit(struct addrinfo* hints,int ai_family,int ai_socktype,int ai_flags){
  memset(hints,0,sizeof(struct addrinfo));
  hints->ai_family = ai_family;
  hints->ai_socktype = ai_socktype;
  hints->ai_flags = ai_flags;
}

int socket_init(socket_t* self,struct addrinfo* resultados){
  self->fd = socket(resultados->ai_family, resultados->ai_socktype, resultados->ai_protocol);
  return (self->fd > 0?EXITO:ERROR);
}

int socket_bind_and_listen(socket_t* self, const char* host, const char* service){
  //capaz cambiar la estructura de resultados por resultado y dividir bind con listen
  int resultado_bind,val = 1;
  struct addrinfo hints;
  struct addrinfo* resultados;// enrealidad seria para iterar
  hints_innit(&hints,AF_INET,SOCK_STREAM,AI_PASSIVE);
  if(getaddrinfo(NULL,service, &hints,&resultados) < 0){
    return ERROR;
  }
  if(socket_init(self,resultados) < 0){
    freeaddrinfo(resultados);
    return ERROR;
  }
  setsockopt(self->fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
  resultado_bind =  bind(self->fd, resultados->ai_addr,resultados->ai_addrlen);
  freeaddrinfo(resultados);
  if(resultado_bind == ERROR){
    socket_uninit(self,SHUT_RD);
    return ERROR;
  }
  int resultado_listen = listen(self->fd, 10/*ver cuantas deberian ser,cpaz una sola es*/);
  return resultado_listen;
}

int socket_uninit(socket_t* self,int modo_de_shutdown){
  shutdown(self->fd,modo_de_shutdown);
  return close(self->fd);
}

int socket_accept(socket_t* listener,socket_t*peer){
    peer->fd = accept(listener->fd, NULL, NULL);
    return((peer->fd == -1)? ERROR:EXITO);
}

int socket_connect(socket_t *self, const char *host, const char *service){
  //aca no necesito iterar asique capaz conviene sacarlo
  bool conecte = false;
  int resultado_socket;
  struct addrinfo hints;
  struct addrinfo* resultados,*ptr;
  hints_innit(&hints,AF_INET,SOCK_STREAM,0);
  if (getaddrinfo(host,service, &hints,&resultados) < 0){
    return ERROR;
  }
  ptr = resultados;
  while (ptr != NULL && !conecte){
    resultado_socket = socket_init(self,ptr);
    if (resultado_socket != ERROR && connect(self->fd,ptr->ai_addr,ptr->ai_addrlen) != ERROR){
      conecte = true;
    }
    ptr = ptr->ai_next;
  }
  freeaddrinfo(resultados);
  return (self->fd < 0? ERROR:EXITO);
}

int socket_send(socket_t* self, const char* buffer, size_t length){
  size_t bytes_no_enviados = length;
  ssize_t bytes_enviados = 0;
  while (bytes_no_enviados > 0 && bytes_enviados != ERROR){
    bytes_enviados = send(self->fd,&buffer[bytes_enviados],length - (size_t)bytes_enviados,MSG_NOSIGNAL);
    if (bytes_enviados != ERROR){
      bytes_no_enviados = bytes_no_enviados - (size_t)bytes_enviados;
    }
  }
  return (bytes_enviados == ERROR? ERROR:EXITO);
}

int socket_receive(socket_t* self, int (*socket_callback)(char* chunk,void* callback_ctx),void*callback_ctx){
  ssize_t bytes_recibidos = 0;
  bool termine = false;
  ssize_t resultado_recv = 0;
  char buffer[TAMANIO_RESPUESTA];
  size_t length = TAMANIO_RESPUESTA;
  while (!termine && resultado_recv!= ERROR){
    resultado_recv = recv(self->fd,buffer,length - (size_t)bytes_recibidos - 1,0);
    bytes_recibidos = resultado_recv;
    buffer[bytes_recibidos] = 0;
    socket_callback(buffer,callback_ctx);
    if (bytes_recibidos == (size_t)length - 1){
      bytes_recibidos = 0;
    }
    if (resultado_recv == 0){
      termine = true;
    }
  }
  printf("\n");
  return EXITO;
}

/*
int socket_receive(socket_t* self, char* buffer, size_t length){
  ssize_t bytes_recibidos = 0;
  bool termine = false;
  ssize_t resultado_recv = 0;
  while(!termine && resultado_recv!= ERROR){
    resultado_recv = recv(self->fd,buffer,length - (size_t)bytes_recibidos - 1,0);
    bytes_recibidos = resultado_recv;
    buffer[bytes_recibidos] = 0;
    printf("%s",buffer);
    if(bytes_recibidos == (size_t)length - 1){
      bytes_recibidos = 0;
    }
    if(resultado_recv == 0){
      termine = true;
    }
  }
  printf("\n");
  return EXITO;
}
*/