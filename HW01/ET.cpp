#include "ET.h"
#include "cfg.h"
#include <GL/glut.h> 

// Scanline Function 
void initNET()
{
	int i;
	for (i = 0; i < Y_MAX; i++)
		NET[i] = nullptr;

	AET = nullptr;
}

// Print AET or NET
void printTuple(Edge *tup)
{
	for (Edge* e = tup; e != nullptr; e = e->next)
		printf(" %d + %.2f + %.2f --> ",
			e->ymax, e->xofymin, e->slopeinverse);
}

// print NET
void printTable()
{
	for (int y = 0; y < Y_MAX; y++)
	{
		if (NET[y] != nullptr)
		{
			printf("\n y = %d: ", y);
			printTuple(NET[y]);
			printf(" ^ ");
		}
	}
}


/* Function to sort an array using insertion sort*/
void insertionSort(Edge *&ett)
{
	if (ett == nullptr)return;
	else if (ett->next == nullptr)return;
	for (Edge *p_pred = ett, *p = ett->next; p != nullptr && p_pred != nullptr; p_pred = p_pred->next, p = p->next)
	{
		// find the insert spot
		Edge *pp_pred, *pp;
		for (Edge *pp_pred = ett, *pp = ett;pp != p;pp_pred = pp_pred->next, pp = pp->next)
		{
			if (pp == ett->next)pp_pred = ett;

			if (pp->xofymin > p->xofymin)
			{
				if (pp == ett)
				{
					Edge *temp = ett;
					p_pred->next = p->next;
					ett = p;
					ett->next = temp;
					p = p_pred->next;
				}
				else
				{
					p_pred->next = p->next;
					pp_pred->next = p;
					p->next = pp;
					p = p_pred->next;
				}
				break;
			}	
		}
		if (p == nullptr)break;
	}
}

// merge the insert and store altogether using insertion sort
void storeEdgeInTuple(Edge *&receiver, int ym, int xm, float slopInv)
{
	// both used for edgetable and active edge table.. 
	// The edge tuple sorted in increasing ymax and x of the lower end. 

	Edge *nedge = new Edge;
	nedge->ymax = ym;nedge->xofymin = (float)xm;nedge->slopeinverse = slopInv;nedge->next = nullptr;
	// for elements == 0
	if (receiver == nullptr)
	{
		receiver = nedge;
		return;
	}
	// for elements == 1
	else if (receiver->next == nullptr)
	{
		// if should insert before the head
		if (receiver->xofymin > (float)xm)
		{
			nedge->next = receiver;
			receiver = nedge;
			return;
		}
		else
		{
			receiver->next = nedge;
			nedge->next = nullptr;
		}
	}
	// for elements >= 2
	else
	{
		// if should insert before the head
		if (receiver->xofymin > (float)xm)
		{
			nedge->next = receiver;
			receiver = nedge;
			return;
		}
		bool insertFlag = false;
		Edge *p, *p_pred;
		for (p = receiver, p_pred = receiver;p != nullptr;p = p->next, p_pred = p_pred->next)
		{
			if (p == receiver->next)p_pred = receiver;
			if (p->xofymin > (float)xm)
			{
				p_pred->next = nedge;
				nedge->next = p;
				insertFlag = true;
				break;
			}
		}
		// if should insert at last
		if (!insertFlag && p == nullptr)
		{
			p_pred->next = nedge;
			nedge->next = nullptr;
		}
	}
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

	if (y1 > y2)
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
	storeEdgeInTuple(NET[scanline], ymaxTS, xwithyminTS, minv);

}


void removeEdgeByYmax(Edge *&Tup, int yy)
{
	if (Tup == nullptr)return;
	if (Tup->next == nullptr)
	{
		if (Tup->ymax != yy)return;
		Edge *temp = Tup;
		Tup = nullptr;
		delete temp;
		return;
	}
	Edge *p = Tup,*ppred = Tup;
	while (p != nullptr)
	{
		if (p == Tup->next)ppred = Tup;
		if (p->ymax != yy)
		{
			p = p->next;
			ppred = ppred->next;
			continue;
		}
		// p->ymax == yy
		if (p == Tup)
		{
			Edge *temp = Tup;
			Tup = Tup->next;
			p = Tup;
			ppred = Tup;
			delete temp;
		}
		else
		{
			Edge *temp = p;
			ppred->next = temp->next;
			p = ppred->next;
			delete temp;
		}
	}

}

// Update AET
void updatexbyslopeinv(Edge *&Tup)
{
	for (Edge *p = Tup;p != nullptr;p = p->next)
		p->xofymin += p->slopeinverse;
}


void ScanlineFill()
{
	/* Follow the following rules:
	1. Horizontal edges: Do not include in edge table
	2. Horizontal edges: Drawn either on the bottom or on the top.
	3. Vertices: If local max or min, then count twice, else count
	once.
	4. Either vertices at local minima or at local maxima are drawn.*/


	int  x1, ymax1, x2, ymax2, FillFlag = 0, coordCount;

	// we will start from scanline 0; 
	// Repeat until last scanline: 

	// i means y_current
	for (int y = 0; y < Y_MAX; y++)//4. Increment y by 1 (next scan line) 
	{

		// 1. Copy from NET bucket y to the 
		// AET those edges whose ymin = y (entering edges) 
		for (Edge *p = NET[y];p != nullptr;p = p->next)
			storeEdgeInTuple(AET, p->ymax, p->xofymin, p->slopeinverse);
		printTuple(AET);

		// 2. Remove from AET those edges for 
		// which y=ymax (not involved in next scan line) 
		removeEdgeByYmax(AET, y);

		//sort AET (remember: ET is presorted) 
		insertionSort(AET);

		printTuple(AET);

		//3. Fill lines on scan line y by using pairs of x-coords from AET 
		FillFlag = 0;
		coordCount = 0;
		x1 = 0;
		x2 = 0;
		ymax1 = 0;
		ymax2 = 0;
		for (Edge *p = AET;p != nullptr;p = p->next)
		{
						// matching 2 vertices
			if (coordCount % 2 == 0)
			{
				x1 = (int)(p->xofymin);
				ymax1 = p->ymax;

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
						coordCount++;
				}
				else
					coordCount++;
			}
			// find the first vertice
			else
			{
				x2 = (int)(p->xofymin);
				ymax2 = p->ymax;

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
					glVertex2i(x1, y);
					glVertex2i(x2, y);
					glEnd();
					glFlush();
					// printf("\nLine drawn from %d,%d to %d,%d",x1,i,x2,i); 
				}
			}
		}

		// 5. For each nonvertical edge remaining in AET, update x for new y 
		updatexbyslopeinv(AET);
	}


	printf("\nScanline filling complete");

}


