#ifndef DETECTION_H
#define	DETECTION_H

#include "position.h"
#include "nav.h"

// Sensors and their unique number
#define S0 0
#define S1 1
#define S2 2
#define S3 3
#define S4 4

// Ideal center values
#define S0_IDEAL 125
#define S2_IDEAL 110

#define S_OFFSET 15

// Tolerance
#define S_TOL 5

// Our ranges
#define S_S_RANGE 20
#define S_M_RANGE 45
#define S_H_RANGE 45



#ifdef __cplusplus
extern "C"
{
#endif

int dectection_reading(int sensor);

void dectection_timer_callback();
void dectection_force_update();
void dectection_update_adj(int s0, int s1, int s2);
void dectection_centering_adj(int s0, int s2);
void detection_update_walls(struct nav_array *array, pos_t *current);
void detection_update_front_wall(struct nav_array *array, pos_t *current);

#ifdef __cplusplus
}
#endif


#endif	/* DETECTION_H */
