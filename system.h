#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdint.h>

void init_clock();
void init_gpio();
void init_usart1();

void usart1_send(uint16_t data);

#endif
