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
 * Retira el último elemento del <u>stack</u>.
 *
 * @param message Variable donde se guarda el item producido por el productor
 * @param priority Prioridad del item extraido. Útil para estudiar el comportamiento LIFO.
 *
 * @return	Entero retirado del stack.
 */
char remove_item(char* control, unsigned int* priority);

/**
 * Muestra por pantalla el elemento consumido.
 *
 * @param item	Último valor leído del stack.
 * @param position Posición del item dentro de los 100 items.
 */
void consume_item(char element, unsigned int position);

/**
 * @brief Función a ejecutar por el consumidor.
 *
 * @details
 * El consumidor producirá un item en un stack compartido y, una vez hecho esto,
 * envía una senhal de confirmación al productor.
 */
void consumer(void);

/* cola de entrada de mensajes para el productor */
mqd_t storage1;
/* cola de entrada de mensajes para el consumidor */
mqd_t storage2;


int main(int argc, char** argv) {
    struct mq_attr attr; /* Atributos de la cola */

    attr.mq_maxmsg = N;
    attr.mq_msgsize = sizeof (char);

    /* Apertura de los buffers */
    storage1 = mq_open(STORAGE1, O_CREAT | O_RDONLY, 0777, &attr);
    storage2 = mq_open(STORAGE2, O_CREAT | O_WRONLY, 0777, &attr);

    if ((storage1 == -1) || (storage2 == -1)) {
        perror ("mq_open");
        exit(EXIT_FAILURE);
    }

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
    mq_send(storage2, control, sizeof(char), 0);
    return message;
}

void consume_item(char element, unsigned int position) {
    consumer_printf("Eliminado el item "bold("%c")" del buffer (número de elemento: %d).\n", element, position);
}

void consumer(void) {
    int element, i;
    unsigned int position;
    char control = '\0';

    for (i = 0; i < N; i++) mq_send(storage2, &control, sizeof(char), 0);

    for (i = 0; i < NUM_ITEMS; i++) {
        element = remove_item(&control, &position);
        consume_item(element, position);
    }
}