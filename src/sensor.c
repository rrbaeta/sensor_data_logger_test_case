#include <stdlib.h>

#define MAX_TEMPERATURE 150

int read_temperature()
{
    return (rand() % (MAX_TEMPERATURE + 1)); // Random temperature between 0 and 99
}