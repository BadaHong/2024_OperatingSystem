#ifndef QUEUE_H
#define QUEUE_H

#include "types.h"
#include "defs.h"
#include "param.h"
#include "proc.h"
#include "queue.h"
#include <stddef.h>

//defining queue structure for project02
struct queue{
        struct proc* buf[NPROC];
        int front; //index of start
        int rear; //index of end
        int size; //number of elements in queue
        int level; //level of queue
};

void initQ(struct queue *q, int level){
        q->front = 0;
        q->rear = 0;
        q->size = 0;
        q->level = level;
}

int sizeOfQ(struct queue *q){
        return (q->rear + NPROC - q->front) % NPROC;
}

int isFull(struct queue *q){
        return q->size == NPROC;
}

int isEmpty(struct queue *q){
        return q->size == 0;
}

int isTerminate(struct queue *q){ //it checks if there is RUNNABLE process
	int count = 0;
	for(int i = q->front; i != q->rear; i = (i+1) % NPROC){
		if(q->buf[i]->state == RUNNABLE){
		//if(q->buf[i]->state == RUNNABLE || q->buf[i]->state == RUNNING){
			count++;
		}
	}
	return count;
}

void enqueue(struct queue *q, struct proc *p){
	if(isFull(q)){
                return;
	}
	q->buf[q->rear] = p;
        q->rear = (q->rear + 1) % NPROC;
        q->size++;
}

struct proc *dequeue(struct queue *q){
        if(isEmpty(q)){
                return NULL;
        }
        struct proc *p = q->buf[q->front];
        q->front = (q->front + 1) % NPROC;
        q->size--;
        return p; //return the head
}

void deleteProcess(struct queue* q, struct proc* p){
	if(isEmpty(q)){
		return;
	}
	for(int i = q->front; i != q->rear; i = (i+1) % NPROC){
		if(q->buf[i] == p){
			for(int j = i; j != q->rear; j = (j+1) % NPROC){
				q->buf[j] = q->buf[(j+1) % NPROC];
			}
			q->rear = (q->rear - 1 + NPROC) % NPROC;
			q->size--;
			return;
		}
	}
}

#endif
