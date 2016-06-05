/* Disciplina: Sistemas Operacionais 1 */
/* Prof.: Valeria Bastos */
/* Exercício 7.b */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>

#define N 3 // Numero de atendentes
#define M 20 // Numero de clientes

sem_t clientes, foiChamado;
pthread_mutex_t mutex;
pthread_cond_t atendentes;
int proximoCliente, ultimoCliente;

void *Atendentes(void *arg){
	int i, pid = * (int *) arg;

	while(1){
		sem_wait(&clientes); // Espera ter clientes
		printf("Tem clientes na padaria!\n");

		pthread_mutex_lock(&mutex);
		printf("Eu sou o atendente %d e chamo o cliente %d!\n", pid, proximoCliente);

		pthread_cond_broadcast(&atendentes); // Acorda todos os clientes que esperam por atendentes chamarem
		
		sem_post(&foiChamado); // Avisa que foi chamado
		
		proximoCliente++; // Incrementa próximo cliente

		pthread_mutex_unlock(&mutex);
	}
}

void *Clientes(void *arg){
	int pid = * (int *) arg;

	pthread_mutex_lock(&mutex);
	while(pid > proximoCliente) {
		// Enquanto não for sua vez, espera na fila
		pthread_cond_wait(&atendentes, &mutex);
	}
	pthread_mutex_unlock(&mutex);

	sem_post(&clientes); // Avisa que tem cliente esperando
	
	sem_wait(&foiChamado); // Espera ser chamado pelo atendente
	printf("Eu sou o cliente %d e é minha vez\n", pid);	
}

int main(int argc, char *argv[]) {
	pthread_t atend[N];
	pthread_t cli[M];
	
	int i, *pid;
	proximoCliente=0;

	// Inicializando semáforos
	sem_init(&clientes, 0, 0);
	sem_init(&foiChamado, 0, 0);

	printf("Padaria abriu!\n");

	// Criando atendentes
	for (i = 0; i < N; i++){
		pid = malloc(sizeof(int));

		if(pid == NULL){
			printf("--ERRO: malloc() em alocação threads\n"); exit(-1);
		}
		*pid = i;

		pthread_create(&atend[i], NULL, Atendentes, (void *) pid);
	}

	// Criando clientes
	for (i = 0; i < M; i++){
		pid = malloc(sizeof(int));

		if(pid == NULL){
			printf("--ERRO: malloc() em alocação threads\n"); exit(-1);
		}
		*pid = i;

		pthread_create(&cli[i], NULL, Clientes, (void *) pid);
	}

	// Esperando todos os atendentes serem atendidos
	for (i = 0; i < M; i++) {
		pthread_join(cli[i], NULL);
	}

	printf("Padaria fechou!\n");

	// Destruindo
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&atendentes);

	return 0;
}