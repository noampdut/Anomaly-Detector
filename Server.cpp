//Author: 315097113 Noam Pdut
#include "Server.h"

Server::Server(int port)throw (const char*) {
    serverSocket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (serverSocket < 0) {
        throw ("socket failed.");
    }

    this->serverStruct.sin_family = AF_INET;
    this->serverStruct.sin_addr.s_addr =INADDR_ANY;
    this->serverStruct.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr*)&serverStruct, sizeof(serverStruct)) < 0) {
        throw ("socket bind failed.");
    }
    if (listen(serverSocket, 3) < 0) {
        throw ("socket listen failed");
    }
    this->stopped = false;
}

void Server::start(ClientHandler& ch)throw(const char*) {
    this->t = new thread([&ch, this](){
            while(!this->stopped) {
                this->clientStruct = {};
                socklen_t sizeClient = sizeof(this->clientStruct);
                int clientId = accept(this->serverSocket, (struct sockaddr*)&this->clientStruct,&sizeClient);
                if (clientId > 0) {
                    ch.handle(clientId);
                    close(clientId);
                }
            }
        close(this->serverSocket);
    });
}

void Server::stop() {
    this->stopped = true;
	this->t->join(); // do not delete this!
}

Server::~Server() {}

string socketIO:: read() {
    char currentChar = 0;
    string str = "";
    while (currentChar != '\n') {
        recv(this->clientId, &currentChar, sizeof(char), 0);
        str+=currentChar;
    }
    return str;
}

void socketIO::write(string text) {
    const char* buffer = text.c_str();
    send(this->clientId,buffer,strlen(buffer), 0);
}


void socketIO::write(float f){
    ostringstream sStream;
    sStream << f;
    string str(sStream.str());
    write(str);
}

void socketIO:: read(float* f) {
    recv(this->clientId, f, sizeof(float), 0);
}

void AnomalyDetectionHandler::handle(int clientID) {
    socketIO sio(clientID);
    CLI cli(&sio);
    cli.start();
}