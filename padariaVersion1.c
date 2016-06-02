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
#define M 20 // Numero de clientes
// TODO: Pensar nos outros semáforos

sem_t em_retirarSenha, em_chamarCliente, atendentes, clientes;
int proximaSenha, proximoCliente;

void *Atendentes(void *arg){
	int pid = * (int *) arg;
	printf("Eu sou o atendente %d e comecei a trabalhar.\n", pid);

	while(1){
		// Pega senha do próximo cliente
		sem_wait(&em_chamarCliente);
		proximoCliente++;
		sem_post(&em_chamarCliente);

		// Chamo o proximo cliente
		sem_post(&clientes);
		
		// Atende próximo cliente
		// Finaliza atendimento
	}
}

void *Clientes(void *arg){
	int pid = * (int *) arg;
	printf("Eu sou o cliente %d e cheguei no estabelecimento.\n", pid);

	// Pegar minha senha
	sem_wait(&em_retirarSenha);
	proximaSenha++;
	sem_post(&em_retirarSenha);

	while(proximaSenha != proximoCliente){
		sem_post(&clientes);
	}

	// Cliente é atendido
	// Cliente vai embora
}

int main(int argc, char *argv[]) {
	int nthreads = N + M;
	pthread_t threads[nthreads];
	int i, *pid;
	proximaSenha=0;
	proximoCliente=0;

	// Inicializando os semáforos
	sem_init(&em_retirarSenha, 0, 1);
	sem_init(&em_chamarCliente, 0, 1);
	sem_init(&em_senha, 0, N);

	// Criando as threads ATENDENTES
	for (i = 0; i < N; i++){
		pid = malloc(sizeof(int));

		if(pid == NULL){
			printf("--ERRO: malloc() em alocação threads\n"); exit(-1);
		}
		*pid = i;

		pthread_create(&threads[i], NULL, Atendentes, (void *) pid);
	}

	// Criando as threads CLIENTES
	for (i = N; i < nthreads; i++){
		pid = malloc(sizeof(int));

		if(pid == NULL){
			printf("--ERRO: malloc() em alocação threads\n"); exit(-1);
		}
		*pid = i;

		pthread_create(&threads[i], NULL, Clientes, (void *) pid);
	}

	// Esperando todas terminarem
	for (i = 0; i < nthreads; i++) {
		pthread_join(threads[i], NULL);
	}

	return 0;
}