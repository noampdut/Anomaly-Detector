//Author: 315097113 Noam Pdut

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"


using namespace std;

class DefaultIO{
public:
    DefaultIO(){}
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}

    // you may add additional methods here
    void readToFile(string fileName) {
        ofstream out(fileName);
        string s = read();
        while (s != "done\n") {
            out << s << endl;
            s = read();
        }
        out.close();
    }
};

struct sharedReports{
    int startTimeStep;
    int endTimeStep;
    string description;
    bool tp;
};

struct CurrentData{
    float threshold;
    vector<AnomalyReport> reports;
    vector<sharedReports> sReports;
    int numberOfRows;
    CurrentData(){
        threshold=0.9;
    }
};

// you may edit this class
class Command {
protected:
	DefaultIO* dio;
public:
    string description;
	Command(DefaultIO* dio):dio(dio){}
	virtual void execute(CurrentData* currentData)=0;
	virtual ~Command(){}
};

// implement here your command classes

class UploadCSV:public Command{
    public:
    UploadCSV(DefaultIO* dio): Command(dio){
        this->description = "upload a time series csv file\n";
    };

    void execute(CurrentData* currentData) {
        this->dio->write("Please upload your local train CSV file.\n");
        this->dio->readToFile("anomalyTrain.csv");
        this->dio->write("Upload complete.\n");
        this->dio->write("Please upload your local test CSV file.\n");
        this->dio->readToFile("anomalyTest.csv");
        this->dio->write("Upload complete.\n");
    }
};

class algorithmSettings: public Command{
    public:
    algorithmSettings(DefaultIO* dio): Command(dio){
        this->description = "algorithm settings\n";
    }
    void execute(CurrentData* currentData) {
        this->dio->write("The current correlation threshold is ");
        this->dio->write(currentData->threshold);
        this->dio->write("\n");
        this->dio->write("Type a new threshold\n");
        float choose;
        this->dio->read(&choose);
        if (choose > 0 && choose <= 1){
            currentData->threshold = choose;
            return;
        }
        this->dio->write("please choose a value between 0 and 1.\n");
        execute(currentData);
    }
};

class detectAnomalies: public Command{
    public:
    detectAnomalies(DefaultIO* dio): Command(dio){
        this->description = "detect anomalies\n";
    }
    void createSharedReports(CurrentData* currentData) {
        sharedReports sr;
        sr.description = "";
        sr.startTimeStep = 0;
        sr.endTimeStep = 0;
        sr.tp = false;
        for(int i = 0; i < currentData->reports.size();i++) {
            if (sr.description == currentData->reports[i].description && currentData->reports[i].timeStep == sr.endTimeStep +1) {
                sr.endTimeStep++;
            } else {
                currentData->sReports.push_back(sr);
                sr.description = currentData->reports[i].description;
                sr.startTimeStep = currentData->reports[i].timeStep;
                sr.endTimeStep = sr.startTimeStep;
            }
        }
        currentData->sReports.push_back(sr);
        currentData->sReports.erase(currentData->sReports.begin());
    }

    void execute(CurrentData* currentData) {
        TimeSeries timeSeries1 = TimeSeries("anomalyTrain.csv");
        TimeSeries timeSeries2 = TimeSeries("anomalyTest.csv");
        HybridAnomalyDetector hybridAnomalyDetector;
        hybridAnomalyDetector.setDefaultThreshold(currentData->threshold);
        hybridAnomalyDetector.learnNormal(timeSeries1);
        currentData->reports = hybridAnomalyDetector.detect(timeSeries2);
        currentData->numberOfRows = hybridAnomalyDetector.numberOfRows;
        createSharedReports(currentData);
        this->dio->write("anomaly detection complete.\n");
    }
};


class DisplayResults: public Command {
    public:
    DisplayResults(DefaultIO* dio): Command(dio){
        this->description = "display results\n";
    }
    void execute(CurrentData* currentData) {
        for(int i=0; i < currentData->reports.size(); i++) {
            this->dio->write(to_string(currentData->reports[i].timeStep) + "\t" + currentData->reports[i].description + "\n");
        }
        this->dio->write("Done.\n");
    }
};


class analyzeResult: public Command {
    public:
    analyzeResult(DefaultIO *dio) : Command(dio) {
        this->description = "upload anomalies and analyze results\n";
    }

    bool OverlapTP(int start, int end, CurrentData* currentData) {
        for(int i = 0; i < currentData->sReports.size();i++) {
            if (end >= currentData->sReports[i].startTimeStep && start <= currentData->sReports[i].endTimeStep) {
                currentData->sReports[i].tp = true;
                return true;
            }
        }
        return false;
    }

    void execute(CurrentData *currentData) {
        this->dio->write("Please upload your local anomalies file.\n");
        for(int i=0;i<currentData->sReports.size();i++){
            currentData->sReports[i].tp=false;
        }

        float p = 0;
        float tp = 0;
        float fp = 0;
        int sumOfInputLines = 0;
        string line = dio->read();
        while (line != "done\n") {
            int index = line.find(",");
            int start = stoi(line.substr(0, index));
            int end = stoi(line.substr(index + 1, line.length()));
            if (OverlapTP(start, end, currentData))
                tp++;
            sumOfInputLines += end + 1 - start;
            p++;
            line = dio->read();
        }

        for(int i = 0; i < currentData->sReports.size();i++) {
            if(currentData->sReports[i].tp == false)
                fp++;
        }

        float N = currentData->numberOfRows - sumOfInputLines;
        float tpP = ((int)(1000.0*tp/p))/1000.0f;
        float fpP = ((int)(1000.0*fp/N))/1000.0f;

        this->dio->write("Upload complete.\n");
        this->dio->write("True Positive Rate: ");
        this->dio->write(tpP);
        this->dio->write("\n");
        this->dio->write("False Positive Rate: ");
        this->dio->write(fpP);
        this->dio->write("\n");
    }
};

class endOfMenu: public Command {
    public:
    endOfMenu(DefaultIO* dio): Command(dio){
        this->description = "exit\n";
    }
    void execute(CurrentData* currentData) {}
};
/*
class StandardIO:public DefaultIO {
    StandardIO(): DefaultIO(){};

    void write(string text) {
        cout << "" << text;
    }
    void write(float f) {
        cout << "" << f;
    }
    string read() {

    }
    void closeDio() {
        return;
    }
};*/

#endif /* COMMANDS_H_ */
