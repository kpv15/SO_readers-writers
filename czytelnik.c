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
//CZYTELNIK

//przygodowanie polaczenia
	go_out(&exit_f);
	puts("start czytelnika");
	key=get_key();
	shm_id=get_shm_id(key);
	printf("C shm_id = %d \n",shm_id);
	baza=connect_shm(shm_id);
	printf("C baza = %p \n", baza);
	sem_id=get_sem_id(key);
	printf("C sem_id = %p \n", sem_id);
//wlasciwa czesc	
	//printf("czytelnik number = %d\n",(*baza).number);
	DataBase loc_baza;
	char *znak="/-\\";
	int i=0;
	int odczyt;
	do{
		sem_down(sem_id,mutex_w);	//sprawdzam czy nie czekaja pisarze
		if (!(*baza).writers_count){
			sem_up(sem_id,mutex_w);
			
			sem_down(sem_id,mutex_r);//zwiekszamy liczbe czytelnikow
			//puts("jestem tutaj");
			(*baza).readers_count++;
			if( (*baza).readers_count==1)//jesli jestesmy pierwsi opuszczamy muteks na baze
				sem_down(sem_id,db);
			sem_up(sem_id,mutex_r);

	//		puts("C weszlem");
				odczyt=(*baza).number;
			sem_down(sem_id,mutex_r);//przestajemy czytac baze zmniejszamy liczbe czytelnikow
			(*baza).readers_count--;	
			if( (*baza).readers_count==0)//jesli bylismy ostatni to odblokowujemy baze
				sem_up(sem_id,db);
			sem_up(sem_id,mutex_r);
			printf("C odczytałem %d %c\r",odczyt,znak[i%3]);//publikujemy swuj odczyt
			fflush(stdout);
			i++;
		}else{//gdy sa pisarze w kolejce to nie mozemy odczytac bufora i podnosimy semafor by pisarze mogli zaktualizowac swoja liczbe
			puts("istnieja oczekujacy pisarze");
			sem_up(sem_id,mutex_w);
		}
		sleep(1);
		
	}while( !end );	
		
//sprzatanie
	disconnect_shm(baza);
	puts("\nsmierc czytelnika");
	fflush(stdout);	
	exit(0);
}