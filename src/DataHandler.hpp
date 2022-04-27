#ifndef DataHandler_HPP_
#define DataHandler_HPP_

#include <iostream>

#include "../lib/json.hpp"
#include "keys.h"

using json = nlohmann::json;
using namespace std;

class DataHandler {
  public:
    DataHandler();
    virtual ~DataHandler();        

    bool validatePacket(string buffer);
    json mountPacket(int header, string addr, int port, int command, string data);
    string retrieveData(string buffer);

  private:

};

#endif