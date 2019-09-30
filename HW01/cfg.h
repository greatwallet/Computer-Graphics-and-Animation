#pragma once
#define _CRT_SECURE_NO_WARNINGS
#ifndef _CFG_H_
#define _CFG_H_
#include <iostream>
#include "ET.h"

// Global Variables

// Position and size
extern const int X_POS, Y_POS, X_MAX, Y_MAX;
extern FILE *fp;

extern EdgeTableTuple AET;
extern EdgeTableTuple* NET;

void myInit(void);
#endif