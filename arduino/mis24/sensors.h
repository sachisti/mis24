#ifndef _SENSORS_H_
#define _SENSORS_H_

#include <inttypes.h>

typedef uint16_t encoder_t;

extern int wall_fwd, wall_left, wall_right;
extern int black_leftencoder, black_rightencoder;
extern volatile encoder_t left_encoder, right_encoder;

// these are set by the movement module depending whether we want to increment or decrement encoders (+1/-1)
extern volatile uint8_t ldir, rdir;

void setup_sensors();

void print_sensors();

void loop_sensors();

#endif
