#include <iostream>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <map>

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
        map <int, std::string> sensor_state;

        struct sockaddr_in serv_addr;        

        void startClient();
        void mainLoop();

        int menu();
        int readMenu();
        int writeMenu();
        void showReadings();

        bool authenticateServer();
        string requestInfo(int sensor);        
        void writeInfo(int sensor, int cmd);

};