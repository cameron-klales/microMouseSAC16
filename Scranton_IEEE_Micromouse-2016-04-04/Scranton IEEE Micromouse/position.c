#include "position.h"

int position_equal(pos_t *pos1, pos_t *pos2)
{
    return (pos1->row == pos2->row) && (pos1->column == pos2->column) && (pos1->direction == pos2->direction);
}

int position_equal_location(pos_t *pos1, pos_t *pos2)
{
    return (pos1->row == pos2->row) && (pos1->column == pos2->column);
}

void position_rotate_right(pos_t *position)
{
    if (position->direction == north)
    {
        position->direction = east;
    }
    else if (position->direction == east)
    {
        position->direction = south; 
    }
    else if (position->direction == south)
    {
        position->direction = west;
    }
    else if (position->direction == west)
    {
        position->direction = north;
    }
}

void position_rotate_left(pos_t *position)
{
    if (position->direction == north)
    {
        position->direction = west;
    }
    else if (position->direction == east)
    {
        position->direction = north; 
    }
    else if (position->direction == south)
    {
        position->direction = east;
    }
    else if (position->direction == west)
    {
        position->direction = south;
    }
}

void position_move_forward(pos_t *position)
{
    if (position->direction == north)
    {
        position->row -= 1;
    }
    else if (position->direction == south)
    {
        position->row += 1;
    }
    else if (position->direction == east)
    {
        position->column += 1;
    }
    else if (position->direction == west)
    {
        position->column -= 1;
    }
}

dir_t position_invert_direction(dir_t dir)
{
    if (dir == north)
    {
        return south;
    }
    else if (dir == east)
    {
        return west;
    }
    else if (dir == south)
    {
        return north;
    }
    else
    {
        return east;
    }
}

dir_t position_right_adj_direction(dir_t dir)
{
    if (dir == north)
    {
        return east;
    }
    else if (dir == east)
    {
        return south;
    }
    else if (dir == south)
    {
        return west;
    }
    else
    {
        return north;
    }
}

dir_t position_left_adj_direction(dir_t dir)
{
    if (dir == north)
    {
        return west;
    }
    else if (dir == east)
    {
        return north;
    }
    else if (dir == south)
    {
        return east;
    }
    else
    {
        return south;
    }
}

dir_t position_convert_to_direction(pos_t *position, facing_t dir)
{
    if (dir == front)
    {
        return position->direction;
    }
    else if (dir == back)
    {
        return position_invert_direction(position->direction);
    }
    else if (dir == right)
    {
        return position_right_adj_direction(position->direction);
    }
    else
    {
        return position_left_adj_direction(position->direction);
    }
}

dir_t position_get_direction_to(pos_t *position, int8_t row, int8_t column)
{
    /*North*/
    if (position->row - 1 == row && position->column == column)
    {
        return north;
    }
    /*East*/
    else if (position->row == row && position->column + 1 == column)
    {
        return east;
    }
    /*South*/
    else if (position->row + 1 == row && position->column == column)
    {
        return south;
    }
    /*West*/
    else if (position->row == row && position->column - 1 == column)
    {
        return west;
    }

    return north;
}

void position_copy(pos_t *value, pos_t *buffer)
{
    buffer->row = value->row;
    buffer->column = value->column;
    buffer->direction = value->direction;
}
