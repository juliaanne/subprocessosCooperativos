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
#define M 10 // Numero de clientes

sem_t  clientes, foiAtendido;
pthread_mutex_t mutex;
pthread_cond_t atendentes;
int proximoCliente, ultimoCliente;

void *Atendentes(void *arg){
	int i, pid = * (int *) arg;

	while(1){
		printf("Eu sou o atendente %d e estou esperando um cliente.\n", pid);
		sem_wait(&clientes);
		printf("Eu sou o atendente %d e tem cliente!\n", pid);

		pthread_mutex_lock(&mutex);
		printf("Eu sou o atendente %d e chamo o cliente %d!\n", pid, proximoCliente);
		pthread_cond_broadcast(&atendentes);
		sem_post(&foiAtendido);
		proximoCliente++;
		pthread_mutex_unlock(&mutex);
	}
}

void *Clientes(void *arg){
	int pid = * (int *) arg;

	

	pthread_mutex_lock(&mutex);
	while(pid != proximoCliente){
		//printf("Eu sou o cliente %d e NÃO é minha vez\n", pid);	
		pthread_cond_wait(&atendentes, &mutex);
	}
	pthread_mutex_unlock(&mutex);
	sem_post(&clientes);
	sem_wait(&foiAtendido);
	



	printf("Eu sou o cliente %d e é minha vez\n", pid);	
	

	printf("Eu sou o cliente %d e saí na padaria\n", pid);
	
}

int main(int argc, char *argv[]) {
	pthread_t atend[N];
	pthread_t cli[M];
	
	int i, *pid;
	// proximoCliente=0;
	ultimoCliente=0;

	sem_init(&clientes, 0, 0);
	sem_init(&foiAtendido, 0, 0);

	for (i = 0; i < N; i++){
		pid = malloc(sizeof(int));

		if(pid == NULL){
			printf("--ERRO: malloc() em alocação threads\n"); exit(-1);
		}
		*pid = i;

		pthread_create(&atend[i], NULL, Atendentes, (void *) pid);
	}

	for (i = 0; i < M; i++){
		pid = malloc(sizeof(int));

		if(pid == NULL){
			printf("--ERRO: malloc() em alocação threads\n"); exit(-1);
		}
		*pid = i;

		pthread_create(&cli[i], NULL, Clientes, (void *) pid);
	}

	for (i = 0; i < M; i++) {
		pthread_join(cli[i], NULL);
	}

	// DESTRUIR TUDO

	return 0;
}