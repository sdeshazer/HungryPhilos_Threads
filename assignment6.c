/*
Samantha deshazer
Assignment 6
CS360 SP

note:
be sure to show which chopsticks are used.
*/
#define _GNU_SOURCE
#include <pthread.h> // pthread
#include <stdio.h>
#include <string.h>  // strerror
#include <stdlib.h>  // exit()
#include <unistd.h>  // sleep
#include <stdbool.h> // thinking boolean
#include <math.h>    // time
#include <errno.h>   // perror()
#include "random.h"


#define PHILOSOPHERS 5
#define MEAN_EAT_TIME 9
#define MEAN_THINKING_TIME 11
#define EAT_TIME 100
#define THINK_DEV 7
#define EAT_DEV 3
#define TIME 100

// globals:
pthread_mutex_t pChopsticks[PHILOSOPHERS];
int philoNum[PHILOSOPHERS] = {0,1,2,3,4};

// ---------------------------------------
// used to establish thinking and eating
// and keep track of sleep times:
void *tPhilo(void *);
bool pickupChopsticks(int);

// ------------------------------
int main(int argc, char *argv[]){
	// thread for every philo:
	pthread_t pPhilos[PHILOSOPHERS];
	int i;
	for(i=0;i<PHILOSOPHERS;i++){
		// sets the value:
		if ( pthread_mutex_init(&pChopsticks[i],NULL) == -1 ){
			perror(strerror(errno));
		}//if
	}//for

	for(i=0;i<PHILOSOPHERS;i++){
		if ( pthread_create(&pPhilos[i], NULL, tPhilo, philoNum) != 0){
			perror(strerror(errno));
		}//if
	} //for

	for(i=0;i<PHILOSOPHERS;i++){
		if(pthread_join(pPhilos[i], NULL) != 0){
			perror(strerror(errno));
		}//if
	}//for

	for(i=0;i<PHILOSOPHERS;i++){
		pthread_mutex_destroy(&pChopsticks[i]);
	}//for
	pthread_exit(NULL);
	return 0;
}// main

// ------------------------
//    tPhilo
// ------------------------
void *tPhilo(void *philo){
	int totalEatTime = 0;
	int totalThinkTime = 0;
	int i = *((int *) philo);
	// additional count, right and left chopsticks
	int j, right, left;
	bool thinking = true;

	printf("Philosopher %d sits at the table\n", i);

	// loop of thinking/eating
	while(totalEatTime < TIME){
		if (thinking == true){
			// ---------------------
			//         THINK
			// ---------------------
			int timeThinking = randomGaussian(MEAN_THINKING_TIME,THINK_DEV);
			if(timeThinking<0)timeThinking=0;
			totalThinkTime += timeThinking;
			printf("Philosopher %d is thinking\n", i);
			sleep(timeThinking);
			thinking = false;
		}else{
			// ---------------------
			//         EAT
			// ---------------------
			// get right chopstick
			if (pickupChopsticks(i) == true) {
				int timeEating = randomGaussian(MEAN_EAT_TIME,EAT_DEV);
				if(timeEating<0)timeEating=0;
				totalEatTime += timeEating;
				printf("[ philospher %d ] eats for %d seconds \n",i,timeEating);\
				printf("current total time spent eating: [ %d seconds ]\n", totalEatTime);
				sleep(timeEating);
				// release chopsticks after eating:
        int left = i;
				int right = (left+1)%PHILOSOPHERS;
				printf("Philosopher %d put down chopsticks %d and %d.\n", i, left, right);
				pthread_mutex_unlock(&pChopsticks[i]);
				pthread_mutex_unlock(&pChopsticks[right]);
				thinking = true;
			}//if
		}// else
	}// while
	printf("--------------------------------\n");
	printf("[ Philosopher %d is full. ]\n", i);
	printf("[ Total time spent thinking: %d ]\n", totalThinkTime);
	printf("--------------------------------\n");
}// philo
// --------------------------
// pickupChopsticks
// --------------------------
bool pickupChopsticks(int i){

	int right, left;
	if(i < PHILOSOPHERS){
		right = i;
	}else{
		right = 0;
	}// else
	if(i < PHILOSOPHERS){
		left = (i + 1) % PHILOSOPHERS;
	}else{
		left = PHILOSOPHERS-1;
	}// else
	// shall return zero if a lock on the mutex object referenced by
	// mutex is acquired. Otherwise, an error number is returned:
	if (!pthread_mutex_trylock(&pChopsticks[left])) {
		// left lock
		if (!pthread_mutex_trylock(&pChopsticks[right])) {
			// right lock
			return true;
		} else {
			// unlock left
			pthread_mutex_unlock(&pChopsticks[left]);
		}// else
	}// if
	return false;
}// pickupChopsticks
