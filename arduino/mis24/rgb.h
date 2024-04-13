#ifndef _RGB_H_
#define _RGB_H_

#include <inttypes.h>

void setup_rgb();

// r,g,b  are 0 or 1
void rgb_rgb(uint8_t r, uint8_t g, uint8_t b);

void rgb_red();
void rgb_green();
void rgb_blue();
void rgb_off();

// r,g,b are 0-255 (blue only uses higest bit: on/off)
void rgb_color(uint8_t r, uint8_t g, uint8_t b);

#endif
