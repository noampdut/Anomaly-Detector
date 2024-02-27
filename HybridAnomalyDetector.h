//Author: 315097113 Noam Pdut

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();
    virtual void createCorreletedFeature(const TimeSeries& ts, int sizeOfVector, float m, int c,string feature1, string feature2
            ,Point** pointArr);
    virtual bool checkIfAnomalous(float x, float y, correlatedFeatures tempCF);
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
