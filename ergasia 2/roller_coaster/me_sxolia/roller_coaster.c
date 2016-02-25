#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#define N 3

//elegxos toy unlock klp
//elegxos toy unlock ride gia thn periptosh opoy ginoyn dyo synexomena
pthread_mutex_t ride;    //energopoiei kathe epivath gia na arxisei th volta tou
pthread_mutex_t roller;
pthread_mutex_t mtx_pass;
pthread_mutex_t mtx_rc;
pthread_mutex_t msg_pass;
pthread_mutex_t msg_rc;
int count=0;

void *passenger();
void *roller_coaster();
int error_check(int check, int func);

int main(int argc, char *argv[]) {
	int number, wait_pas,i; 
	int check;
	char command[10];
	pthread_t p_rc, *p_pass;
	
	wait_pas = 0; //oi epivates pou menoun meta thn epivivash
	check = pthread_mutex_init(&ride,NULL);
	error_check(check,1);
	check = pthread_mutex_init(&roller,NULL);
	error_check(check,1);
	check = pthread_mutex_init(&mtx_rc,NULL);
	error_check(check,1);
	check = pthread_mutex_init(&mtx_pass,NULL);
	error_check(check,1);
	check = pthread_mutex_init(&msg_pass,NULL);
	error_check(check,1);
	check = pthread_mutex_init(&msg_rc,NULL);
	error_check(check,1);
	
	check = pthread_mutex_lock(&roller);
	error_check(check,2);
	check = pthread_mutex_lock(&ride);
	error_check(check,2);
	check = pthread_mutex_lock(&mtx_rc);
	error_check(check,2);
	check = pthread_mutex_lock(&mtx_pass);
	error_check(check,2);
	check = pthread_mutex_lock(&msg_rc);
	error_check(check,2);
	
	check = pthread_create(&p_rc, NULL, &roller_coaster, NULL);
	if(check!=0) {
		printf("Problem to create roller_coaster thread\n");
		return(7);
    }
	while(1){

		printf("For new passengers press 'new' and the number of passengers\n");
		printf("Else for exit press 'exit'\n");
		printf("wait_pas: %d\n",wait_pas);
		scanf(" %9s",command);
		
		if(strcmp(command,"new")==0){
			scanf("%d",&number);
			wait_pas = wait_pas + number;
			
			p_pass = (pthread_t *)malloc(sizeof(pthread_t)*number);
			if(p_pass==NULL){                                        //elegxos epityxias ths malloc
				printf("Problem with memory allocation\n");
				return(2);
			}
			for(i=0;i<number;i++){
				check = pthread_create(&p_pass[i], NULL, &passenger, NULL);
				if(check!=0) {
					printf("Problem to create %d thread\n", i);
					free(p_pass);
					return(7);
				}
			}
			free(p_pass);
		}
		else if(strcmp(command,"exit")==0){
			printf("Roller coaster has closed for today!\n"); //FREE THREADS
			break;
		}
		else{ 
			printf("Try again!\n");
			continue;
		}

		while(wait_pas>=N){
			check = pthread_mutex_unlock(&roller);
			error_check(check,3);
			check = pthread_mutex_lock(&mtx_rc);
			error_check(check,2);
			
			for(i=0;i<N;i++){
				check = pthread_mutex_unlock(&ride);
				error_check(check,3);
				check = pthread_mutex_lock(&mtx_pass);
				error_check(check,2);
			}
			check = pthread_mutex_lock(&mtx_rc);
			error_check(check,2);
			wait_pas = wait_pas - N;
			
		}
		
		
	}
	
	return (0);
}

void *roller_coaster(){
	int check;
	while(1){
		check = pthread_mutex_lock(&roller);
		error_check(check,2);
		sleep(1);
		printf("The roller coaster begin!\n");
		check = pthread_mutex_unlock(&mtx_rc);
		error_check(check,3);
		check = pthread_mutex_lock(&msg_rc);
		error_check(check,2);
		printf("Passengers abord the roller_coaster!\n");
		sleep(1);
		printf("The roller coaster is ready for new ride!\n");
		check = pthread_mutex_unlock(&mtx_rc);
		error_check(check,3);
	}
	return (NULL);
}

void *passenger(){
	int check;
	
	check = pthread_mutex_lock(&ride);
	error_check(check,2);
	check = pthread_mutex_unlock(&mtx_pass);
	error_check(check,3);
	
	check = pthread_mutex_lock(&msg_pass);
	error_check(check,2);
	count++;
	printf("passenger : %d \n", count);
	if(count%N==0){
		check = pthread_mutex_unlock(&msg_rc);
		error_check(check,3);
	}
	check = pthread_mutex_unlock(&msg_pass);
	error_check(check,3);
	return (NULL);
}

int error_check(int check, int func){
    if (check!=0){
        if(func==1){
            printf("Problem with pthread_mutex_init\n");
        }
        else if (func==2){
            printf("Problem with pthread_mutex_lock\n");
        }
        else if (func==3){
            printf("Problem with pthread_mutex_unlock\n");
        }
        else{
            printf("Problem with pthread_mutex_destroy\n");
        }
        exit(1);
    }
    else{
        return(0);
    }
}