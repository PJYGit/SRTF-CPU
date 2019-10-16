/*
	I wrote the queue by myself in order to make it much
	easier for the using in this program.
	However, some parts of the code may seem similar to the 
	code in the course "Data Structure" which I took in my 
	home university since I'm an exchange student.
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_HARD_TIMES 10
typedef int ExcType;

typedef struct Queue
{
	int ready_time;
	int ID;
	int running_time;
	int num_exc;				// #times of exchanging with the hard drive
	ExcType* exc;				// an array for storing how long the exchanging will be

	int time_of_exc;			// how many exchanging times left
	int now_running_time;
	int remaining_time;
	int total_ready_time;
	int now_blocked_time;
	int total_blocked_time;

	struct Queue* Next;
}PQUEUE;

// add an element to the queue with values
void insert(PQUEUE* f, int readyT, int id, int runningT, int num, ExcType* excArray);
// add an element to the queue with address
void insertAdress(PQUEUE* p, PQUEUE* node);
// sort the queue according to "remaining_time"
void sort(PQUEUE* f);

// check if there is a process ready in the new queue
int checkNew(PQUEUE* p, int ready);
// pop the ready process from the new queue
PQUEUE* popNewQueue(PQUEUE* p, int ready);
// pop the top
PQUEUE* popQueue(PQUEUE* p);
// pop the running process from the running queue
PQUEUE* popRunQueue(PQUEUE* p, int id);

// check if the blocked time is enough
int checkOutBlocked(PQUEUE* p, int hard);
// check if a process need to be blocked
int checkInBlocked(PQUEUE* p);

// check if a process is done
int checkFinished(PQUEUE* p);

// method for add ready time for processes in the ready queue
void addReadyTime(PQUEUE* p);
// method for add "total_blocked_time" for the processes in the blocked queue
void addTotalBT(PQUEUE* p);

// method for calculating the total running time of process 0
int zeroTime(PQUEUE* p);

// method for print the message for the queue, which was used for manual debug
void printQueue(PQUEUE* p);