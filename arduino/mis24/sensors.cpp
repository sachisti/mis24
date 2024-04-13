#include "Arduino.h"
#include "sensors.h"

int wall_fwd, wall_left, wall_right;
int black_leftencoder, black_rightencoder;
volatile encoder_t left_encoder, right_encoder;
volatile uint32_t last_changed_left, last_changed_right;
volatile uint32_t tmr2;
volatile uint8_t ldir, rdir;

void setup_sensors()
{
  last_changed_left = last_changed_right = 0;
  PCMSK2 = 128;  // PCINT23 (128)
  PCMSK0 = 1; 
  PCICR = 5;  // PCIE2 + PCIE0, enable pin-change interrupt PCINT[26:16] pins, and PCINT[7:0]  D8 is 0, D7 is 23

  TCCR2A = 0;  // OC2x disconnected, normal mode 0->255
  TCCR2B = 6;  // prescaler 256
  TIMSK2 = 1;  // turn on overflow interrupt
}

ISR(TIMER2_OVF_vect)
{
  tmr2++;
}

static uint8_t last_PIND;
static uint8_t d;
static uint32_t now_change;

ISR(PCINT0_vect)  //receiver fwd/bwd
{
  now_change = tmr2;
  
  if (now_change - last_changed_right >= 3)
  {
    right_encoder += rdir;
    last_changed_right = now_change;
  }
}
  
ISR(PCINT2_vect)  //receiver fwd/bwd
{
  now_change = tmr2;
  
  if (now_change - last_changed_left >= 3)  // approx. 8ms
  {
    left_encoder += ldir;
    last_changed_left = now_change;
  }  
}

uint32_t t0_period, t1_period, t2_period;

void measure() 
{
  uint16_t tmr = TCNT0;
  while (tmr >= TCNT0) tmr = TCNT0;
  uint32_t t1 = micros();
  tmr = TCNT0;
  while (tmr >= TCNT0) tmr = TCNT0;
  uint32_t t2 = micros();
  t0_period = t2 - t1;

  tmr = TCNT1;
  while (tmr >= TCNT1) tmr = TCNT1;
  t1 = micros();
  tmr = TCNT1;
  while (tmr >= TCNT1) tmr = TCNT1;
  t2 = micros();
  t1_period = t2 - t1;

  tmr = TCNT2;
  while (tmr >= TCNT2) tmr = TCNT2;
  t1 = micros();
  tmr = TCNT2;
  while (tmr >= TCNT2) tmr = TCNT2;
  t2 = micros();
  t2_period = t2 - t1;
}

void print_sensors()
{
//  measure();
  Serial.print(black_leftencoder);
  Serial.print(" ");
  Serial.print(black_rightencoder);
  Serial.print(" ");
  Serial.print(wall_fwd);
  Serial.print(" ");
  Serial.print(wall_left);
  Serial.print(" ");
  Serial.print(wall_right);
  Serial.print(" ");
  Serial.print(left_encoder);
  Serial.print(" ");
  Serial.print(right_encoder);
  Serial.print(" ");
  Serial.print(tmr2);
  Serial.println();
}

void loop_sensors()
{
  black_leftencoder = digitalRead(8);
  black_rightencoder = digitalRead(7);
  wall_fwd = analogRead(5);
  wall_left = analogRead(7);
  wall_right = analogRead(6);
}
