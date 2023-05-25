#ifndef SENSORS_H 
#define SENSORS_H

#include "common.h"

void scanButton(struct LADDER &ladder);
void scanMove(struct LADDER &ladder);
void switchMode(struct LADDER &ladder);
bool scanPir(struct LADDER &ladder);

#endif
