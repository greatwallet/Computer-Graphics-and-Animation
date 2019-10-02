#pragma once
#define _CRT_SECURE_NO_WARNINGS
#ifndef _CFG_H_
#define _CFG_H_
#include <iostream>
#include "ET.h"

// Global Variables

// CONST
extern const float PI;
// Position and size
extern const int X_POS, Y_POS, X_MAX, Y_MAX;
extern FILE *fp;

// zero edge means no edge
extern const Edge ZERO_EDGE;

extern Edge* AET;
extern Edge** NET;

void myInit(void);
#endif