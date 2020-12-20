#pragma once
#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_
#include <Eigen/Dense>
using namespace Eigen;

// deg to rad
float deg2rad(float deg);
// Generate translate matrix
Matrix3f translate(float tx,float ty);

// Generate rotation matrix
Matrix3f rotate(float deg);
// Generate scale matrix
Matrix3f scale(float sx, float sy);

Vector3f genVec(int x, int y);
#endif