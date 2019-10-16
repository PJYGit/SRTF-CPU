/*
	I wrote the queue by myself in order to make it much
	easier for the using in this program.
	However, some parts of the code may seem similar to the
	code in the course "Data Structure" which I took in my
	home university since I'm an exchange student.
*/

#include "my_queue.h"

void insert(PQUEUE* f, int readyT, int id, int runningT, int num, ExcType* excArray)
{
	PQUEUE* pNew = (PQUEUE*)malloc(sizeof(PQUEUE));

	if (pNew == NULL)
	{
		printf("fail to malloc!");
		exit(-1);
	}

	pNew->ready_time = readyT;
	pNew->ID = id;
	pNew->running_time = runningT;
	pNew->num_exc = num;

	ExcType* exc_mes = malloc((MAX_HARD_TIMES + 1) * sizeof(ExcType));
	int i;
	for (i = 0; i < MAX_HARD_TIMES; i++)
		exc_mes[i] = excArray[i];
	pNew->exc = exc_mes;

	pNew->time_of_exc = 0;
	pNew->now_running_time = 0;
	pNew->remaining_time = runningT;
	pNew->total_ready_time = 0;
	pNew->now_blocked_time = 0;
	pNew->total_blocked_time = 0;

	if (f->Next == NULL)
	{
		pNew->Next = NULL;
		f->Next = pNew;
	}

	else
	{
		PQUEUE* temp = f;
		while (temp->Next != NULL) {
			temp = temp->Next;
		}
		temp->Next = pNew;
		pNew->Next = NULL;
	}

}
void insertAdress(PQUEUE* p, PQUEUE* node) {
	if (p->Next == NULL)
		p->Next = node;

	else
	{
		PQUEUE* temp = p;
		while (temp->Next != NULL) {
			temp = temp->Next;
		}
		temp->Next = node;
		
	}

	node->Next = NULL;
}
void sort(PQUEUE* f)
{
	PQUEUE* w, * q, * tmp;
	int i, length = 0;
	int temp;
	ExcType* tmpExc;

	tmp = f;
	while (tmp->Next != NULL) {
		length++;
		tmp = tmp->Next;
	}

	for (i = 0; i < length; i++) {
		w = f->Next;
		while (w->Next != NULL) {
			q = w->Next;

			if (w->remaining_time > q->remaining_time)
			{
				temp = w->ready_time;
				w->ready_time = q->ready_time;
				q->ready_time = temp;

				temp = w->ID;
				w->ID = q->ID;
				q->ID = temp;

				temp = w->running_time;
				w->running_time = q->running_time;
				q->running_time = temp;

				temp = w->num_exc;
				w->num_exc = q->num_exc;
				q->num_exc = temp;

				temp = w->remaining_time;
				w->remaining_time = q->remaining_time;
				q->remaining_time = temp;

				temp = w->total_ready_time;
				w->total_ready_time = q->total_ready_time;
				q->total_ready_time = temp;

				temp = w->now_blocked_time;
				w->now_blocked_time = q->now_blocked_time;
				q->now_blocked_time = temp;

				temp = w->total_blocked_time;
				w->total_blocked_time = q->total_blocked_time;
				q->total_blocked_time = temp;

				temp = w->time_of_exc;
				w->time_of_exc = q->time_of_exc;
				q->time_of_exc = temp;

				temp = w->now_running_time;
				w->now_running_time = q->now_running_time;
				q->now_running_time = temp;

				tmpExc = w->exc;
				w->exc = q->exc;
				q->exc = tmpExc;

			}

			if (w->Next != NULL)
				w = w->Next;
			else break;
		}
	}

}

int checkNew(PQUEUE* p, int ready) {
	PQUEUE* temp = p;
	int find = 0;
	while (temp->Next != NULL)
	{
		if (temp->Next->ready_time == ready)
		{
			find = 1;
			break;
		}
		temp = temp->Next;
	}
	if (find) return 1;
	else return 0;
}
PQUEUE* popNewQueue(PQUEUE* p, int ready)
{
	PQUEUE* temp = p;
	PQUEUE* job = NULL;
	
	while (temp->Next != NULL)
	{
		if (temp->Next->ready_time == ready)
		{
			job = temp->Next;
			p->Next = job->Next;
			break;
		}
		temp = temp->Next;
	}

	return job;
}

PQUEUE* popQueue(PQUEUE* p)
{
	PQUEUE* tp;
	tp = p->Next;
	p->Next = tp->Next;

	return tp;
}

PQUEUE* popRunQueue(PQUEUE* p, int id) {
	PQUEUE* temp = p;
	PQUEUE* job = NULL;
	
	while (temp->Next != NULL)
	{
		if (temp->Next->ID == id)
		{
			job = temp->Next;
			temp->Next = job->Next;
			break;
		}
		temp = temp->Next;
	}

	return job;
}

int checkOutBlocked(PQUEUE* p, int hard) {
	int find = 0;

	if (p->Next->now_blocked_time == hard)
		find = 1;

	return find;
}

int checkInBlocked(PQUEUE* p)
{
	if (p->Next->time_of_exc < p->Next->num_exc)
	{
		int temp = p->Next->time_of_exc;
		if (p->Next->now_running_time == p->Next->exc[temp])
		{
			p->Next->time_of_exc++;
			return 1;
		}
	}

	return 0;
}

int checkFinished(PQUEUE* p)
{
	if (p->Next->remaining_time == 0)
		return 1;
	return 0;
}

void addReadyTime(PQUEUE* p)
{
	PQUEUE* temp = p;
	while (temp->Next != NULL)
	{
		temp->Next->total_ready_time++;
		temp = temp->Next;
	}
}

void addTotalBT(PQUEUE* p)
{
	PQUEUE* temp = p;

	while (temp->Next != NULL)
	{
		temp->Next->total_blocked_time++;
		temp = temp->Next;
	}
}

int zeroTime(PQUEUE* p)
{
	PQUEUE* tempF = p;
	int tempT = 0;
	while (tempF->Next != NULL)
	{
		tempT += tempF->Next->running_time;
		tempF = tempF->Next;
	}
	return (tempT + 1);
}

void printQueue(PQUEUE* p)
{
	PQUEUE* t = p;
	while (t->Next != NULL) {
		printf("%d  ", t->Next->ID);
		t = t->Next;
	}
}
