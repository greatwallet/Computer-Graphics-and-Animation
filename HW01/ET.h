#pragma once
#ifndef _ET_H_
#define _ET_H_

#define maxVer 10000 
// Start from lower left corner 
// For both AET and NET
typedef struct edgebucket
{
	int ymax; //max y-coordinate of edge 
	float xofymin; //x-coordinate of lowest edge point updated only in aet 
	float slopeinverse;
}EdgeBucket;

// ET list
typedef struct edgetabletup
{
	// the array will give the scanline number 
	// The edge table (ET) with edges entries sorted 
	// in increasing y and x of the lower end 

	int countEdgeBucket; //no. of edgebuckets 
	EdgeBucket buckets[maxVer];
}EdgeTableTuple;

void initEdgeTable();

void printTuple(EdgeTableTuple *tup);

void printTable();

void insertionSort(EdgeTableTuple *ett);

void storeEdgeInTuple(EdgeTableTuple *receiver, int ym, int xm, float slopInv);

void storeEdgeInTable(int x1, int y1, int x2, int y2);

void removeEdgeByYmax(EdgeTableTuple *Tup, int yy);

void updatexbyslopeinv(EdgeTableTuple *Tup);

void ScanlineFill();
#endif // !_ET_H_
