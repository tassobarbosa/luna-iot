#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h> 
#include <arpa/inet.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> 
#include <map>

#include "DataHandler.hpp"
#include "keys.h"

using namespace std;

class Server {
    public:
		Server();
		virtual ~Server();    

        int opt = 1;
        int master_socket, addrlen, new_socket, client_socket[30], max_clients, activity, i, valread, sd;
        int max_sd;
        struct sockaddr_in address;
            
        char buffer[1025];        
                    
        fd_set readfds;                                    


        DataHandler dt_handler;
        map <std::string, int> auth_clients;
        std::map<string, int>::iterator it;

        string power, date, wifi, bluetooth, camera, temperature, gps, accel, gyro;
        
        void startServer();
        void closeSockets();
        void mainLoop();

        void processData(string buffer);
        void authenticateClient(string addr, int port, int command);
        bool isAuthenticated(string addr, int port);
        void readSensor(string addr, int port, int command);
        void controlHardware(int command, string data);
        void ackPolling(string addr, int port, int command);

        string makeKey(string addr, int port);

    private:
                 
};

#endif