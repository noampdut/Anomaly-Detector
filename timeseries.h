//Author: 315097113 Noam Pdut

#ifndef TIMESERIES_H_
#define TIMESERIES_H_
#include <map>
#include <bits/stdc++.h>

using namespace std;

class TimeSeries {

public:
    map<string,vector<float>> data;
    vector<std::string> vectorFeatures;

	TimeSeries(const char* CSVfileName);
    map<string, vector<float>> getMap()const;
    vector<string> getFeatures()const;
};

#endif /* TIMESERIES_H_ */
