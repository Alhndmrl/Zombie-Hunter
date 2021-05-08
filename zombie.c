/*Alihan Demirel
 */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

/*number of zombies in the game: 
you should synchronize threads editing this variable*/
int zombieCounter;
int deadZombie;

/* Keeps track of number of zombies entered.*/
void zombieEntered(){
	++zombieCounter;

}
/* Keeps track of number of zombies killed.*/
void zombieKilled(){
	++deadZombie;	
	if(zombieCounter != 0){
		--zombieCounter;
	}
}

/* Returns true if number of zombies in the room are 
greater than or equal to 100.*/
int tooManyZombiesInTheRoom(){
	if(zombieCounter >= 100){
       
       return 1;
    }
    return 0;
}

/*Returns true if more than 100 zombies have been killed.*/

int killed100Zombies(){
	if(deadZombie > 100 ){
		return 1;
	}
	return 0;
}

/* Returns true if there is at least one zombies in the room.*/
int zombiesExist(){
	if(zombieCounter >= 1 ){
		return 1;
	}
	return 0;
}
/*Returns the number of zombies killed.*/
int getKilledCount(){
	
	return deadZombie;
}

/* Returns the number of zombies in the room.*/
int getInTheRoomCount(){
	
	return zombieCounter;
}
/*doorman thread*/
void *doorMan(void *p){
	while(1){
		double chance =((double)rand()/RAND_MAX);
		if(chance >= 0.5 && chance <= 1.0){
			pthread_mutex_lock(&m);
			zombieEntered();
		}
		if(tooManyZombiesInTheRoom() ){
		
			printf("WİNNER ZOMBİE");
			exit(EXIT_FAILURE);
		}
		if(killed100Zombies()){
			
			printf("WİNNER HUMAN");
			exit(EXIT_FAILURE);
		}
		
		pthread_mutex_unlock(&m);
		usleep(2);
	}		
}

/*slayer thread*/
void *slayer(void *p){
	while(1){
	pthread_mutex_lock(&m);
	if( zombieCounter >= 0){
		zombieKilled();
		if(tooManyZombiesInTheRoom() ){
			
			printf("WİNNER ZOMBİE\n");
			exit(EXIT_FAILURE);
		}
		if(killed100Zombies()){
			
			printf("WİNNER HUMAN\n");
			exit(EXIT_FAILURE);
		}
			
		pthread_mutex_unlock(&m);
		usleep(2);
		}			
	}
}
/*simulator main thread*/
int main(int argc, char **argv){
	int x=atoi(argv[1]);
	pthread_t holdthedoor[x];
	pthread_t serialzombiekiller;
	int thread1,thread2;
	for(int i = 0; i < x ; ++i){
	thread1 = pthread_create(&holdthedoor[x],NULL,doorMan,NULL);
	if(thread1 != 0){
		perror("not create");
		exit(EXIT_FAILURE);
		}
	}
	
	thread2 = pthread_create(&serialzombiekiller,NULL,slayer,NULL);
	if(thread2 != 0){
		perror("not create");
		exit(EXIT_FAILURE);
		}
	
	pthread_join(serialzombiekiller,NULL);
	
	for(int i=0;i <x ;++i){
	
     pthread_join(holdthedoor[i],NULL);
     
		}
    }
    
    
    
