#include "ET.h"
#include "cfg.h"
#include <GL/glut.h> 

// Scanline Function 
void initEdgeTable()
{
	int i;
	for (i = 0; i < Y_MAX; i++)
	{
		NET[i].countEdgeBucket = 0;
	}

	AET.countEdgeBucket = 0;
}

// Print AET or NET
void printTuple(EdgeTableTuple *tup)
{
	int j;

	if (tup->countEdgeBucket)
		printf("\nCount %d-----\n", tup->countEdgeBucket);

	for (j = 0; j<tup->countEdgeBucket; j++)
	{
		printf(" %d+%.2f+%.2f",
			tup->buckets[j].ymax, tup->buckets[j].xofymin, tup->buckets[j].slopeinverse);
	}
}

// print NET
void printTable()
{
	int i, j;

	for (i = 0; i<Y_MAX; i++)
	{
		if (NET[i].countEdgeBucket)
			printf("\nScanline %d", i);

		printTuple(&NET[i]);
	}
}


/* Function to sort an array using insertion sort*/
void insertionSort(EdgeTableTuple *ett)
{
	int i, j;
	EdgeBucket temp;

	for (i = 1; i < ett->countEdgeBucket; i++)
	{
		temp.ymax = ett->buckets[i].ymax;
		temp.xofymin = ett->buckets[i].xofymin;
		temp.slopeinverse = ett->buckets[i].slopeinverse;
		j = i - 1;

		while ((temp.xofymin < ett->buckets[j].xofymin) && (j >= 0))
		{
			ett->buckets[j + 1].ymax = ett->buckets[j].ymax;
			ett->buckets[j + 1].xofymin = ett->buckets[j].xofymin;
			ett->buckets[j + 1].slopeinverse = ett->buckets[j].slopeinverse;
			j = j - 1;
		}
		ett->buckets[j + 1].ymax = temp.ymax;
		ett->buckets[j + 1].xofymin = temp.xofymin;
		ett->buckets[j + 1].slopeinverse = temp.slopeinverse;
	}
}


void storeEdgeInTuple(EdgeTableTuple *receiver, int ym, int xm, float slopInv)
{
	// both used for edgetable and active edge table.. 
	// The edge tuple sorted in increasing ymax and x of the lower end. 
	(receiver->buckets[(receiver)->countEdgeBucket]).ymax = ym;
	(receiver->buckets[(receiver)->countEdgeBucket]).xofymin = (float)xm;
	(receiver->buckets[(receiver)->countEdgeBucket]).slopeinverse = slopInv;

	// sort the buckets 
	insertionSort(receiver);

	(receiver->countEdgeBucket)++;


}

// store edge in NET of line(x1,y1)-->(x2,y2)
void storeEdgeInTable(int x1, int y1, int x2, int y2)
{
	// m means for slope, minv means for the inverse of slope
	float m, minv;
	// scanline means ymin of the line first show up
	int ymaxTS, xwithyminTS, scanline; //ts stands for to store 

	if (x2 == x1)
	{
		minv = 0.000000;
	}
	else
	{
		m = ((float)(y2 - y1)) / ((float)(x2 - x1));

		// horizontal lines are not stored in edge table 
		if (y2 == y1)
			return;

		minv = (float)1.0 / m;
		printf("\nSlope string for %d %d & %d %d: %f", x1, y1, x2, y2, minv);
	}

	if (y1>y2)
	{
		scanline = y2;
		ymaxTS = y1;
		xwithyminTS = x2;
	}
	else
	{
		scanline = y1;
		ymaxTS = y2;
		xwithyminTS = x1;
	}
	// the assignment part is done..now storage.. 
	storeEdgeInTuple(&NET[scanline], ymaxTS, xwithyminTS, minv);


}


void removeEdgeByYmax(EdgeTableTuple *Tup, int yy)
{
	int i, j;
	for (i = 0; i< Tup->countEdgeBucket; i++)
	{
		if (Tup->buckets[i].ymax == yy)
		{
			printf("\nRemoved at %d", yy);
			// simply remove the element by shifting forward the rest
			for (j = i; j < Tup->countEdgeBucket - 1; j++)
			{
				Tup->buckets[j].ymax = Tup->buckets[j + 1].ymax;
				Tup->buckets[j].xofymin = Tup->buckets[j + 1].xofymin;
				Tup->buckets[j].slopeinverse = Tup->buckets[j + 1].slopeinverse;
			}
			Tup->countEdgeBucket--;
			i--;
		}
	}
}

// Update AET
void updatexbyslopeinv(EdgeTableTuple *Tup)
{
	int i;

	for (i = 0; i<Tup->countEdgeBucket; i++)
	{
		(Tup->buckets[i]).xofymin = (Tup->buckets[i]).xofymin + (Tup->buckets[i]).slopeinverse;
	}
}


void ScanlineFill()
{
	/* Follow the following rules:
	1. Horizontal edges: Do not include in edge table
	2. Horizontal edges: Drawn either on the bottom or on the top.
	3. Vertices: If local max or min, then count twice, else count
	once.
	4. Either vertices at local minima or at local maxima are drawn.*/


	int i, j, x1, ymax1, x2, ymax2, FillFlag = 0, coordCount;

	// we will start from scanline 0; 
	// Repeat until last scanline: 

	// i means y_current
	for (i = 0; i<Y_MAX; i++)//4. Increment y by 1 (next scan line) 
	{

		// 1. Move from ET bucket y to the 
		// AET those edges whose ymin = y (entering edges) 
		for (j = 0; j<NET[i].countEdgeBucket; j++)
		{
			storeEdgeInTuple(&AET, NET[i].buckets[j].
				ymax, NET[i].buckets[j].xofymin,
				NET[i].buckets[j].slopeinverse);
		}
		printTuple(&AET);

		// 2. Remove from AET those edges for 
		// which y=ymax (not involved in next scan line) 
		removeEdgeByYmax(&AET, i);

		//sort AET (remember: ET is presorted) 
		insertionSort(&AET);

		printTuple(&AET);

		//3. Fill lines on scan line y by using pairs of x-coords from AET 
		j = 0;
		FillFlag = 0;
		coordCount = 0;
		x1 = 0;
		x2 = 0;
		ymax1 = 0;
		ymax2 = 0;
		while (j<AET.countEdgeBucket)
		{
			// matching 2 vertices
			if (coordCount % 2 == 0)
			{
				x1 = (int)(AET.buckets[j].xofymin);
				ymax1 = AET.buckets[j].ymax;

				if (x1 == x2)
				{
					/* three cases can arrive-
					1. lines are towards top of the intersection
					2. lines are towards bottom
					3. one line is towards top and other is towards bottom
					*/

					if (((x1 == ymax1) && (x2 != ymax2)) || ((x1 != ymax1) && (x2 == ymax2)))
					{
						x2 = x1;
						ymax2 = ymax1;
					}

					else
					{
						coordCount++;
					}
				}

				else
				{
					coordCount++;
				}
			}
			// find the first vertice
			else
			{
				x2 = (int)AET.buckets[j].xofymin;
				ymax2 = AET.buckets[j].ymax;

				FillFlag = 0;

				// checking for intersection... 
				if (x1 == x2)
				{
					/*three cases can arive-
					1. lines are towards top of the intersection
					2. lines are towards bottom
					3. one line is towards top and other is towards bottom
					*/
					if (((x1 == ymax1) && (x2 != ymax2)) || ((x1 != ymax1) && (x2 == ymax2)))
					{
						x1 = x2;
						ymax1 = ymax2;
					}
					else
					{
						coordCount++;
						FillFlag = 1;
					}
				}
				else
				{
					coordCount++;
					FillFlag = 1;
				}


				if (FillFlag)
				{
					//drawing actual lines... 
					glColor3f(0.0f, 0.7f, 0.0f);

					glBegin(GL_LINES);
					glVertex2i(x1, i);
					glVertex2i(x2, i);
					glEnd();
					glFlush();

					// printf("\nLine drawn from %d,%d to %d,%d",x1,i,x2,i); 
				}

			}

			j++;
		}


		// 5. For each nonvertical edge remaining in AET, update x for new y 
		updatexbyslopeinv(&AET);
	}


	printf("\nScanline filling complete");

}


