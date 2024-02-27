//Author: 315097113 Noam Pdut
#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"

using namespace std;

class CLI {
	DefaultIO* dio;
	// you can add data members
    vector<Command*> commands;
public:
	CLI(DefaultIO* dio);
	void start();
	virtual ~CLI();

private:
    void createCommands();
};

#endif /* CLI_H_ */
