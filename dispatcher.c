/*
dispatcher.c

Student Name : Jiayao Pang
Student ID # : 194174300

Dispatch Algorithm : Shortest Remaining Time
*/

#include <string.h>
#include <stddef.h>
#include <stdio.h>

#define MAX_LINE_LENGTH 100

/*
Any required standard libraries and your header files here
*/
#include "my_queue.h"
#define MAX_HARD_TIMES 10

void dispatcher(FILE *fd, int harddrive){
    /*
        Your code here.
        You may edit the following code
    */
    char line_buffer[MAX_LINE_LENGTH];
    int start_time, run_time, process_id, num_exchanges, exchange_time;
    char *token;

	// init all the queues
	PQUEUE* newQueue = (PQUEUE*)malloc(sizeof(PQUEUE));
	if (newQueue == NULL) {
		printf("fail to malloc!");
		exit(-1);
	}
	newQueue->Next = NULL;

	PQUEUE* readyQueue = (PQUEUE*)malloc(sizeof(PQUEUE));
	if(readyQueue == NULL){
		printf("fail to malloc!");
		exit(-1);
	}
	readyQueue->Next = NULL;

	PQUEUE* runQueue = (PQUEUE*)malloc(sizeof(PQUEUE));
	if (runQueue == NULL) {
		printf("fail to malloc!");
		exit(-1);
	}
	runQueue->Next = NULL;

	PQUEUE* blockedQueue = (PQUEUE*)malloc(sizeof(PQUEUE));
	if (blockedQueue == NULL) {
		printf("fail to malloc!");
		exit(-1);
	}
	blockedQueue->Next = NULL;

	PQUEUE* finishedQueue = (PQUEUE*)malloc(sizeof(PQUEUE));
	if (finishedQueue == NULL) {
		printf("fail to malloc!");
		exit(-1);
	}
	finishedQueue->Next = NULL;

	// define all the flags
	int num_finish = 0;
	int time = 0;
	int cur_proc = -1;			// actually -1 means the current process is process 0
	int one_ready = 0;
	int one_ready_time = 0;
	int one_blocked = 0;

    //Process simulation input line by line
	int job_counter = 0;
    while (fgets(line_buffer, MAX_LINE_LENGTH, fd) != NULL && line_buffer[0] != '\n'){
		ExcType* exc_mes = malloc((MAX_HARD_TIMES + 1) * sizeof(ExcType));

        token = strtok(line_buffer, " ");
        sscanf(token, "%d",&start_time);
       
        token = strtok(NULL, " ");
        sscanf(token, "%d",&process_id);
        
        token = strtok(NULL, " ");
        sscanf(token, "%d",&run_time);
        
        num_exchanges = 0;
        token = strtok(NULL, " ");
        while ( token != NULL ){
            num_exchanges += sscanf(token, "%d",&exchange_time);
			exc_mes[num_exchanges - 1] = exchange_time;
            token = strtok(NULL, " ");
        }
		job_counter++;
		// insert all the job messages into the new queue
		insert(newQueue, start_time, process_id, run_time, num_exchanges, exc_mes);
		free(exc_mes);
       }

	// while the jobs are not all finished
	while (num_finish != job_counter) {
		// check new queue -> ready queue
		if (checkNew(newQueue, time))
		{
			PQUEUE* new_ready = popNewQueue(newQueue, time);
			insertAdress(readyQueue, new_ready);
			sort(readyQueue);

			one_ready++;
			one_ready_time = time;
		}

		/* check the running queue */

		// one ready & current process is 0
		if (one_ready > 0 && cur_proc == -1)
		{
			// run the SRT job
			one_ready--;
			PQUEUE* new_run = popQueue(readyQueue);
			cur_proc = new_run->ID;
			
			insertAdress(runQueue, new_run);
			new_run->now_running_time++;
			new_run->remaining_time--;
		}

		// one ready & current process is not 0
		else if (one_ready > 0 && cur_proc != -1)
		{
			// judge if blocked
			if (checkInBlocked(runQueue)) {
				PQUEUE* blocked = popRunQueue(runQueue, cur_proc);
				PQUEUE* new_run = popQueue(readyQueue);

				insertAdress(runQueue, new_run);
				cur_proc = new_run->ID;
				new_run->now_running_time ++;
				new_run->remaining_time --;
				one_ready--;

				insertAdress(blockedQueue, blocked);
				one_blocked++;
			}
			// judge if finished
			else if (checkFinished(runQueue))
			{
				PQUEUE* finished = popRunQueue(runQueue, cur_proc);
				PQUEUE* new_run = popQueue(readyQueue);

				insertAdress(runQueue, new_run);
				cur_proc = new_run->ID;
				new_run->now_running_time ++;
				new_run->remaining_time --;
				one_ready--;

				insertAdress(finishedQueue, finished);
				num_finish++;
			}
			else {
				// judge if change proc
				if (one_ready_time == time && readyQueue->Next->remaining_time < runQueue->Next->remaining_time) {
					PQUEUE* old_run = popRunQueue(runQueue, cur_proc);
					PQUEUE* old_ready = popQueue(readyQueue);

					insertAdress(runQueue, old_ready);
					cur_proc = old_ready->ID;

					insertAdress(readyQueue, old_run);
					sort(readyQueue);

					runQueue->Next->now_running_time ++;
					runQueue->Next->remaining_time --;

				}
				else
				{
					runQueue->Next->now_running_time++;
					runQueue->Next->remaining_time--;
				}
			}
		}

		// no one ready & current process is not 0
		else if (one_ready == 0 && cur_proc != -1) {
			// check if blocked
			// check if finish
			// current pro running time ++
			if (checkInBlocked(runQueue)) {
				PQUEUE* blocked = popRunQueue(runQueue, cur_proc);
				insertAdress(blockedQueue, blocked);
				one_blocked++;
				cur_proc = -1;
			}
			else if (checkFinished(runQueue))
			{
				PQUEUE* finished = popRunQueue(runQueue, cur_proc);
				insertAdress(finishedQueue, finished);
				num_finish++;
				cur_proc = -1;
			}
			else
			{
				runQueue->Next->now_running_time ++;
				runQueue->Next->remaining_time --;
			}
		}
		else {/* waste of cpu time with doing nothing */ }
		


		// check blocked queue
		if (one_blocked) {
			// find the first blocked one and check block time
			if (checkOutBlocked(blockedQueue, harddrive))
			{
				PQUEUE* old_blocked = popQueue(blockedQueue);
				old_blocked->now_blocked_time = 0;

				// next blocked process gets the harddrive
				if (blockedQueue->Next != NULL)
				{
					blockedQueue->Next->now_blocked_time ++;
					addTotalBT(blockedQueue);
				}
				// blocked queue -> finished queue
				if (old_blocked->remaining_time == 0) {
					insertAdress(finishedQueue, old_blocked);
					num_finish++;
				}
				// blocked queue -> running queue
				else if (cur_proc == -1)
				{
					insertAdress(runQueue, old_blocked);
					cur_proc = old_blocked->ID;
					old_blocked->now_running_time ++;
					old_blocked->remaining_time --;
				}
				// blocked queue -> readyqueue
				else
				{
					insertAdress(readyQueue, old_blocked);
					sort(readyQueue);
		
					one_ready++;
				}
				
				one_blocked--;
			}
			else
			{
				blockedQueue->Next->now_blocked_time++;
				addTotalBT(blockedQueue);
			}

		}

		// add time for all the processes in the ready queue
		addReadyTime(readyQueue);
		
		time++;
	}

	// check finished queue and print out the results
	printf("0 %d\n", time - zeroTime(finishedQueue));
	PQUEUE* tempF = finishedQueue;
	while (tempF->Next != NULL)
	{
		printf("%d %d %d %d\n", tempF->Next->ID, tempF->Next->running_time, 
			tempF->Next->total_ready_time, tempF->Next->total_blocked_time);

		tempF = tempF->Next;
	}
}


