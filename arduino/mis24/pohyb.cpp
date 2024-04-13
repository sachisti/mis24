#include "Arduino.h"
#include "pohyb.h"

void setup_pohyb() 
{
   pinMode(MLF_A, OUTPUT) ; //Logic pins are also set as output
   pinMode(MLF_B, OUTPUT) ;
   pinMode(MRF_A, OUTPUT) ; //Logic pins are also set as output
   pinMode(MRF_B, OUTPUT) ;
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
