//Author: 315097113 Noam Pdut

#ifndef SERVER_H_
#define SERVER_H_
#include "CLI.h"
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <pthread.h>
#include <thread>
#include <unistd.h>
#include <sstream>


using namespace std;
class socketIO:public DefaultIO{
    int clientId;

public:
    socketIO(int clientId):DefaultIO(){this->clientId = clientId;}
    virtual string read();
    virtual void write(string text);
    virtual void write(float f);
    virtual void read(float* f);
};

class ClientHandler{
    public:
    virtual void handle(int clientID)=0;
};

class AnomalyDetectionHandler:public ClientHandler{
	public:
    virtual void handle(int clientID);
};

class Server {
    bool stopped;
    int serverSocket;
	thread* t;
    sockaddr_in serverStruct;
    sockaddr_in clientStruct;
public:
	Server(int port) throw (const char*);
	virtual ~Server();
	void start(ClientHandler& ch)throw(const char*);
	void stop();
};

#endif /* SERVER_H_ */
