#ifndef POSITION_H
#define	POSITION_H

#include "inttypes.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Coordinate system direction
 */
typedef enum
{
    north,
    south,
    east,
    west
} dir_t;

/**
 * Facing direction relative to the mouse
 */
typedef enum
{
    front,
    back,
    left,
    right
} facing_t;

/**
 * Used to keep track of the position of the mouse
 */
typedef struct
{
	int8_t row;
	int8_t column;
    dir_t direction;
} pos_t;

/**
 * Checks to see if the two positions are equal (they have the same location and direction)
 * @param pos1 the first position
 * @param pos2 the second position
 * @return true if the two positions are equal; false otherwise
 */
int position_equal(pos_t *pos1, pos_t *pos2);

/**
 * Checks to see if the two positions have equal location
 * @param pos1 the first position
 * @param pos2 the second position
 * @return true if the two positions have equal locations; false otherwise
 */
int position_equal_location(pos_t *pos1, pos_t *pos2);

/**
 * Rotates the direction in the position to the right
 * @param position the position to be rotated
 */
void position_rotate_right(pos_t *position);

/**
 * Rotates the direction in the position to the left
 * @param position the position to be rotated
 */
void position_rotate_left(pos_t *position);

/**
 * Moves the position forward to the direction it is facing
 * @param position the position to be moved forward
 */
void position_move_forward(pos_t *position);

/**
 * Returns the direction that is 180 degrees to the given direction
 * @param dir the given direction
 * @return the direction that is 180 degrees to the given direction
 */
dir_t position_invert_direction(dir_t dir);

/**
 * Returns the direction that is to the right of the given direction
 * @param dir the given direction
 * @return the direction that is to the right of the given direction
 */
dir_t position_right_adj_direction(dir_t dir);

/**
 * Returns the direction that is to the left of the given direction
 * @param dir the given direction
 * @return the direction that is to the left of the given direction
 */
dir_t position_left_adj_direction(dir_t dir);

/**
 * Converts the scalar value into a directional value
 * @param position the position of the mouse
 * @param facing which facing we want
 * @return the directional value of the facing
 */
dir_t position_convert_to_direction(pos_t *position, facing_t facing);

dir_t position_get_direction_to(pos_t *position, int8_t row, int8_t column);

void position_copy(pos_t *value, pos_t *buffer);


#ifdef __cplusplus
}
#endif


#endif	/* POSITION_H */
