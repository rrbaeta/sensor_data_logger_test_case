#include "circular_buffer.h"

void buffer_add(CircularBuffer *cb, int value)
{
    cb->buffer[cb->head] = value;
    cb->head = (cb->head + 1) % BUFFER_SIZE;
    if (cb->count < BUFFER_SIZE)
    {
        cb->count++;
    }
    else
    {
        cb->tail = (cb->tail + 1) % BUFFER_SIZE;
    }
}