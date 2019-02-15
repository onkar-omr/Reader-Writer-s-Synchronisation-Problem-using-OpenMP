/*

Author : Onkar Randive
		 G01036553

ECE-612 Real Time Embedded Systems 
Assignment 1		 
Readers-Writers Synchronisation Problem in C using OpenMP

Descrption: The code implements the Classic Readers-Writers Synchronisation Problem in C using the <omp.h> library for OpenMP. 
There are 5 threads which are randomly selected to be readers or writers using the rand() function. OMP.h library functions are used to 
create parallel threads	and omp locks are used to lock the shared resource between these threads.

Note : Please compile using gcc G01036553.c  -fopenmp 
	*/ 

#include<stdio.h>
#include<omp.h>																//OpenMP Library file
#include<unistd.h>
#include<stdlib.h>

int main()
{
		
		int num,reader_count=0;												//num is used to generate random binary bits(0 0r 1)
		omp_lock_t r_lock,w_lock;											//Locks used to lock threads from using the shared resources
		omp_init_lock(&r_lock);												//Lock initialisation : When initialised, locks are set to unset by default. 
		omp_init_lock(&w_lock);												// Writer Lock initialised
		omp_set_num_threads(5);												//Number of threads to be generated is passed as an argument to this function.
		
		#pragma omp parallel												// Preprocessor directive indicating parallel thread execution
		{
			num=rand()%2;													// Bits 0 or 1 random generated
			if (num==1)							
			{
			if(reader_count==0)												//For the first reader
			{
			printf("\nReader %d : is trying to access the database ",omp_get_thread_num());
			omp_set_lock(&r_lock);											// Reader lock id set for the thread
			sleep(2);
			printf("\nReader %d : is Reading ",omp_get_thread_num());
			sleep(2);
			reader_count++;
			printf("\nReader %d : has Completed Reading : Leaving ",omp_get_thread_num());
			omp_unset_lock(&r_lock);										// Reader Lock is unset.
			reader_count--;
			}
			else
			{																// For the next readers
			printf("\nReader %d : is trying to access the database ",omp_get_thread_num());
			sleep(2);
			omp_test_lock(&r_lock);											// test_lock is used check for the lock and if available sets it 																	but does not stop due to 
			printf("\nReader %d : is Reading ",omp_get_thread_num());		//	but does not stop execution due to non availability
			reader_count++;
			printf("\nReader %d : has Completed Reading : Leaving ",omp_get_thread_num());
			sleep(2);
			omp_unset_lock(&r_lock);
			reader_count--;
			}
			}
			else
			{
			printf("\nWriter %d : is trying to access the database",omp_get_thread_num());
			sleep(2);
			omp_set_lock(&r_lock);											// No reader as well as writer allowed during writing process. 
			omp_set_lock(&w_lock);											//Reader and Writer Locks set.
			printf("\nWriter %d : Writing",omp_get_thread_num());
			sleep(2);
			printf("\nWriter %d : Writing Completed : Leaving",omp_get_thread_num());
			sleep(2);
			omp_unset_lock(&r_lock);
			omp_unset_lock(&w_lock);
			}
		}
		
		omp_destroy_lock(&r_lock);											// The locks are destroyed.
		omp_destroy_lock(&w_lock);
		printf("\n");
}