#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#define N 10

int wait_pas = 0;

void *passenger();
void *roller_coaster();

int main(int argc, char *argv[]) {
	int check, k, i;
	int number;
	char command[10];
	pthread_t p_rc, *p_pass;
	
	k = 0;
	
	synch(main);
	
	synch(roll);
	
	synch(pass);
	
	check = pthread_create(&p_rc, NULL, &roller_coaster, NULL);
	if(check!=0) {
		printf("Problem to create roller_coaster thread\n");
		
		return(7);
    }
    
    while(1) {
		printf("For new passengers press 'new' and the number of passengers\n");
		printf("Else for exit press 'exit'\n");
		printf("wait_pas: %d\n",wait_pas);
		scanf(" %9s",command);
		
		if(strcmp(command,"new")==0){
			scanf("%d",&number);
			wait_pas = wait_pas + number;
			
			p_pass = (pthread_t *)malloc(sizeof(pthread_t)*number);
			if(p_pass==NULL){                                        
				printf("Problem with memory allocation\n");

				return(2);
			}
			for(i=0;i<number;i++){
				
				synch_begin(main);
				
				k++;
				
				check = pthread_create(&p_pass[i], NULL, &passenger, &k);
				if(check!=0) {
					printf("Problem to create %d thread\n", i);
					free(p_pass);
					
					return(7);
				}
	
				synch_wait();
				
				synch_end(main);
			}
			free(p_pass);
		}
		else if(strcmp(command,"exit")==0){
			printf("Roller coaster has closed for today!\n");
			break;
		}
		else{ 
			printf("Try again!\n");
			continue;
		}
		printf("wait_pas: %d\n",wait_pas);
		
		synch_begin(roll);
		
		synch_notify();
		
		synch_end(roll);
		
		synch_begin(main);
		
		synch_wait();
		
		synch_end(main);
	}
    
    return (0);
}

void *roller_coaster(){
	int i;
	int count;
	
	count = 0;
	while(1) {
		
		synch_begin(roll);
	
		synch_wait();
		
		synch_end(roll);
		
		while(wait_pas>=N){
			printf("The roller coaster is ready for new ride!\n");
			for(i=0; i<N; i++) {
				count++;
				
				synch_begin(pass);
				
				synch_notify();
				
				synch_end(pass);
			}
			wait_pas = wait_pas - N;
			sleep(1);
			printf("The roller coaster begin!\n");
			sleep(2);
			printf("Passengers abord the roller_coaster!\n");
			sleep(1);
		}
		
		synch_begin(main);
		
		synch_notify();
		
		synch_end(main);
	}
	
	return (NULL);
}


void *passenger(void *num_p){
	int k;
	
	synch_begin(main);
	
	k = *(int *)(num_p);
	
	synch_notify();
	
	synch_end(main);
	
	synch_begin(pass);
	
	synch_wait();
	
	synch_end(pass);
	
	printf("i am passenger %d\n", k);
	
	return (NULL);
}