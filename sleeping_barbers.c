#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

void *customer_staff(int);
void *barber();
void randwait(int);
sem_t wake_barber;		//to check whether barber is sleeping or not
sem_t waiting_chair;		//to check whether a chair is empty in waiting room
sem_t seatbelt;			//to make the customer wait in the barber chair while his/her hair is being done
sem_t barber_chair;		//to check whether barber chair is empty or not
int over=0;			//to check if all customers are done

int main()
{
	int i,j;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	long RandSeed; 
	printf("\nEnter the no. of barbers employed ");	//no.of barbers
	int no_barber;
	scanf("%d",&no_barber);
	printf("\nEnter the number of customers(n)  ");	//no.of customers
	int no_customer;
	scanf("%d",&no_customer);
	sem_init(&waiting_chair, 0, no_customer); 
	sem_init(&barber_chair, 0, no_barber);
	sem_init(&wake_barber, 0, 0); 
	sem_init(&seatbelt, 0, 0); 
	srand48(RandSeed); 
	pthread_t barber_tid[no_barber],staff_tid[no_customer],student_tid[no_customer];	//tid for threads of staff,students and barber
	for(i=0;i<no_barber;i++)
	{
		pthread_create(&barber_tid[i],NULL,(void *)barber,(void *)i);			//creating barber thread
		
	}
	for(i=0;i<no_customer;i++)
	{
		pthread_create(&staff_tid[i],NULL,(void *)customer_staff,(void *)i);		//craeting customer threads
	}
	/*for)j=0;j<n;j++)
	{
		pthread_create(&staff_tid[i],NULL,cusomer_student,NULL);
	}*/
	for(i=0;i<=no_customer;i++)	
	{
		pthread_join(staff_tid[i],NULL);			//joining back the customer threads
		printf("\ncustomer %d thread joined\n",i);		
	}
	//printf("\ndone\n");
	over=1;	
	for(i=0;i<no_barber;i++)					//if all the customers are done ,
	{
		//sem_post(&wake_barber);				//wake the barber and
		pthread_join(barber_tid[i],NULL);			//join the barber thread
	}
	return 0;
}

void *customer_staff(int i)
{
	while(!over)
	{
		sem_wait(&waiting_chair);
		printf("\nCustomer %d entering the waiting room",i);
		sem_wait(&barber_chair);
		printf("\nCustomer %d waking up the barber",i);
		sem_post(&wake_barber);
		sem_wait(&seatbelt);
		sem_post(&barber_chair);
		printf("\nCustomer %d leaving the shop",i); 
	}
	pthread_exit(0);
}
		
void *barber(int i)
{
	while(!over)
	{
		if(!over)
		{	
			printf("\nBarber %d is sleeping",i);
			sem_wait(&wake_barber);
		
			printf("\nBarber %d is cutting hair",i);
			randwait(1);
			printf("\nBarber %d has finished cutting the hair",i);
			sem_post(&seatbelt);
		}
	
		else
		{
			printf("\nThe barber %d is done for the day",i);
		}
	}
	pthread_exit(0);
}		
	
	
void randwait(int secs)
{ 
	int len; // Generate a random number... 
	len = (int) ((drand48() * secs) + 1); 
	sleep(len); 
} 	
	
