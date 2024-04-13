#ifndef _POHYB_H_
#define _POHYB_H_

#define MLF_B 5 //dopredu pravy
#define MLF_A 4 //dozadu pravy
#define MRF_B 6 //dopredu lavy
#define MRF_A 2 //dozadu lavy

#define TRAVEL_SPEED 42

void setup_pohyb();

void demo1();  //accellerate and then decellerate fwd
void demo2();  //accellerate and then decellerate bwd

void demo();   // current debugging demo

void vpred(int vzdialenost);  // travel vzdialenost encoder ticks fwd



#endif
