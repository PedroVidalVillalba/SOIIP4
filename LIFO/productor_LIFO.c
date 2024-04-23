/**
 * Sistemas Operativos 2.
 * Práctica 4. Sincronización de procesos con paso de mensajes.
 *             Programa del productor con cola LIFO.
 *
 * @date 23/04/2024
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
 * @return  Caracter asociado a la iteración <u>iter</u>.
 */
char produce_item(int iter);

/**
 * Coloca el elemento <u>item</u> en el buffer.
 *
 * @param item	Elemento a introducir en el buffer.
 * @param iter  Número de iteración.
 *
 */
void insert_item(char item, int iter);

/* Cola de entrada de mensajes para el productor */
mqd_t storage1;
/* Cola de entrada de mensajes para el consumidor */
mqd_t storage2;



int main(int argc, char** argv) {
    struct mq_attr attr; /* Atributos de la cola */

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

void insert_item(char item, int iter) {
    char control;

    mq_receive(storage2, &control, sizeof(char), NULL);
    mq_send(storage1, &item, sizeof(char), iter);
}

void producer(void) {
    int i;
    char item;

    for (i = 0; i < NUM_ITEMS; i++) {
        item = produce_item(i);
        insert_item(item, i);
        producer_printf("Insertado el item "bold("%c")" en el buffer (número de elemento: %d).\n", item, i);
    }
}