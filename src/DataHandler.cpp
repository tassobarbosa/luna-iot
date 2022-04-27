#include "DataHandler.hpp"

DataHandler::DataHandler(){    
       
}

DataHandler::~DataHandler(){
    
};

bool DataHandler::validatePacket(string buffer){
    json packet;
 
    try {        
        packet = json::parse(buffer);
        packet["header"].get<std::string>();
        packet["addr"].get<std::string>();
        packet["port"].get<int>();
        packet["command"].get<std::string>();
        packet["data"].get<std::string>();

    } catch(...){
        cout << "Exception: error packet\r\n";
        return false;
    }

    return true;
}

json DataHandler::mountPacket(int header, string addr, int port, int command, string data){
    json packet;    
    char hex_string[4];

    sprintf(hex_string, "0x%X", header);
    packet["header"] = hex_string;
    packet["addr"] = addr;
    packet["port"] = port;
    sprintf(hex_string, "0x%X", command);
    packet["command"] = hex_string;     
    packet["data"] = data;

    return packet;
}

string DataHandler::retrieveData(string buffer){
    json packet;
 
    try {        
        packet = json::parse(buffer);
        return packet["data"].get<std::string>();

    } catch(...){        
        return "Exception: error packet\r\n";
    }
}