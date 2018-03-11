/*
Zaliczenie SO - Król Grzegorz
czytelnik producent semafory (priorytet dla piszacych)
pamiec wspoldzielona jako bufor czytania/pisania
*/
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //fork exec
#include "api.h"

int pisarz_id,czytelnik_id;
key_t key;
int shm_id,sem_id;

void deallock(){
	puts("main czeka na smierc dzieci");
	errno=0;
	if(wait(NULL)==-1&&errno==ECHILD )
		printf("wait error brak dzieci",errno);
	errno=0;
	if(wait(NULL)==-1&&errno==ECHILD)
		printf("wait error brak dzieci",errno);
	puts("wszystkie dzieci martwe\n koniec programu");
	//sprzatanie
	clear_shm_id(shm_id);
	clear_sem_id(sem_id);
	exit(0);
}
void force_deallock(){
	puts("\nmain wymusza smierc dzieci");
	//if( kill(pisarz_id, SIGINT)==-1);
	//	puts("katastrofa nie można zabić dziecka");
	//if( kill(czytelnik_id, SIGINT)==-1);
	//	puts("katastrofa nie można zabić dziecka");
	errno=0;
	if(wait(NULL)==-1&&errno==ECHILD )
		printf("wait error brak dzieci",errno);
	errno=0;
	if(wait(NULL)==-1&&errno==ECHILD)
		printf("wait brak dzieci",errno);
	puts("wszystkie dzieci martwe\n koniec programu");
	//sprzatanie
	clear_shm_id(shm_id);
	clear_sem_id(sem_id);
	exit(0);
}

int main()
{
	go_out(&force_deallock);	
	//tworzenie pamieci wspoldzielonej
	key=get_key();
	shm_id=create_shm_id(key);
	sem_id=create_sem_id(key);
	
	printf("shm_id = %d \n", shm_id);
	printf("sem_id = %d \n", sem_id);
//	int sem_key = semget();

//wywolanie procesow
	if (pisarz_id=fork()==0)
	{
		//wywolanie pisarza
		puts("utworzono pisarza");
		execl("./pisarz", "pisarz", NULL);
	}
	else if(pisarz_id<0){
		puts("fork pisarz error");
		return 1;
	}
	
	if (czytelnik_id=fork()==0)
	{
		//wywolanie czytelnika
		puts("utworzono czytelnika");
		execl("./czytelnik", "czytelnik", NULL);
	}
	else if(czytelnik_id<0){
		puts("fork pisarz error");
		return 1;
	}
	
	int exit;
	
	deallock();
	return 0;
}