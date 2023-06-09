/*
 *   Copyright (c) 2023 Alexis LEBEL @ Poclain Hydraulics Industrie
 *   All rights reserved.
 */
#include "CircularBuffer.h"

int nullElt = 0;

int initCircularBuffer(CircularBuffer *circBuf, int *array, int length){
    circBuf->buffer = array;
    circBuf->maxlen = length;

    return 0;
}

int resetCircularBuffer(CircularBuffer *circBuf){
    *(circBuf->head) = 0;
    *(circBuf->tail) = 0;
}

int pushToCircularBuffer(CircularBuffer *circBuf, int pdata){
    int next = 0;

    next = (*(circBuf->head))+1;
    
    if (next >= (circBuf->maxlen - 2)){
        next=0;
    }

    if (next == *(circBuf->tail)){
        return -1;
    }

    circBuf->buffer[*(circBuf->head)] = pdata;
    *(circBuf->head) = next;

    
    return 0;
}

int popFromCircularBuffer(CircularBuffer *circBuf, int *data){
    int next;

    if (*(circBuf->head) == *(circBuf->tail)){
        return -1;
    }

    next = (*(circBuf->tail))+1;
    if (next >= (circBuf->maxlen - 2)){
        next=0;
    }

    *data = circBuf->buffer[*(circBuf->tail)];
    *(circBuf->tail) = next;
    return 0;
}