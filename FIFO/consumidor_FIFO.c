/**
 * Sistemas Operativos 2.
 * Práctica 4. Sincronización de procesos con paso de mensajes.
 *             Programa del consumidor con cola FIFO.
 *
 * @date 30/04/2024
 * @authors Barba Cepedello, Luis
 * @authors Vidal Villalba, Pedro
 */


#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <mqueue.h>
#include "params.h"


/**
 * @brief Función a ejecutar por el consumidor.
 *
 * @details
 * El consumidor consumirá NUM_ITEMS elementos de un buffer compartido,
 * enviando mensajes de confirmación al productor cada vez que lo haga.
 */
void consumer(void);

/**
 * Retira el último elemento de la cola de mensajes.
 *
 * @param control   Puntero al mensaje de control a enviar (ACK).
 * @param priority  Prioridad del item extraido. Útil para estudiar el comportamiento FIFO.
 *
 * @return	Carácter retirado del buffer.
 */
char remove_item(char* control, unsigned int* priority);

/**
 * Muestra por pantalla el elemento consumido.
 *
 * @param item	    Último valor leído del stack.
 * @param position  Posición del item dentro de los 100 items.
 */
void consume_item(char item, unsigned int position);

/**
 * Se asegura que el buffer quede sin elementos.
 * 
 * @param buffer    Cola de mensajes a vaciar.
*/
void clear_buffer(mqd_t buffer);

/* Cola de entrada de mensajes para el productor: Buffer de items */
mqd_t storage1;
/* Cola de entrada de mensajes para el consumidor: Buffer de control (ACKs) */
mqd_t storage2;

unsigned int sleep_time;

int main(int argc, char** argv) {
    struct mq_attr attr; /* Atributos de la cola */

    if (argc != 2) {
        printf("Número de parámetros incorrecto.\nUso: %s <sleep_time>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    sleep_time = atoi(argv[1]);


    attr.mq_maxmsg = N;
    attr.mq_msgsize = sizeof (char);

    /* Apertura de los buffers */
    storage1 = mq_open(STORAGE1, O_CREAT | O_RDONLY, 0777, &attr);
    storage2 = mq_open(STORAGE2, O_CREAT | O_WRONLY, 0777, &attr);

    if ((storage1 == -1) || (storage2 == -1)) fail("ERROR: Fallo al abrir los buffers");

    consumer();

    mq_close(storage1);
    mq_close(storage2);

    mq_unlink(STORAGE1);
    mq_unlink(STORAGE2);

    exit(EXIT_SUCCESS);
}


char remove_item(char* control, unsigned int* priority) {
    char message;
    mq_receive(storage1, &message, sizeof(char), priority);
    mq_send(storage2, control, sizeof(char), 0);    /* Enviar ACK */
    return message;
}

void consume_item(char item, unsigned int position) {
    consumer_printf("Eliminado el item "bold("%c")" del buffer (número de elemento: "bold("%2d")")\n", item, position);
}

void consumer(void) {
    int item, i;
    unsigned int position;
    char control = '?';

    for (i = 0; i < N; i++) mq_send(storage2, &control, sizeof(char), 0);

    for (i = 0; i < NUM_ITEMS; i++) {
        sleep(sleep_time);
        item = remove_item(&control, &position);
        consume_item(item, NUM_ITEMS - position);
    }
    clear_buffer(storage1);
}

void clear_buffer(mqd_t buffer) {
    struct mq_attr attr, old_attr;
    char trash;
    int i;

    /* Recuperamos los atributos del buffer */
    mq_getattr(buffer, &attr);
    /* Transformar el buffer en no bloqueante para poder vaciarlo */
    attr.mq_flags |= O_NONBLOCK;
    mq_setattr(buffer, &attr, &old_attr);

    for (i = 0; i < attr.mq_curmsgs; i++) {
        mq_receive(buffer, &trash, sizeof(char), NULL);
    }

    mq_getattr(buffer, &attr);
    consumer_printf("Número de elementos restantes en el buffer de producción: "bold("%ld")"\n", attr.mq_curmsgs);

    /* Volvemos a dejar el buffer como estaba */
    mq_setattr(buffer, &old_attr, NULL);
}
