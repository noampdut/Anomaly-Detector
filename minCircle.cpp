//Author: 315097113 Noam Pdut
#include "minCircle.h"
#include <cstdlib>
#include <cmath>

//calculate the distance between two points
float calcDistance(Point p1, Point p2) {
    float x=(p1.x-p2.x)*(p1.x-p2.x);
    float y=(p1.y-p2.y)*(p1.y-p2.y);
    return sqrt(x+y);
}

//get the middle point between two points
Point getMiddlePoint (Point p1, Point p2){
    float x = (p1.x+p2.x)/2;
    float y = (p1.y+p2.y)/2;
    return Point(x,y);
}

//create minimal circle from two points.
Circle createCircleFrom2Points (vector<Point> R) {
    float distance = calcDistance(R[0], R[1]);
    float radius = distance/2;
    Point p = getMiddlePoint(R[0], R[1]);
    return Circle(p, radius);
}

float getSlop(Point p1, Point p2) {
    return (p1.y-p2.y)/(p1.x-p2.x);
}

//create minimal circle from three points.
Circle createCircleFrom3Points(vector<Point> R) {
    Point p1 = R[0];
    Point p2 = R[1];
    Point p3 = R[2];

    //get the middle point of line (p1,p2)
    Point middlep1p2 = getMiddlePoint(p1, p2);
    //get the slop of line (p1,p2)
    float slopp1p2 = getSlop(p1, p2);
    //the perpendicular of line(p1,p2)
    float perpendicularSlopp1p2 = -(1/slopp1p2);

    //get the middle point of line (p2,p3)
    Point middlep2p3 = getMiddlePoint(p2, p3);
    //get the slop of line (p2,p3)
    float slopp2p3 = getSlop(p2, p3);
    //the perpendicular of line(p1,p2)
    float perpendicularSlopp2p3 = -(1/slopp2p3);

    //get the intersection point of perpendicularSlopp2p3 and perpendicularSlopp1p2
    float x = (- perpendicularSlopp2p3*middlep2p3.x + middlep2p3.y + perpendicularSlopp1p2*middlep1p2.x - middlep1p2.y)
            / (perpendicularSlopp1p2 - perpendicularSlopp2p3);
    float y = perpendicularSlopp1p2 * (x - middlep1p2.x) + middlep1p2.y;

    Point centerCircle = Point(x,y);
    float radius = calcDistance(p1, centerCircle);
    return Circle(centerCircle, radius);
}

//get the trivial circle from the points in R.
Circle trivialCircle (vector<Point> R){
    if (R.empty()) {
        return Circle(Point(0,0), 0);
    }

    if (R.size() == 1) {
        return Circle(R[0], 0);
    }

    if (R.size() == 2) {
        return createCircleFrom2Points(R);
    }
    return createCircleFrom3Points(R);
}

//check if the point is in the circle
bool ifPointIsInCircle (Circle circle, Point* point) {
    float distance = calcDistance(circle.center, Point(point->x, point->y));
    if (distance > circle.radius){
        return false;
    }
    return true;
}

/*
 * implements of welzl algorithm to find minimal circle
 */
Circle welzlAlgorithm(Point** points, vector<Point> R, size_t size) {
    if (size == 0 || R.size() == 3) {
        return trivialCircle(R);
    }
    int i = rand() % size;
    //swap the point in index i with the point in index size
    Point tempP = Point (points[size-1]->x, points[size-1]->y);
    points[size-1]->x = points[i]->x;
    points[size-1]->y = points[i]->y;
    points[i]->x = tempP.x;
    points[i]->y = tempP.y;

    //get the circle without the random point
    Circle d = welzlAlgorithm(points, R, size-1);
    //check if the random point is the circle d
    if (ifPointIsInCircle(d, points[size-1])) {
        return d;
    }
    //the random point is not in the circle.
    Point randomPoint = Point (points[size -1]->x, points[size-1]->y);
    R.push_back(randomPoint);
    return welzlAlgorithm(points, R, size-1);
}

Circle findMinCircle(Point** points, size_t size) {
    return welzlAlgorithm(points, {}, size);
}