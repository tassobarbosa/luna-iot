#include <iostream>
#include "lib/json.hpp"
#include "src/keys.h"
#include "src/Server.hpp"
#include "src/DataHandler.hpp"

int main(){
    system("clear");
    Server server;

    server.startServer();
    server.mainLoop();
    
    return 0;
}