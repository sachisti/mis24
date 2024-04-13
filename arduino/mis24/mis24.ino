#include "pohyb.h"
#include "comm.h"
#include "sensors.h"
#include "rgb.h"

void raspberry_hovori(uint8_t *paket) 
{
  if (strcmp(paket, "demo1") == 0) demo1();
  else if (strcmp(paket, "demo2") == 0) demo2();
  else if (strcmp(paket, "demo") == 0) demo();
  else if (strcmp(paket, "f50") == 0) vpred(50);
  else if (strcmp(paket, "f30") == 0) vpred(30);
  else if (strcmp(paket, "f20") == 0) vpred(20);
}

void setup()
{
  setup_pohyb();
  setup_comm();
  setup_sensors();
  setup_rgb();
  rgb_red();
  delay(300);
  rgb_green();
  delay(300);
  rgb_blue();
  delay(300);
  rgb_off();
  prijemca_paketu = raspberry_hovori;
}

void loop()
{
  loop_comm();
  loop_sensors();
  print_sensors();
  delay(300);
}
