//Author: 315097113 Noam Pdut

#include "anomaly_detection_util.h"
#include <cstdlib>
#include <cmath>

float sumArr(float* x, int size) {
    float sum = 0;
    for (int i = 0; i < size; i++) {
        sum += x[i];
    }
    return sum;
}

float avg(float* x, int size) {
    float sum = sumArr(x, size);
    return sum / (float)size;
}

float avg(float* x, float* y, int size) {
    float sum = 0;
    for (int i = 0; i < size; i++) {
        sum += x[i] * y[i];
    }
    return sum / (float)size;
}

// returns the variance of X and Y
float var(float* x, int size) {
    float sum1 = 0;
    float sum2 = 0;
    for (int i = 0; i < size; i++) {
        float j = x[i] * x[i];
        sum1 += j;
        sum2 += x[i];
    }
    sum1 = sum1 / (float)size;
    sum2 = sum2 / (float)size;
    float result = sum1 - (sum2 * sum2);
    return result;
}

// returns the covariance of X and Y
float cov(float* x, float* y, int size) {
    float sum = avg(x, y, size) - (avg(x, size) * avg(y, size));
    return sum;
}

// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size) {
    float sqrtVar = std::sqrt((var(x, size))) * std::sqrt(var(y, size));
    return cov(x, y, size) / sqrtVar;
}

// performs a linear regression and returns the line equation
Line linear_reg(Point** points, int size) {
    float arrX[size];
    float arrY[size];

    for (int i = 0; i < size; i++) {
        arrX[i] = points[i]->x;
        arrY[i] = points[i]->y;
    }
    float a = cov(arrX, arrY, size) / var(arrX, size);
    float b = avg(arrY, size) - a * avg(arrX, size);
    return Line(a, b);
}

// returns the deviation between point p and the line equation of the points
float dev(Point p, Point** points, int size) {
    Line l = linear_reg(points, size);
    return dev(p, l);
}

// returns the deviation between point p and the line
float dev(Point p, Line l) {
    float y = l.f(p.x);
    return std::abs(p.y - y);
}