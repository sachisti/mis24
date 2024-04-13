#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <math.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <pthread.h>

#define MAX_DLZKA_PAKETU 30

#define ZARIADENIE "/dev/ttyUSB0"
 
extern volatile int program_bezi;
extern volatile int pocet_beziacich_vlakien;
extern volatile int arduino_inicializovane;

extern void (*prijemca_paketu)(uint8_t *paket);

int pripoj_arduino();

void precitaj_paket_z_arduina();

void zapis_paket_do_arduina(uint8_t *zapisovany_paket);

void odpoj_arduino();
