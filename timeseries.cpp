//Author: 315097113 Noam Pdut
#include "timeseries.h"
#include <map>
/*
 * name of class: TimeSeries.
 * argument: CSVFile- the file with the data: features and float numbers.
 * the constructor initialize the data map, with every feature as key and is vector.
 * the constructor also initialize the vector of feature to save all the keys by the same order as the order in the
 * CSVfile.
 */
TimeSeries::TimeSeries(const char *CSVfileName) {
    std::fstream file;
    std::string line, word, temp;
    std::vector <std::string> features;
    std::vector<std::vector<float>> vectorsOfFeatures;

    file.open(CSVfileName);
    getline(file, line);
    std::stringstream s(line);
    //get from the first line all the features names.
    while (getline(s,word,',')) {
        features.push_back(word);
    }
    unsigned long numOfFeatures = features.size();
    vectorsOfFeatures.assign(numOfFeatures, vector<float>());
    /*
     *get all the lines from the file, while save every float number in line in different vector, as the correct
     * feature.
     */
    while (!file.eof()) {
        getline(file, line);
        std::stringstream s(line);
        unsigned long i = 0;
        //get all the line.
        while (getline(s,word,',') && i != numOfFeatures) {
            vectorsOfFeatures[i].push_back(std::stof(word));
            i++;
        }
    }
    std::map <std::string, std::vector<float>> dataMap;
    //insert to the map the vectors to the correct key.
    for(int i = 0; i < numOfFeatures; i++) {
        dataMap.insert({features[i],vectorsOfFeatures[i]});
    }
    file.close();
    //initialized the members.
    vectorFeatures = features;
    data = dataMap;
}
/*
 * getter for the map member of TimeSeries.
 */
 map<string, vector<float>> TimeSeries::getMap() const{
    return data;
}

/*
 * getter for the vector of features member. the vector contain all the names of the features.
 */
 vector<string> TimeSeries::getFeatures() const{
    return vectorFeatures;
}

