//Author: 315097113 Noam Pdut
#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "anomaly_detection_util.h"

using namespace std;

class Circle{
public:
	Point center;
	float radius;
	Circle(Point c,float r):center(c),radius(r){}
};

Circle findMinCircle(Point** points,size_t size);
bool ifPointIsInCircle (Circle circle, Point* point);
float calcDistance(Point p1, Point p2);


#endif /* MINCIRCLE_H_ */
