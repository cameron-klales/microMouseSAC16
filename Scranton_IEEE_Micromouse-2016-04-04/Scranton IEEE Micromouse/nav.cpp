#include "nav.h"
#include "queue.h"
#include "motor.h"
#include "Arduino.h"

#define DEBUG

extern "C"
{

int nav_is_pos_in_bounds(struct nav_array *array, pos_t *position)
{
    return (position->column >= 0) && (position->row >= 0) && (position->column < array->columns) && (position->row < array->rows);
}

int nav_is_in_bounds(struct nav_array *array, int8_t row, int8_t column)
{
    return (row >= 0) && (column >= 0) && (row < array->rows) && (column < array->columns);
}

int nav_size(struct nav_array *array)
{
    return (array->rows * array->columns);
}

void nav_init(struct nav_array *array, struct nav_cell *cells, int8_t rows, int8_t columns)
{
    array->rows = rows;
    array->columns = columns;
    array->cells = cells;

    int size = nav_size(array);

    int i;
    for (i = 0; i < size; i++)
    {
        struct nav_cell *cell = &array->cells[i];
        cell->wall = 0;
        cell->has_visited = 0;
        cell->flood_num = -1;
        cell->row = (i / array->columns);
        cell->column = (i % array->columns);
    }
}

struct nav_cell *nav_get_cell_pos(struct nav_array *array, pos_t *position)
{
    return nav_get_cell(array, position->row, position->column);
}

struct nav_cell *nav_get_cell(struct nav_array *array, int8_t row, int8_t column)
{
    int index;
    /*if (! nav_is_in_bounds(array, row, column))
        return 0;*/
    
    index = (row * array->columns) + column;  
    return &array->cells[index];
}

void nav_reset_flood_num(struct nav_array *array)
{
    int i;
    int size = nav_size(array);
    for(i = 0; i < size; i++)
    {
        struct nav_cell *cell = &array->cells[i];
        cell->flood_num = -1;
    }
}

int nav_is_flooded(struct nav_cell *cell)
{
    return (cell->flood_num != -1);
}

void nav_flood(struct nav_array *array, pos_t *start)
{
    /*Declare our buffer on the stack*/
    nav_queue_cell cells[256];
    nav_queue queue;
    
    struct nav_cell *first;
    
    nav_reset_flood_num(array);
    
    nav_queue_init(&queue, cells, 256);
    
    /*Start at the first cell and queue it*/
    first = nav_get_cell_pos(array, start);
    
    /*Add it to the queue*/
    nav_queue_enqueue(&queue, first, 0);
    
    while(! nav_queue_empty(&queue))
    {
        /*Declare our buffer on the stack*/
        nav_queue_cell queue_cell;
        
        /*Current cell*/
        struct nav_cell *cell;
        
        /*Dequeue and fill our buffer*/
        nav_queue_dequeue(&queue, &queue_cell);
        
        /*Assign our current cell*/
        cell = queue_cell.cell;
        
        /*Assign n to the flood number*/
        queue_cell.cell->flood_num = queue_cell.n;
        
        /*North*/
        if (!nav_north_wall(queue_cell.cell) && nav_is_in_bounds(array, cell->row - 1, cell->column))
        {
            struct nav_cell *north_cell = nav_get_cell(array, cell->row - 1, cell->column);
            
            if (!nav_is_flooded(north_cell) && !nav_queue_is_queued(&queue, north_cell))
            {
                nav_queue_enqueue(&queue, north_cell, queue_cell.n + 1);
            }
        }
        
        /*East*/
        if (!nav_east_wall(queue_cell.cell) && nav_is_in_bounds(array, cell->row, cell->column + 1))
        {
            struct nav_cell *east_cell = nav_get_cell(array, cell->row, cell->column + 1);
            
            if (!nav_is_flooded(east_cell) && !nav_queue_is_queued(&queue, east_cell))
            {
                nav_queue_enqueue(&queue, east_cell, queue_cell.n + 1);
            }
        }
        
        /*South*/
        if (!nav_south_wall(queue_cell.cell) && nav_is_in_bounds(array, cell->row + 1, cell->column))
        {
            struct nav_cell *south_cell = nav_get_cell(array, cell->row + 1, cell->column);
            
            if (!nav_is_flooded(south_cell) && !nav_queue_is_queued(&queue, south_cell))
            {
                nav_queue_enqueue(&queue, south_cell, queue_cell.n + 1);
            }
        }
        
        /*West*/
        if (!nav_west_wall(queue_cell.cell) && nav_is_in_bounds(array, cell->row , cell->column - 1))
        {
            struct nav_cell *west_cell = nav_get_cell(array, cell->row, cell->column - 1);
            
            if (!nav_is_flooded(west_cell) && !nav_queue_is_queued(&queue, west_cell))
            {
                nav_queue_enqueue(&queue, west_cell, queue_cell.n + 1);
            }
        }
    }
}

void nav_drive_to_target(struct nav_array *array, pos_t *start, pos_t *target)
{
    while(! position_equal_location(start, target))
    {
        /*Get the next lowest neighbor*/
        struct nav_cell *next_cell = nav_get_next_neighbor(array, start->row, start->column);
        
        /*Get the direction to the next cell*/
        dir_t dir = position_get_direction_to(start, next_cell->row, next_cell->column);
        
        /*Turn to a direction*/
        motor_turn_to_direction(start, dir);
        start->direction = dir;
        
        /*Move forward*/
        motor_move_forward();
        position_move_forward(start);
    }
}

struct nav_cell *nav_get_next_neighbor(struct nav_array *array, int8_t row, int8_t column)
{
    struct nav_cell *cell = nav_get_cell(array, row, column);
    int target = cell->flood_num - 1;
    
#ifdef DEBUG
    Serial.println("Checking north");
#endif

    /*North*/
    if (nav_is_in_bounds(array, row - 1, column) && !nav_north_wall(cell))
    {
#ifdef DEBUG
    	Serial.println("Seeing if North is target");
#endif
        struct nav_cell *north = nav_get_cell(array, row - 1, column);
        
        if (north->flood_num == target)
        {

#ifdef DEBUG
    	Serial.println("North is target");
#endif
           return north; 
        }
    }

#ifdef DEBUG
    Serial.println("Checking East");
#endif

    /*East*/
    if (nav_is_in_bounds(array, row, column + 1) && !nav_east_wall(cell))
    {

#ifdef DEBUG
    	Serial.println("Seeing if East is target");
#endif
        struct nav_cell *east = nav_get_cell(array, row, column + 1);
        
        if (east->flood_num == target)
        {
#ifdef DEBUG
    	Serial.println("East is target");
#endif
           return east; 
        }
    }

#ifdef DEBUG
    Serial.println("Checking South");
#endif

    /*South*/
    if (nav_is_in_bounds(array, row + 1, column) && !nav_south_wall(cell))
    {
        struct nav_cell *south = nav_get_cell(array, row + 1, column);
        
#ifdef DEBUG
    	Serial.println("Seeing if South is target");
#endif

        if (south->flood_num == target)
        {

#ifdef DEBUG
    	Serial.println("South is target");
#endif
           return south; 
        }
    }

#ifdef DEBUG
    Serial.println("Checking West");
#endif
    /*West*/
    if (nav_is_in_bounds(array, row, column - 1) && !nav_west_wall(cell))
    {

#ifdef DEBUG
    	Serial.println("Seeing if West is target");
#endif
        struct nav_cell *west = nav_get_cell(array, row, column - 1);
        
        if (west->flood_num == target)
        {
#ifdef DEBUG
    	Serial.println("West is target");
#endif
           return west; 
        }
    }

#ifdef DEBUG
    	Serial.println("---- ERROR ----");
    	Serial.println("Reached end of condition");
    	Serial.println("---- ERROR ----");
#endif

    // This should never happen
    return 0;
}

void nav_explore(struct nav_array *array, pos_t *start)
{
    nav_explore_rec(array, start);
}

void nav_explore_rec(struct nav_array *array, pos_t *current)
{
    /*Get current position*/
    struct nav_cell *cell = nav_get_cell_pos(array, current);

    //Update the wall detection
    //delay(250);
	detection_update_walls(array, current);
	detection_update_front_wall(array, current);

    /*Mark cell as visited*/
    cell->has_visited = 1;
    
    /*North*/
    if (nav_is_in_bounds(array, cell->row - 1, cell->column) && !nav_north_wall(cell))
    {
        struct nav_cell *north_cell = nav_get_cell(array, cell->row - 1, cell->column);
        
        if (!north_cell->has_visited)
        {   
            // Turn to the direction and move forward
            motor_turn_to_direction(current, north);
            current->direction = north;

            motor_move_forward();
            position_move_forward(current);
            
            // Explore new cell
            nav_explore_rec(array, current);
            
            // Move back to original cell
            motor_turn_to_direction(current, south);
            current->direction = south;

            motor_move_forward();
            position_move_forward(current);
        }
    }
        
    /*East*/
    if (nav_is_in_bounds(array, cell->row, cell->column + 1) && !nav_east_wall(cell))
    {
        struct nav_cell *east_cell = nav_get_cell(array, cell->row, cell->column + 1);
        
        if (!east_cell->has_visited)
        {   
            /*Turn to the direction and move forward*/
            motor_turn_to_direction(current, east);
            current->direction = east;

            motor_move_forward();
            position_move_forward(current);
            
            /*Explore new cell*/
            nav_explore_rec(array, current);
            
            /*Move back to original cell*/
            motor_turn_to_direction(current, west);
            current->direction = west;

            motor_move_forward();
            position_move_forward(current);
            
        }
    }

    /*South*/
    if (nav_is_in_bounds(array, cell->row + 1, cell->column) && !nav_south_wall(cell))
    {
        struct nav_cell *south_cell = nav_get_cell(array, cell->row + 1, cell->column);
        
        if (!south_cell->has_visited)
        {   
            /*Turn to the direction and move forward*/
            motor_turn_to_direction(current, south);
            current->direction = south;
            
            motor_move_forward();
            position_move_forward(current);

            /*Explore new cell*/
            nav_explore_rec(array, current);
            
            /*Move back to original cell*/
            motor_turn_to_direction(current, north);
            current->direction = north;
            
            motor_move_forward();
            position_move_forward(current);
        }
    }

    /*West*/
    if (nav_is_in_bounds(array, cell->row, cell->column - 1) && !nav_west_wall(cell))
    {
        struct nav_cell *west_cell = nav_get_cell(array, cell->row, cell->column - 1);
        
        if (!west_cell->has_visited)
        {   
            /*Turn to the direction and move forward*/
            motor_turn_to_direction(current, west);
            current->direction = west;

            motor_move_forward();
            position_move_forward(current);
            
            /*Explore new cell*/
            nav_explore_rec(array, current);
            
            /*Move back to original cell*/
            motor_turn_to_direction(current, east);
            current->direction = east;

            motor_move_forward();
            position_move_forward(current);
            
        }
    }
}

void nav_update_wall_cell(struct nav_cell *cell, dir_t dir)
{
    if (dir == north)
    {
    	cell->wall |= 0x01;
    }
    else if (dir == east)
    {
    	cell->wall |= 0x02;
    }
    else if (dir == south)
    {
    	cell->wall |= 0x04;
    }
    else
    {
    	cell->wall |= 0x08;
    }   
}

void nav_update_wall(struct nav_array *array, pos_t *position, facing_t dir)
{
    struct nav_cell *cell = nav_get_cell_pos(array, position);
    
    /*Convert the scalar to a direction*/
    dir_t wall_dir = position_convert_to_direction(position, dir);
    
    /*Update our nav cell*/
    nav_update_wall_cell(cell, wall_dir);
    
    /*Update coresponding cell
    TODO put this code somewhere else in its own subroutine*/
    if (wall_dir == north && nav_is_in_bounds(array, position->row - 1, position->column))
    {
        struct nav_cell *north_cell = nav_get_cell(array, position->row - 1, position->column);
        nav_update_wall_cell(north_cell, south);
    }
    else if (wall_dir == east && nav_is_in_bounds(array, position->row, position->column + 1))
    {
        struct nav_cell *east_cell = nav_get_cell(array, position->row, position->column + 1);
        nav_update_wall_cell(east_cell, west);
    }
    else if (wall_dir == south && nav_is_in_bounds(array, position->row + 1, position->column))
    {
        struct nav_cell *south_cell = nav_get_cell(array, position->row + 1, position->column);
        nav_update_wall_cell(south_cell, north);
    }
    else if (wall_dir == west && nav_is_in_bounds(array, position->row, position->column - 1))
    {
        struct nav_cell *west_cell = nav_get_cell(array, position->row, position->column - 1);
        nav_update_wall_cell(west_cell, east);
    } 
}

uint8_t nav_north_wall(struct nav_cell *cell)
{
	return cell->wall & 0x01;
}

uint8_t nav_east_wall(struct nav_cell *cell)
{
	return cell->wall & 0x02;
}

uint8_t nav_south_wall(struct nav_cell *cell)
{
	return cell->wall & 0x04;
}

uint8_t nav_west_wall(struct nav_cell *cell)
{
	return cell->wall & 0x08;
}

}
