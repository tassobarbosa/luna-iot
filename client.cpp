#include <iostream>
#include "lib/json.hpp"
#include "src/keys.h"
#include "src/Client.hpp"
#include "src/DataHandler.hpp"

int main(){
    system("clear");

    Client client;

    client.startClient();
    client.mainLoop();
    
    return 0;
}