#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h> // pamiec wspoldzielona
#include <sys/sem.h> // semafory
#include <signal.h>
//lista istniejacych mutexuw
typedef enum{
	db,
	mutex_w,
	mutex_r,
	number_of_sem,
}sem;
//baza danych
typedef struct {
	int number;
	int readers_count;
	int writers_count;
}DataBase;

key_t get_key(); 
//zarzadzanie pamiecia
int create_shm_id(key_t key);
int get_shm_id(key_t key);
DataBase* connect_shm(int id);
void disconnect_shm(DataBase* baza);
void clear_shm_id(int id);
//zarzadzanie semaforami
int create_sem_id(key_t key);
int get_sem_id(key_t key);
void sem_up(int id,sem sem_number);
void sem_down(int id,sem sem_number);
void clear_sem_id(int id);
//przechwycenie SIGINT
struct sigaction act;
void go_out(void (*fun)());
 