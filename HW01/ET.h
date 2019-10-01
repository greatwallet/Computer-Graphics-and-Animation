#pragma once
#ifndef _ET_H_
#define _ET_H_

// Start from lower left corner 
// For both AET and NET
typedef struct edge
{
	int ymax; //max y-coordinate of edge 
	float xofymin; //x-coordinate of lowest edge point updated only in aet 
	float slopeinverse;
	edge* next;
}Edge;


void initNET();

void printTuple(Edge *tup);

void printTable();

void insertionSort(Edge *&ett);

void storeEdgeInTuple(Edge *&receiver, int ym, int xm, float slopInv);

void storeEdgeInTable(int x1, int y1, int x2, int y2);

void removeEdgeByYmax(Edge *&Tup, int yy);

void updatexbyslopeinv(Edge *&Tup);

void ScanlineFill();
#endif // !_ET_H_
