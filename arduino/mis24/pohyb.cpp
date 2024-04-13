#include "Arduino.h"
#include "pohyb.h"
#include "sensors.h"

void setup_pohyb() 
{
   pinMode(MLF_A, OUTPUT) ; //Logic pins are also set as output
   pinMode(MLF_B, OUTPUT) ;
   pinMode(MRF_A, OUTPUT) ; //Logic pins are also set as output
   pinMode(MRF_B, OUTPUT) ;
}

void rychlost(int lavy, int pravy)
{
  if (lavy >= 0)
  {
    digitalWrite(MLF_A, LOW);
    analogWrite(MLF_B, lavy);
    if (lavy == 0) digitalWrite(MLF_B, LOW);
    ldir = 1;
  }
  else
  {
    digitalWrite(MLF_A, HIGH);
    analogWrite(MLF_B, 255 - lavy);
    ldir = -1;    
  }

  if (pravy >= 0)
  {
    digitalWrite(MRF_A, LOW);
    analogWrite(MRF_B, pravy);
    if (pravy == 0) digitalWrite(MRF_B, LOW);
    rdir = 1;
  }
  else
  {
    digitalWrite(MRF_A, HIGH);
    analogWrite(MRF_B, 255 - pravy);
    rdir = -1;    
  }
}

static encoder_t begin_left, begin_right;
static encoder_t end_left, end_right;
  
// zrychlovat
static uint8_t lr, rr;

void compensate()
{
    encoder_t dleft = left_encoder - begin_left;
    encoder_t dright = right_encoder - begin_right;
    if (dleft < dright - 1) 
    {
      if (lr > TRAVEL_SPEED + 5) 
      {
        rr--;
      }
      else
      {
        lr++;
      }
    }
    else if (dleft > dright + 1)
    {
      if (rr > TRAVEL_SPEED + 5) 
      {
        lr--;
      }
      else
      {
        rr++;
      }
    }
    rychlost(lr, rr);
}

void vpred(int vzdialenost)
{
  begin_left = left_encoder;
  begin_right = right_encoder;
  end_left = begin_left + vzdialenost;
  end_right = begin_right + vzdialenost;
  lr = rr = 0;
  
  while ((lr + rr) / 2 < TRAVEL_SPEED)
  { 
    lr ++;
    rr ++;
    compensate();
    delayMicroseconds(700);
  }

  while (left_encoder < end_left - 5)
  {
    compensate();
    delayMicroseconds(400);
  }
  while ((left_encoder < end_left) && (lr > 30))
  {
    lr -= 2;
    rr -= 2;
    compensate();
    delayMicroseconds(600);
  }

  rychlost(27, 27);
  while (left_encoder < end_left);
  rychlost(0,0);
}

void demo()
{
  rychlost(80, 0);
  delay(2000);
  rychlost(0, 80);
  delay(2000);
}

void demo1()
{
  digitalWrite(MLF_A, LOW);
  digitalWrite(MRF_A, LOW);
  for (int i = 0; i < 256; i++)
  {
    analogWrite(MLF_B, i);
    analogWrite(MRF_B, i);
    delay(2);
  }

  Serial.println("max");
  
  for (int i = 255; i >= 0; i--)
  {
    analogWrite(MLF_B, i);
    analogWrite(MRF_B, i);
    delay(2);
  }
}

void demo2()
{
  digitalWrite(MLF_A, HIGH);
  digitalWrite(MRF_A, HIGH);
  digitalWrite(MLF_B, HIGH);
  digitalWrite(MRF_B, HIGH);
  for (int i = 255; i >= 0; i--)
  {
    analogWrite(MLF_B, i);
    analogWrite(MRF_B, i);
    delay(2);
  }
  
  Serial.println("max");
  
  for (int i = 0; i < 256; i++)
  {
    analogWrite(MLF_B, i);
    analogWrite(MRF_B, i);
    delay(2);
  }

  digitalWrite(MLF_A, LOW);
  digitalWrite(MRF_A, LOW);
  digitalWrite(MLF_B, LOW);
  digitalWrite(MRF_B, LOW);
}

void dopredu()
{
   digitalWrite(MLF_A,LOW) ;
   digitalWrite(MLF_B,HIGH) ;
   digitalWrite(MRF_A,LOW) ;
   digitalWrite(MRF_B,HIGH) ;
}

void dozadu()
{
   digitalWrite(MLF_A,HIGH) ;
   digitalWrite(MLF_B,LOW) ;
   digitalWrite(MRF_A,HIGH) ;
   digitalWrite(MRF_B,LOW) ;
}

void vpravo()
{
   digitalWrite(MLF_A,LOW) ;
   digitalWrite(MLF_B,HIGH) ;
   digitalWrite(MRF_A,HIGH) ;
   digitalWrite(MRF_B,LOW) ;
}

void vlavo()
{
   digitalWrite(MLF_A,HIGH) ;
   digitalWrite(MLF_B,LOW) ;
   digitalWrite(MRF_A,LOW) ;
   digitalWrite(MRF_B,HIGH) ;
}

void okolo_praveho_kola()
{
   digitalWrite(MLF_A,LOW);
   digitalWrite(MLF_B,HIGH);
   digitalWrite(MRF_A,LOW);
   digitalWrite(MRF_B,LOW);
}

void okolo_laveho_kola()
{
   digitalWrite(MLF_A,LOW);
   digitalWrite(MLF_B,LOW);
   digitalWrite(MRF_A,LOW);
   digitalWrite(MRF_B,HIGH);
}
