#ifndef QUEUE_H
#define	QUEUE_H


#ifdef __cplusplus
extern "C"
{
#endif

#include "inttypes.h"

typedef struct
{
    struct nav_cell *cell;
    int16_t n;
} nav_queue_cell;

typedef struct
{
    nav_queue_cell *array;
    int capicity;
    int current;
} nav_queue;

void nav_queue_init(nav_queue *queue, nav_queue_cell *cells, int capicity);
void nav_queue_enqueue(nav_queue *queue, struct nav_cell *cell, int16_t n);
void nav_queue_dequeue(nav_queue *queue, nav_queue_cell *buffer);
int nav_queue_empty(nav_queue *queue);
int nav_queue_is_queued(nav_queue *queue, struct nav_cell *cell);

#ifdef __cplusplus
}
#endif

#endif	/* QUEUE_H */

