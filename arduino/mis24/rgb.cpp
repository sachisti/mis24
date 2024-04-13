#include "Arduino.h"
#include "rgb.h"

#define RED 10  
#define GREEN 11
#define BLUE 9  

void setup_rgb() 
{
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
}

void rgb_rgb(uint8_t r, uint8_t g, uint8_t b)
{
  analogWrite(RED, 0);
  analogWrite(GREEN, 0);
  digitalWrite(RED, r);
  digitalWrite(GREEN, g);
  digitalWrite(BLUE, b);
}

void rgb_red()
{
  rgb_rgb(1, 0, 0);
}

void rgb_green()
{
  rgb_rgb(0, 1, 0);
}

void rgb_blue()
{
  rgb_rgb(0, 0, 1);
}

void rgb_off()
{
  rgb_rgb(0, 0, 0);
}

void rgb_color(uint8_t r, uint8_t g, uint8_t b)
{
  analogWrite(RED, r);
  analogWrite(GREEN, g);
  digitalWrite(BLUE, b >> 7);
}
