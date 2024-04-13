#include "Arduino.h"
#include "comm.h"

// pakety v oboch smeroch zacinaju znakom '$' a koncia znakom '\n'

static uint8_t paket[MAX_DLZKA_PAKETU];
static uint8_t *p;
static uint8_t citam;

void prazdny_prijemca(uint8_t *paket) {}

uint8_t raspberry_inicializovane, raspberry_pripravujesa;
void (*prijemca_paketu)(uint8_t *paket) = prazdny_prijemca;

void setup_comm() 
{
  raspberry_pripravujesa = 0;
  raspberry_inicializovane = 0;
  Serial.begin(115200);
  p = paket;
  citam = 0;
}

void spracuj_paket()
{
  // na tomto mieste mame v poli paket[] prijaty 0-ukonceny retazec znakov
  if (!raspberry_inicializovane)
  {
    if (strcmp(paket, "init") == 0)
    {
      raspberry_pripravujesa = 1;
      Serial.println("$ok");
    }
    else if (raspberry_pripravujesa && (strcmp(paket, "ok") == 0))
    {
      raspberry_inicializovane = 1;
    }
    return;
  }
  int a;
  // v tejto ukazke retazec dekodujeme ako cislo a vypiseme spravu s dvojnasobkom
  // vo vasom pripade budete na tomto mieste reagovat na spravu podla potreby...
  sscanf((char *)paket, "%d", &a);
  Serial.print("$dvojnasobok je ");
  Serial.println(a * 2);
  prijemca_paketu(paket);
}

void precitaj_dalsi_znak()
{
  if (p - paket >= MAX_DLZKA_PAKETU) p = paket;
  
  *p = Serial.read();
  if (*p == '$') 
  {
    p = paket;
    citam = 1;
  }
  else if (citam)
  {
    if (*p != '\n') p++;
    else 
    {
      *p = 0;
      citam = 0;
      spracuj_paket();
      p = paket;
    }
  }
}

void loop_comm() 
{
  if (!raspberry_pripravujesa)
  {
    Serial.println("$init");
    delay(100);
  }
  while (Serial.available()) precitaj_dalsi_znak();

  if (millis() % 4000 == 0) 
  {
    Serial.print("$arduino zdravi kazde 4 sekundy\n");
    delay(1);
  }
}
