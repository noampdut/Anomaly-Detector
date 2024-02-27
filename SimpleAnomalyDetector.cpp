//Author: 315097113 Noam Pdut
#include "SimpleAnomalyDetector.h"
#include <map>

SimpleAnomalyDetector::SimpleAnomalyDetector() {}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {}

void SimpleAnomalyDetector::setDefaultThreshold(float newThreshold){
    this->defaultThreshold = newThreshold;
}

void SimpleAnomalyDetector::setNumberOfRows(int number) {
    this->numberOfRows = number;
}

/*
 * convert the vectors of the two correlated fetters into Points array.
 */
Point** vectorsToPoints(vector<float> feature1, vector<float> feature2) {
    Point** p = new Point*[feature1.size()];
    for (int i = 0; i < feature1.size(); i++){
        p[i] = new Point(feature1[i],feature2[i]);
    }
    return p;
}

/*
 * delete the points array of every correlated features.
 */
void deletePoint(Point** p, int sizeArray) {
    for (int i = 0; i < sizeArray; i++) {
        delete p[i];
    }
    delete [] p;
}

void SimpleAnomalyDetector::createCorreletedFeature(const TimeSeries& ts, int sizeOfVector, float m, int c,
                                                    string feature1, string feature2, Point** pointArr) {
   // map<string,vector<float>> data= ts.getMap();
    //check if there is correlation with feature i, and if so, check if it is higher than defaultThreshold.
    if(m > defaultThreshold) {
        //create correlated feature struct and push it to cf vector member.
        correlatedFeatures cfTemp;
        cfTemp.feature1 = feature1;
        cfTemp.feature2 = feature2;
        cfTemp.corrlation = m;
        cfTemp.lin_reg = linear_reg(pointArr, sizeOfVector);
        cfTemp.threshold = maxOffset(pointArr, sizeOfVector, cfTemp.lin_reg) * (float)1.1;
        cf.push_back(cfTemp);
    }
}

/*
 * the function calculate the correletion between every feature by getting a normal data of TimeSeries.
 */
void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts) {
    map<string,vector<float>> data= ts.getMap();
    vector<string> vectorOfFeatures = ts.getFeatures();
    int numberOfFeatures = vectorOfFeatures.size();
    int sizeOfVector = data[vectorOfFeatures[1]].size(); //size of the rows.
    //this->setNumberOfRows(sizeOfVector);
    Point** pointArr;

    /*
     * implement the pseudocode of finding the most correlative pairs of features.
     */
    for (int i = 0; i < numberOfFeatures; i ++) {
        float m = 0;
        int c = 0;
        //run over all the others features.
        for (int j = i+1; j< numberOfFeatures; j++) {
            //get the vectors as array.
            float* arrI = &data[vectorOfFeatures[i]][0];
            float* arrJ = &data[vectorOfFeatures[j]][0];
            //get the correlation between the current features.
            float p = std::abs(pearson(arrI, arrJ, sizeOfVector));
            //check if the current features is the most correlated to features i.
            if(p > m) {
               m = p;
               c = j;
           }
        }
        //create array of points, to get the linear reg.
        pointArr = vectorsToPoints(data[vectorOfFeatures[i]], data[vectorOfFeatures[c]]);
        createCorreletedFeature(ts, sizeOfVector, m, c, vectorOfFeatures[i], vectorOfFeatures[c], pointArr);
        deletePoint(pointArr, sizeOfVector);
    }
}

bool SimpleAnomalyDetector::checkIfAnomalous(float x, float y, correlatedFeatures tempCF) {
    //get the dev between the excepted y and the current y.
    float temp = abs(y-tempCF.lin_reg.f(x));
    return (temp > tempCF.threshold);
}

/*
 * detect all the deviations of the Time Series, by checking for every row if there is deviation between two correlated
 * features.
 */
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts) {
    map<string,vector<float>> data= ts.getMap();
    vector<string> vectorOfFeatures = ts.getFeatures();
    int sizeOfVector = data[vectorOfFeatures[1]].size(); //number of rows
    this->setNumberOfRows(sizeOfVector);
    vector<AnomalyReport> reportsVector;

    //run over all the lines.
    for(int i = 0; i < sizeOfVector; i++) {
        //run over all the correlated features in the row
        for(int j = 0; j < cf.size(); j++) {
            //get the x,y values of the two features in the current line.
            float x = data[cf[j].feature1][i];
            float y = data[cf[j].feature2][i];

            //check if there is deviation
            if (checkIfAnomalous(x,y,cf[j])) {
                //create an anomaly report and add it into a reports vector.
                AnomalyReport report = AnomalyReport(cf[j].feature1+ "-" +cf[j].feature2, i+1);
                reportsVector.push_back(report);
            }
        }
    }
    return reportsVector;
}

float SimpleAnomalyDetector::maxOffset(Point** pointArr, int sizeOfVector, Line lin_reg) {
    float maxOffset = 0;
    for (int i = 0; i < sizeOfVector; i++) {
        float y = pointArr[i]->y;
        float x = pointArr[i]->x;
        float temp = abs(y-lin_reg.f(x));
        if (temp > maxOffset) {
            maxOffset = temp;
        }
    }
    return maxOffset;
}

