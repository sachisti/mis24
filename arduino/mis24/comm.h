#ifndef _COMM_H_
#define _COMM_H_

#include <inttypes.h>

#define MAX_DLZKA_PAKETU 30

// pakety v oboch smeroch zacinaju znakom '$' a koncia znakom '\n'

extern uint8_t raspberry_inicializovane;
extern void (*prijemca_paketu)(uint8_t *paket);

void setup_comm();
void loop_comm();

#endif
