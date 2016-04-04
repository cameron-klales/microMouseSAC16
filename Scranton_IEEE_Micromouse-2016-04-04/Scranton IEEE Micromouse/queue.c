#include "queue.h"

void nav_queue_init(nav_queue *queue, nav_queue_cell *cells, int capicity)
{
  queue->array = cells;
  queue->current = 0;
  queue->capicity = capicity;
}

void nav_queue_enqueue(nav_queue *queue, struct nav_cell *cell, int16_t n)
{ 
    nav_queue_cell *queue_cell;
    if (queue->capicity < queue->current + 1)
    {
        return;
    }
    
    queue_cell = &queue->array[queue->current];
    queue_cell->cell = cell;
    queue_cell->n = n;
    
    queue->current++;
}

void nav_queue_dequeue(nav_queue *queue, nav_queue_cell *buffer)
{
    int i;
    nav_queue_cell *queue_cell = &queue->array[0];
    
    buffer->cell = queue_cell->cell;
    buffer->n = queue_cell->n;
    
    for (i = 0; i < queue->current; i++)
    {
        queue->array[i] = queue->array[i + 1];
    }
    
    queue->current--;
}

int nav_queue_empty(nav_queue *queue)
{
    return (queue->current == 0);
}

int nav_queue_is_queued(nav_queue *queue, struct nav_cell *cell)
{
    int i;
    for (i = 0; i < queue->current; i++){
        if (queue->array[i].cell == cell)
            return 1;
    }
    
    return 0;
}
