#include "cfg.h"
#include <GL/glut.h>

// Global Variables

// Position and size
const int X_POS = 100, Y_POS = 150, X_MAX = 800, Y_MAX = 600;
const float PI = 3.14159265358;
FILE *fp;


const Edge ZERO_EDGE = { -1,-1.0,-INFINITY };
Edge* AET = nullptr;
Edge** NET = new Edge*[Y_MAX];

void myInit(void)
{
	// background coler
	fp = fopen("PolyDino.txt", "r");
	if (fp == NULL)
	{
		printf("Could not open file");
		exit(-1);
	}

	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	gluOrtho2D(0, Y_MAX, 0, X_MAX);
	glClear(GL_COLOR_BUFFER_BIT);
}
// quite