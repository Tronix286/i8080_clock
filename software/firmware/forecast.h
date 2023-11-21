#ifndef _FORECAST_H
#define _FORECAST_H

#include "main.h"
#include <stdlib.h>

void setH(int h);
void addP(long P, float t);
void setMonth(uint8_t month);
float getCast();
void myftoa(double x,int f,char *str);
int getTrend();
#endif