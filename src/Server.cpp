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
                
                if (dt_handler.validatePacket(string(buffer))) {                      
                    //send(sd , buffer , strlen(buffer) , 0 ); 
                    processData(string(buffer));

                } else {

                    getpeername(sd , (struct sockaddr*)&address, (socklen_t*)&addrlen);  
                    printf("Host disconnected, ip: %s, port: %d \n", inet_ntoa(address.sin_addr) , ntohs(address.sin_port)); 
                    auth_clients.erase(inet_ntoa(address.sin_addr));      

                    close(sd);  
                    client_socket[i] = 0; 

                }  
            }  
        }	
	}

}

void Server::processData(string buffer){

    string addr, data;
    int header, command;

    json packet = json::parse(buffer);

    header = stoi(packet["header"].get<std::string>(), nullptr, 16);
    addr = packet["addr"].get<std::string>();
    command = stoi(packet["command"].get<std::string>(), nullptr, 16);
    data = packet["data"].get<std::string>();
    
    switch (header){
        case HEAD_HANDSHAKE:
            authenticateClient(addr, command);
            break;  
        default:

            break;
    }    
}

void Server::authenticateClient(string addr, int command){
      
    string data;

    switch (command) {
        case HS_HELLO:
            auth_clients.insert (std::pair<string,int>(addr, HS_PENDING));
            data = dt_handler.mountPacket(HEAD_HANDSHAKE, addr, HS_WELCOME, "1").dump() + '\0';
            send(sd, &data[0], data.size(), 0);
            break;

        case HS_REGISTER:
            it = auth_clients.find(addr);
            if (it != auth_clients.end()){
                auth_clients[addr] = HS_AUTH;
                data = dt_handler.mountPacket(HEAD_HANDSHAKE, addr, HS_GRANT, "1").dump() + '\0';
                send(sd, &data[0], data.size(), 0);
            }
            break;    

        default:
            break;
    }  


  for (it=auth_clients.begin(); it!=auth_clients.end(); ++it)
    std::cout << it->first << " => " <<hex<< it->second << '\n';

}