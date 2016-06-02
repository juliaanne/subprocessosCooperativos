/* Disciplina: Sistemas Operacionais 1 */
/* Prof.: Valeria Bastos */
/* Primeiro trabalho */
/* Julia Anne */
/* Tayssa Vandelli */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>

#define N 3 // Numero de atendentes
#define M 5 // Numero de clientes

int main(int argc, char *argv[]) {
	int i; 
	pthread_t threads[NTHREADS];

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond_cons, NULL);
	pthread_cond_init(&cond_prod, NULL);

	for(i=0; i<NTHREADS/2; i++){
		pthread_create(&threads[i], NULL, Produtora, NULL);
	}
	
	for(i=NTHREADS/2; i<NTHREADS; i++){
		pthread_create(&threads[i], NULL, Consumidora, NULL);
	}

	for (i = 0; i < NTHREADS; i++) {
		pthread_join(threads[i], NULL);
	}
	printf ("*\nFIM\n");
	
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond_cons);
	pthread_cond_destroy(&cond_prod);
	
	return 0;
}