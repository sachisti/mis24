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

#include "comm.h"

void hlavny_program()
{
    char sprava[100];
    int cislo = 0;

    for (int i = 0; i < 5; i++)
    {
        sprintf(sprava, "%d", cislo);
        cislo++;
        printf("rpi->arduino: %s\n", sprava);
        zapis_paket_do_arduina(sprava);
        sleep(1);
    } 
    odpoj_arduino();
}

int main()
{
    pthread_t t;
    pocet_beziacich_vlakien = 0;
    program_bezi = 1;

    if (!pripoj_arduino()) exit(-1);
    while (!arduino_inicializovane) usleep(1000);

    hlavny_program();
    program_bezi = 0;

    while (pocet_beziacich_vlakien > 0) usleep(1000);

    printf("program skoncil\n");
    return 0;
}

