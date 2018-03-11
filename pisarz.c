#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //fork exec
#include "api.h"

//zmienne do wspoldzielenia
key_t key;
int shm_id,sem_id;
DataBase* baza;
int end=0;

void exit_f(){
	end=1;
}

int main(){
//PISARZ
//przygodowanie polaczenia	
	go_out(&exit_f);
	puts("start pisarza");
	key=get_key();
	shm_id=get_shm_id(key);
	printf("P shm_id = %d \n", shm_id);	
	baza=connect_shm(shm_id);
	printf("P baza = %p \n", baza);
	sem_id=get_sem_id(key);
	printf("P sem_id = %p \n", sem_id);
	
//wlasciwa czesc	
	//(*baza).number = 5;
	int product;
	int tmp;
	for(int i = 0; i < 100 && !end ; i++){
		product=i;
	
		sem_down(sem_id,mutex_w);
			(*baza).writers_count++;
		sem_up(sem_id,mutex_w);
		
		sem_down(sem_id,db);
			sem_down(sem_id,mutex_w);
				(*baza).writers_count--;
			sem_up(sem_id,mutex_w);
			(*baza).number=product;
			printf("P wpisuje %d do bazy\a\t",i);
						
			fflush(stdout);
			sleep(2);
			printf("koniec zapisu\a\n");
			fflush(stdout);	
		sem_up(sem_id,db);
		
		
		sleep(6);
	}
	/*
	int tmp;
	tmp=semctl(sem_id,db,GETVAL,tmp);
	printf(" P before val sem(db)= %d \n",tmp);
	sem_down(sem_id,db);
	tmp=semctl(sem_id,db,GETVAL,tmp);
	printf(" P inner val sem(db)= %d \n",tmp);
	sleep(10);
	sem_up(sem_id,db);
	tmp=semctl(sem_id,db,GETVAL,tmp);
	printf(" P after val sem(db)= %d \n",tmp);
	*/
//sprzatanie	
	disconnect_shm(baza);
	puts("smierc pisarza");
	fflush(stdout);	

	exit(0);
}