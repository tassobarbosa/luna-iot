#include "Server.hpp"	

Server::Server(){ 
    auth_clients.empty();
}

Server::~Server(){

}

void Server::startServer(){
	printf("Initializing tcp Server...\n");

	max_clients = 30;

    for (i =0; i<max_clients; i++){
        client_socket[i] = 0;
    }

    if((master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0){  
        perror("socket failed");  
        exit(EXIT_FAILURE);  
    }

    //set master socket to allow multiple connections    
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 ){  
        perror("setsockopt");  
        exit(EXIT_FAILURE);  
    }

    address.sin_family = AF_INET;  
    address.sin_addr.s_addr = INADDR_ANY;  
    address.sin_port = htons( PORT ); 

    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0){  
        perror("bind failed");  
        exit(EXIT_FAILURE);  
    } 

    printf("Listener on port %d \n", PORT); 

    if (listen(master_socket, 3) < 0){  
        perror("listen");  
        exit(EXIT_FAILURE);  
    }  
             
    addrlen = sizeof(address);  
    puts("Server is ready to connect!");
	
}

void Server::mainLoop(){

    while(true){
        
        FD_ZERO(&readfds);               
        FD_SET(master_socket, &readfds);  
        max_sd = master_socket; 

        for ( i = 0 ; i < max_clients ; i++) {  
            sd = client_socket[i];  

            if(sd > 0)
				FD_SET( sd , &readfds);  
                 
            if(sd > max_sd)
				max_sd = sd;  
		}

		//WAIT indefinitely for activities on sockets
		activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);  
	
		if ((activity < 0) && (errno!=EINTR)){  
			printf("select error");  
		}  			
         

        if (FD_ISSET(master_socket, &readfds)){  
			new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen);
            if (new_socket < 0){  
                perror("accept");  
                exit(EXIT_FAILURE);  
            }
             
            printf("New connection, socket fd: %d, ip: %s, port: %d\n",
			 new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));                                                         
                             
            for (i = 0; i < max_clients; i++){  
                if( client_socket[i] == 0 ){  
                    client_socket[i] = new_socket;  
                    printf("Adding to list of sockets as %d\n", i);
                    break;  
                }  
            }  
        }

        for (i = 0; i < max_clients; i++) {  
            sd = client_socket[i];  
                 
            if (FD_ISSET( sd , &readfds)) { 
                valread = read( sd , buffer, 1024);
                buffer[valread] = '\0';
                cout << "RECV: " << buffer << "\r\n";
                
                if (dt_handler.validatePacket(string(buffer))) {                       
                    processData(string(buffer));

                } else {

                    getpeername(sd , (struct sockaddr*)&address, (socklen_t*)&addrlen);  
                    printf("Host disconnected, ip: %s, port: %d \n", inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

                    auth_clients.erase(makeKey(inet_ntoa(address.sin_addr),ntohs(address.sin_port)));      

                    close(sd);  
                    client_socket[i] = 0; 

                }  
            }  
        }	
	}

}

void Server::processData(string buffer){

    string addr, data;
    int header, command, port;

    json packet = json::parse(buffer);

    header = stoi(packet["header"].get<std::string>(), nullptr, 16);
    addr = packet["addr"].get<std::string>();
    port = packet["port"].get<int>();
    command = stoi(packet["command"].get<std::string>(), nullptr, 16);
    data = packet["data"].get<std::string>();
    
    switch (header){
        case HEAD_HANDSHAKE:
            authenticateClient(addr, port, command);
            break;
        case HEAD_REQUEST:
            if (isAuthenticated(addr, port))
                readSensor(addr, port, command);
            break;            
        default:

            break;
    }    
}

void Server::authenticateClient(string addr, int port, int command){
      
    string packet;

    switch (command) {
        case HS_HELLO:
            auth_clients.insert (std::pair<string,int>(makeKey(addr, port), HS_PENDING));
            packet = dt_handler.mountPacket(HEAD_HANDSHAKE, addr, port, HS_WELCOME, "1").dump() + '\0';
            send(sd, &packet[0], packet.size(), 0);
            cout << "SENT: " << packet << "\r\n";
            break;

        case HS_REGISTER:
            it = auth_clients.find(makeKey(addr, port));
            if (it != auth_clients.end()){
                auth_clients[makeKey(addr, port)] = HS_AUTH;
                packet = dt_handler.mountPacket(HEAD_HANDSHAKE, addr, port, HS_GRANT, "1").dump() + '\0';
                send(sd, &packet[0], packet.size(), 0);
                cout << "SENT: " << packet << "\r\n";
            }
            break;    

        default:
            break;
    }  

    cout << "Pool connection:\r\n";
    for (it=auth_clients.begin(); it!=auth_clients.end(); ++it){
        if(it->second == HS_AUTH) std::cout << it->first << " => GRANTED" << '\n';
        else std::cout << it->first << " => PENDING" << '\n';
    }    

}

bool Server::isAuthenticated(string addr, int port){
    it = auth_clients.find(makeKey(addr, port));
    if (it != auth_clients.end()){
        cout<<"CLI is authorized\r\n";
        return true;
    }    

    return false;
}

void Server::readSensor(string addr, int port, int command){
    string packet, data;    

    switch (command) {
        case SA_TEMPERATURE:
            cout<<"Fake reading TEMP sensor...\r\n";
            data = "30";
            break;
        case SA_DATE:
            cout<<"Fake reading RTC...\r\n";
            data = "27/04/22 20:25:27";
            break;            
        case SA_GPS:
            cout<<"Fake reading GPS...\r\n";
            data = "281546S522425O";
            break;
        default:
            break;
    }  


    packet = dt_handler.mountPacket(HEAD_REQUEST, addr, port, command, data).dump() + '\0';
    send(sd, &packet[0], packet.size(), 0);
    cout << "SENT: " << packet << "\r\n";    

}



string Server::makeKey(string addr, int port){
    return addr + ":" + to_string(port);
}