const int MLF_B = 5; //dopredu pravy
const int MLF_A = 4; //dozadu pravy
const int MRF_B = 2; //dopredu lavy
const int MRF_A = 3; //dozadu lavy

void setup() {
   pinMode(MLF_A,OUTPUT) ; //Logic pins are also set as output
   pinMode(MLF_B,OUTPUT) ;
   pinMode(MRF_A,OUTPUT) ; //Logic pins are also set as output
   pinMode(MRF_B,OUTPUT) ;
}

void loop() {
//dopredu
   digitalWrite(MLF_A,LOW) ;
   digitalWrite(MLF_B,HIGH) ;
   digitalWrite(MRF_A,LOW) ;
   digitalWrite(MRF_B,HIGH) ;
   delay (3000);
//dozadu
   digitalWrite(MLF_A,HIGH) ;
   digitalWrite(MLF_B,LOW) ;
   digitalWrite(MRF_A,HIGH) ;
   digitalWrite(MRF_B,LOW) ;
   delay (3000);
//clockwise
   digitalWrite(MLF_A,LOW) ;
   digitalWrite(MLF_B,HIGH) ;
   digitalWrite(MRF_A,HIGH) ;
   digitalWrite(MRF_B,LOW) ;
   delay (3000);
//counterclockwise
   digitalWrite(MLF_A,HIGH) ;
   digitalWrite(MLF_B,LOW) ;
   digitalWrite(MRF_A,LOW) ;
   digitalWrite(MRF_B,HIGH) ;
   delay (3000);
//goRight
   digitalWrite(MLF_A,LOW);
   digitalWrite(MLF_B,HIGH);
   digitalWrite(MRF_A,LOW);
   digitalWrite(MRF_B,LOW);
   delay(3000);
//goLeft
   digitalWrite(MLF_A,LOW);
   digitalWrite(MLF_B,LOW);
   digitalWrite(MRF_A,LOW);
   digitalWrite(MRF_B,HIGH);
   delay(3000);
}
