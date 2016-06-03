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

pthread_mutex_t em_senhaAtual, em_proximaSenha;
pthread_cond_t fila, clientes;

int proximaSenha, senhaAtual;

void *Atendentes(void *arg){
	int pid = * (int *) arg;
	printf("Eu sou o atendente %d e comecei a trabalhar.\n", pid);

	while(1){
		pthread_mutex_lock(&em_proximaSenha);
		while(proximaSenha == senhaAtual){
			printf("Eu sou o atendente %d e estou esperando um cliente.\n", pid);
			pthread_cond_wait(&clientes, &em_proximaSenha);
		}
		pthread_mutex_unlock(&em_proximaSenha);
		
		printf("Eu sou o atendente %d e tem um cliente!\n", pid);
		
		printf("Eu sou o atendente %d e estou chamando o cliente com senha %d.\n", pid, senhaAtual);
		pthread_cond_broadcast(&fila);

		pthread_mutex_lock(&em_senhaAtual);
		senhaAtual++;
		pthread_mutex_unlock(&em_senhaAtual);
		
		printf("Eu sou o atendente %d e terminei de atender.\n", pid);
	}
}

void *Clientes(void *arg){
	int minhaSenha, pid = * (int *) arg;
	printf("Eu sou o cliente %d e cheguei na padaria\n", pid);

	pthread_mutex_lock(&em_proximaSenha);
	minhaSenha = proximaSenha;
	proximaSenha++;
	pthread_mutex_unlock(&em_proximaSenha);
	printf("Eu sou o cliente %d e peguei a senha %d\n", pid, minhaSenha);

	pthread_cond_signal(&clientes);

	pthread_mutex_lock(&em_senhaAtual);
	while(minhaSenha != senhaAtual){
		printf("Eu sou o cliente %d com senha %d não é minha vez\n", pid, minhaSenha);
		pthread_cond_wait(&fila, &em_proximaSenha);
	}
	pthread_mutex_unlock(&em_proximaSenha);

	printf("Eu sou o cliente %d com senha %d e é a minha vez.\n", pid, minhaSenha);

	printf("Eu sou o cliente %d e fui atendido!\n", pid);

}

int main(int argc, char *argv[]) {
	pthread_t atendentes[N];
	pthread_t clientes[M];
	
	int i, *pid;
	proximaSenha=0;
	senhaAtual=0;

	for (i = 0; i < N; i++){
		pid = malloc(sizeof(int));

		if(pid == NULL){
			printf("--ERRO: malloc() em alocação threads\n"); exit(-1);
		}
		*pid = i;

		pthread_create(&atendentes[i], NULL, Atendentes, (void *) pid);
	}

	for (i = 0; i < M; i++){
		pid = malloc(sizeof(int));

		if(pid == NULL){
			printf("--ERRO: malloc() em alocação threads\n"); exit(-1);
		}
		*pid = i;

		pthread_create(&clientes[i], NULL, Clientes, (void *) pid);
	}

	for (i = 0; i < M; i++) {
		pthread_join(clientes[i], NULL);
	}

	// DESTRUIR TUDO

	return 0;
}