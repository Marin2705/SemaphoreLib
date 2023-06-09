/*
 *   Copyright (c) 2023 Alexis LEBEL @ Poclain Hydraulics Industrie
 *   All rights reserved.
 */
#ifndef __CIRCULAR_BUFFER_H
#define __CIRCULAR_BUFFER_H

#define FALSE 0x0
#define TRUE 0x1

typedef struct 
{
    int *buffer;
    int* head;
    int* tail;
    int maxlen;
}CircularBuffer;

int initCircularBuffer(CircularBuffer *circBuf, int *array, int length);
int resetCircularBuffer(CircularBuffer *circBuf);
int pushToCircularBuffer(CircularBuffer *circBuf, int pdata);
int popFromCircularBuffer(CircularBuffer *circBuf, int *data);


#endif