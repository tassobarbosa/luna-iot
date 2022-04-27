#include <iostream>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "DataHandler.hpp"
#include "keys.h"

using namespace std;

class Client {
    public:
        Client();
		virtual ~Client();

    	int sock = 0, valread;	
        char buffer[1024] = { 0 };

        bool auth;
        string addr;
        int port;
        DataHandler dt_handler;

        struct sockaddr_in serv_addr;        

        void startClient();
        void mainLoop();

        int menu();
        int readMenu();

        bool authenticateServer();
        string requestInfo(int sensor);

};