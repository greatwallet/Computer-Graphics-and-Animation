//https://raw.githubusercontent.com/sprtkd/someCCodes/master/graphix/ass6/PolyDino.txt
// https://www.geeksforgeeks.org/scan-line-polygon-filling-using-opengl-c/
// CPP program to illustrate 
// Scanline Polygon fill Algorithm 
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h> 
#include <math.h> 
#include <GL/glut.h> 
#include <list>
#include "cfg.h"
#include "ET.h"


void drawPolyDino()
{

	glColor3f(1.0f, 0.0f, 0.0f);
	int count = 0, x1, y1, x2, y2;
	rewind(fp);
	while (!feof(fp))
	{
		count++;
		if (count>2)
		{
			x1 = x2;
			y1 = y2;
			// count = 2;
		}
		if (count == 1)
		{
			fscanf(fp, "%d,%d", &x1, &y1);
			x1 += 100; y1 += 100;
		}
		else
		{
			fscanf(fp, "%d,%d", &x2, &y2);
			x2 += 100; y2 += 100;
			printf("\n%d,%d", x2, y2);
			glBegin(GL_LINES);
			glVertex2i(x1, y1);
			glVertex2i(x2, y2);
			glEnd();
			storeEdgeInTable(x1, y1, x2, y2);//storage of edges in edge table. (NET)


			glFlush();
		}
	}


}

void drawDino(void)
{
	initEdgeTable();
	drawPolyDino();
	printf("\nTable");
	printTable();

	ScanlineFill();//actual calling of scanline filling.. 
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(Y_MAX, X_MAX);
	glutInitWindowPosition(100, 150);
	glutCreateWindow("Scanline filled dinosaur");
	myInit();
	glutDisplayFunc(drawDino);

	glutMainLoop();

	fclose(fp);
	delete NET;
}
