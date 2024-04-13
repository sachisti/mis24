#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <math.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <signal.h>
#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <pthread.h>

#include "comm.h"

#define COMM_LOG_FOLDER  "/home/kruzok/logs/comm"

static char comm_log_filename[60];

static int fdZapis[2];
static int fdCitanie[2];

static pid_t plink_pid;
char paket[MAX_DLZKA_PAKETU];
volatile int komunikacia_bezi;
volatile int program_bezi;
volatile int pocet_beziacich_vlakien;
volatile int arduino_pripravujesa;
volatile int arduino_inicializovane;

void prazdne_spracovanie_paketu(uint8_t *paket)
{
    // v poli paket[] je precitany 0-ukonceny paket prijaty z arduina
    // zadefinujte si rovnaku funkciu a priradte ju do premennej prijemca_paketu
    printf("arduino->rpi: %s\n", paket);
}

void (*prijemca_paketu)(uint8_t *paket) = prazdne_spracovanie_paketu;

long long msec()
{
  struct timeval tv;
  gettimeofday(&tv, 0);
  return 1000L * tv.tv_sec + tv.tv_usec / 1000L;
}

void log_msg(const char *msg)
{
	FILE *f = fopen(comm_log_filename, "a+");
	long long tm = msec();
	fprintf(f, "%.2lf %s\n", tm / 1000.0, msg);
	fclose(f);
}

void log_msg2(const char *msg1, const char *msg2)
{
	FILE *f = fopen(comm_log_filename, "a+");
	long long tm = msec();
	fprintf(f, "%.2lf %s: %s\n", tm / 1000.0, msg1, msg2);
	fclose(f);
}

void precitaj_paket_z_arduina()
{
    unsigned char ch;
    int precital;
    int packet_arrived = 0;

    do
    {
  
      // pockaj na znak $, ktory oznacuje zaciatok paketu
      do {
          ch = 0;
          if ((precital = read(fdCitanie[0], &ch, 1)) < 0)
          {
              if (errno != EAGAIN)
              {
		  komunikacia_bezi = 0;
		  break;
              }
              else usleep(2000);
          }
      } while (komunikacia_bezi && (ch != '$'));
      if (!komunikacia_bezi) return;
  
      // nacitaj cely riadok az po znak \n
      int precitane = 0;
      do {
            if ((precital = read(fdCitanie[0], paket + precitane, 1)) < 0)
            {
                if (errno != EAGAIN)
                {
                    komunikacia_bezi = 0;
		    break;
                }
                else usleep(2000); 
            }
            else if (precital > 0)
            {
                precitane += precital;
                if (precitane && paket[precitane - 1] == '\n') break;
                if (paket[precitane - 1] == '\r') precitane--; 
                if (precitane > MAX_DLZKA_PAKETU) break;
            }
      } while (komunikacia_bezi);
  
      // precitany znak \n nakoniec vymazeme
      if (precitane == 0) precitane++;
      paket[precitane - 1] = 0;
  
      packet_arrived = 1;
      if (arduino_inicializovane == 0)
      {
         if (strcmp(paket, "init") == 0) 
	 {
             arduino_pripravujesa = 1;
	     log_msg2("a2r", "init");
	 }
	 else if (arduino_pripravujesa && (strcmp(paket, "ok") == 0))
	 {
	     arduino_inicializovane = 1;
	     log_msg2("a2r", "ok");
	 }
	 packet_arrived = 0;
      } 
    } while (!arduino_inicializovane || !packet_arrived);

    log_msg2("a2r:", paket);
}


void *citaci_thread(void *args)
{
    precitaj_paket_z_arduina();
    while (komunikacia_bezi)
    {
        prijemca_paketu(paket);
        precitaj_paket_z_arduina();
    }

    usleep(100000);
    kill(plink_pid, SIGTERM);
    printf("citaci thread skoncil\n");
    pocet_beziacich_vlakien = 0;

    return 0;
}

void zapis_paket_do_arduina(uint8_t *zapisovany_paket)
{
    int zapisal = write(fdZapis[1], "$", 1);
    if (zapisal < 1)
    {
      perror("nepodarilo sa zapisat paket do arduina");
      exit(-1);
    }

    int dlzka_paketu = strlen(zapisovany_paket);
    int zapisane = 0;
    while (zapisane < dlzka_paketu)
    {
      zapisal = write(fdZapis[1], zapisovany_paket, dlzka_paketu);
      if (zapisal < 0)
      {
        perror("nepodarilo sa zapisat paket na arduino");
        exit(-1);
      }
      else if (zapisal == 0) usleep(100);
      zapisane += zapisal;
    }
    zapisal = write(fdZapis[1], "\n", 1);
    if (zapisal < 1)
    {
      perror("nepodarilo sa ukoncit paket pre arduino");
      exit(-1);
    }
    log_msg2("r2a:", zapisovany_paket);
}

int pripoj_arduino()
{
    pthread_t t;
    arduino_inicializovane = 0;
    arduino_pripravujesa = 0;
    paket[0] = 0;
    time_t tm;
    time(&tm);
    sprintf(comm_log_filename, "%s/%ld.txt", COMM_LOG_FOLDER, tm);
    log_msg("begin");
    char link_filename[60];
    sprintf(link_filename, "%s/last", COMM_LOG_FOLDER);
    unlink(link_filename);
    symlink(comm_log_filename, link_filename);

    if (pipe(fdZapis) < 0)
    {
        perror("nepodarilo sa vytvorit pipe na citanie");
        return 0;
    }
    if (pipe(fdCitanie) < 0)
    {
        perror("nepodarilo sa vytvorit pipe na zapisovanie");
        return 0;
    }

    if ((plink_pid = fork()) == 0)
    {
        /* dcersky proces */

        close(0);
        close(1);
        dup2(fdZapis[0], 0);
        dup2(fdCitanie[1], 1);
        close(fdZapis[0]);
        close(fdZapis[1]);
        close(fdCitanie[0]);
        close(fdCitanie[1]);

        if (execl("/usr/bin/plink", "/usr/bin/plink", ZARIADENIE,
                  "-serial", "-sercfg", "115200,N,n,8,1", NULL) < 0)
        {
            perror("nepodarilo sa spustit program plink");
            return 0;
        }
    }

    if (plink_pid < 0)
    {
        perror("nepodarilo sa vytvorit novy proces");
        return 0;
    }

    close(fdZapis[0]);
    close(fdCitanie[1]);
    if (fcntl( fdCitanie[0], F_SETFL, fcntl(fdCitanie[0], F_GETFL) | O_NONBLOCK) < 0)
    {
        perror("nepodarilo sa nastavit pipe ako asynchronnu");
        return 0;
    }

    sleep(2);
    if (waitpid(plink_pid, 0, WNOHANG))
    {
        printf("plink skoncil, pravdepodobne sa nepodarilo nadviazat spojenie s arduinom\n");
        return 0;
    }

    komunikacia_bezi = 1;
    if (pthread_create(&t, 0, citaci_thread, 0) != 0)
    {
      perror("nepodarilo sa vytvorit thread");
      exit(-1);
    }
    else pocet_beziacich_vlakien++;
    while (!arduino_pripravujesa)
    {
      zapis_paket_do_arduina("init");
      usleep(100000);
    }
    zapis_paket_do_arduina("ok");
    while (!arduino_inicializovane)
      usleep(100000);

    printf("otvorene spojenie s arduinom\n");
    return 1;
}

void odpoj_arduino()
{
    arduino_inicializovane = 0;
    close(fdCitanie[0]);
    close(fdZapis[1]);
}
