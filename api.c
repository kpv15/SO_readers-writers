#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //fork exec
#include "api.h"

key_t get_key(){
	key_t key;
	if ((key = ftok(".", 1)) == -1) {
		puts("blad tworzenia klucza");
		exit(1);
	}
	return key;
}
//funckje pamieci
int create_shm_id(key_t key){
	int shm_id;
	if ((shm_id = shmget(key, sizeof(DataBase), IPC_CREAT | IPC_EXCL | 0600)) == -1) {
		puts("blad tworzenia pamieci");
		exit(1);
	}
	DataBase* baza=connect_shm(shm_id);
	(*baza).readers_count=0;
	(*baza).writers_count=0;
	disconnect_shm(baza);
	return shm_id;
}

int get_shm_id(key_t key){
	int shm_id;
	if ((shm_id = shmget(key, sizeof(DataBase), 0600)) == -1) {
		puts("blad tworzenia pamieci2");
		exit(1);
	}
	return shm_id;
}

DataBase* connect_shm(int id){
	DataBase* baza;
	if ((baza = (DataBase*)shmat(id, NULL, 0)) == (void *) -1) {
		puts("P blad laczenia pamieci");
		exit(1);
	}
	return baza;
}

void disconnect_shm(DataBase* baza){
	shmdt(baza);
}

void clear_shm_id(int id){
	shmctl (id,IPC_RMID,0);
}

//funkcje semaforow
int create_sem_id(key_t key){
	int sem_id;
	if ( (sem_id = semget(key, number_of_sem, IPC_CREAT | IPC_EXCL | 0600)) == -1) {
		puts("blad tworzenia semafora");
		exit(1);
	}
	for(int i = 0 ; i < number_of_sem; i++)
		semctl(sem_id,i,SETVAL,1);
		int tmp;
	puts("wartosc sem po inicjalizacji");
	for(int i = 0 ; i < number_of_sem; i++){
		tmp=semctl(sem_id,i,GETVAL,tmp);
		printf(" val sem(%d)= %d \n",i,tmp);
	}
	return sem_id;
}

int get_sem_id(key_t key){
	int sem_id;
	if ((sem_id = semget(key, number_of_sem, 0600)) == -1) {
		puts("blad tworzenia semafora2");
		exit(1);
	}
	return sem_id;
}

void sem_up(int id,sem sem_number){
	struct sembuf temp;
	temp.sem_num=sem_number;
	temp.sem_op=1;
	temp.sem_flg=0;
	if(semop(id, &temp, 1)==-1){
		puts("blad podniesienia semafora");
		exit(1);
	}
}

void sem_down(int id,sem sem_number){
	struct sembuf temp;
	temp.sem_num=sem_number;
	temp.sem_op=-1;
	temp.sem_flg=0;
	if(semop(id, &temp, 1)==-1){
		puts("blad opuszczenia semafora");
		exit(1);
	}
}

void clear_sem_id(int id){
	semctl(id,0,IPC_RMID);
}

//przechwycenie SIGINT
void go_out(void (*fun)()){
	struct sigaction act;
	sigset_t iset;
	sigemptyset(&iset);
	act.sa_handler = fun;
	act.sa_mask = iset;
	act.sa_flags = 0;
	sigaction(SIGINT, &act, 0);
}

