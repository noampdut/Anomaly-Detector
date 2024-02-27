//Author: 315097113 Noam Pdut
#include "CLI.h"

CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
    commands.push_back(new UploadCSV(dio));
    commands.push_back(new algorithmSettings(dio));
    commands.push_back(new detectAnomalies(dio));
    commands.push_back(new DisplayResults(dio));
    commands.push_back(new analyzeResult(dio));
    commands.push_back(new endOfMenu(dio));
}

void CLI::start() {
    CurrentData currentData;
    int numOfCommands = commands.size();
    while(true) {
        string startOfMenu = "Welcome to the Anomaly Detection Server.\nPlease choose an option:\n";
        this->dio->write(startOfMenu);
        for (int i = 0; i < numOfCommands; i++) {
            string commandToPrint = to_string((i + 1)) + "." + this->commands[i]->description;
            dio->write(commandToPrint);
        }
        string choose = dio->read();
        if (choose > "6" || choose < "1") {
            this->commands[5]->execute(&currentData);
        }
        int intChoose = stoi(choose);
        this->commands[intChoose-1]->execute(&currentData);
        if (intChoose == 6) {
            break;
        }
    }
}

//delete all the commands
CLI::~CLI() {
    for (int i = 0; i< this->commands.size(); i++){
        delete commands[i];
    }
}

