#ifndef PARAMS_H
#define PARAMS_H

#include <unistd.h>
#include <sys/types.h>

#define N 5             /* Tamaño del buffer */
#define NUM_ITEMS 100   /* Número de items a generar por cada productor */

#define STORAGE1    "/storage1"     /* Nombre de la primera cola de mensajes */
#define STORAGE2    "/storage2"     /* Nombre de la segunda cola de mensajes */

/* Macro para imprimir mensajes de fallo */
#define fail(message)  { perror(message); exit(EXIT_FAILURE); }

#define PRODUCER_COLOR  "\x1b[36m"
#define CONSUMER_COLOR  "\x1b[31m"
#define NO_COLOR        "\x1b[0m"
#define bold(text) "\x1b[1m" text "\x1b[22m"

#define producer_printf(format, ...) ( printf(PRODUCER_COLOR "[%d] " format NO_COLOR, getpid(), ##__VA_ARGS__) )
#define consumer_printf(format, ...) ( printf(CONSUMER_COLOR "[%d] " format NO_COLOR, getpid(), ##__VA_ARGS__) )

#endif //PARAMS_H
