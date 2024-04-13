#include "pohyb.h"
#include "comm.h"

void setup()
{
  setup_pohyb();
  setup_comm();
}

void loop()
{
  loop_comm();
}
