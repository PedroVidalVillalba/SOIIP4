/**
 * Sistemas Operativos 2.
 * Práctica 4. Sincronización de procesos con paso de mensajes.
 *             Programa del productor con cola FIFO.
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
 * @brief Función a ejecutar por el productor.
 *
 * @details
 * El productor producirá NUM_ITEMS items en un buffer compartido.
 */
void producer(void);

/**
 * Genera un entero aleatorio entre 0 y 10.
 *
 * @param iter  Número de iteración en la que se produce el item
 * @return  Caracter asociado a la iteración iter.
 */
char produce_item(int iter);

/**
 * Coloca el elemento item en el buffer.
 *
 * @param item	    Elemento a introducir en el buffer.
 * @param position  Posición del item dentro de los 100 items.
 */
void insert_item(char item, int position);

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

    /* Borrado de los buffers de entrada
    por si existían de una ejecución previa*/
    mq_unlink(STORAGE1);
    mq_unlink(STORAGE2);

    attr.mq_maxmsg = N;
    attr.mq_msgsize = sizeof (char);

    /* Apertura de los buffers */
    storage1 = mq_open(STORAGE1, O_CREAT | O_WRONLY, 0777, &attr);
    storage2 = mq_open(STORAGE2, O_CREAT | O_RDONLY, 0777, &attr);

    if ((storage1 == -1) || (storage2 == -1)) fail("mq_open");

    producer();

    mq_close(storage1);
    mq_close(storage2);

    mq_unlink(STORAGE1);
    mq_unlink(STORAGE2);

    exit(EXIT_SUCCESS);
}


char produce_item(int iter) {
    return 'a' + (iter % N);
}

void insert_item(char item, int position) {
    char control;

    mq_receive(storage2, &control, sizeof(char), NULL);
    mq_send(storage1, &item, sizeof(char), position);
}

void producer(void) {
    int i;
    char item;

    for (i = 0; i < NUM_ITEMS; i++) {
        item = produce_item(i);
        sleep(sleep_time);
        insert_item(item, NUM_ITEMS - i);
        producer_printf("Insertado el item "bold("%c")" en el buffer (número de elemento: "bold("%2d")")\n", item, i);
    }

    /* Vaciar los ACKs iniciales en el buffer de control */
    for (i = 0; i < N; i++) mq_receive(storage2, &item, sizeof(char), NULL);

    clear_buffer(storage2);
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

    /* Recibir tantos mensajes como queden en el buffer */
    for (i = 0; i < attr.mq_curmsgs; i++) {
        mq_receive(buffer, &trash, sizeof(char), NULL);
    }

    mq_getattr(buffer, &attr);
    producer_printf("Número de elementos restantes en el buffer de control: "bold("%ld")"\n", attr.mq_curmsgs);

    /* Volvemos a dejar el buffer como estaba */
    mq_setattr(buffer, &old_attr, NULL);
}
