//Author: 315097113 Noam Pdut
#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector():SimpleAnomalyDetector() {}

HybridAnomalyDetector::~HybridAnomalyDetector() = default;

void HybridAnomalyDetector::createCorreletedFeature(const TimeSeries& ts, int sizeOfVector, float m, int c,
                                                    string feature1, string feature2,Point** pointArr) {
    SimpleAnomalyDetector::createCorreletedFeature(ts, sizeOfVector, m, c, feature1, feature2, pointArr);
    if (m > 0.5 && m < defaultThreshold) {
        Circle minimalCircle = findMinCircle(pointArr, sizeOfVector);
        correlatedFeatures cfTemp;
        cfTemp.feature1 = feature1;
        cfTemp.feature2 = feature2;
        cfTemp.corrlation = m;
        float radius = minimalCircle.radius*(float)1.1;
        cfTemp.threshold = radius;
        cfTemp.x = minimalCircle.center.x;
        cfTemp.y = minimalCircle.center.y;
        cf.push_back(cfTemp);
    }
}

bool HybridAnomalyDetector::checkIfAnomalous(float x, float y, correlatedFeatures tempCF) {
    if (tempCF.corrlation >= defaultThreshold && SimpleAnomalyDetector::checkIfAnomalous(x,y,tempCF)) {
        return true;
    }
    Point p = Point(x,y);
    if (tempCF.corrlation > 0.5 && tempCF.corrlation < defaultThreshold && ((calcDistance(Point(tempCF.x, tempCF.y), p) > tempCF.threshold))) {
        return true;
    }
    return false;
}