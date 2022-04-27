#include "Client.hpp"	

Client::Client(){

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);	
    auth = false;
    
}

Client::~Client(){

}

int Client::menu(){
	int opt;
	cout<<"Select option\n"
	<<"1: Authenticate\t\t2: Read\t\t3: Write\r\n";
	cin >> opt;

	return opt;
}


int Client::readMenu(){
	int opt;
	cout<<"Select a sensor\n"
	<<"1: Temperature\t\t2: Date\t\t3: GPS\r\n";
	cin >> opt;

	return opt + 192;
}

string Client::requestInfo(int sensor){
	string packet;	

	packet = dt_handler.mountPacket(HEAD_REQUEST, addr, port, sensor, "1").dump() + '\0';
    send(sock, &packet[0], packet.size(), 0);
	cout << "SENT: " << packet << "\r\n";

	read(sock, buffer, 1024);
	cout << "RECV: " << buffer << "\r\n";

	return dt_handler.retrieveData(string(buffer));
}

bool Client::authenticateServer(){
	string packet;	

	packet = dt_handler.mountPacket(HEAD_HANDSHAKE, addr, port, HS_HELLO, "1").dump() + '\0';
    send(sock, &packet[0], packet.size(), 0);
	cout << "SENT: " << packet << "\r\n";

	read(sock, buffer, 1024);
	cout << "RECV: " << buffer << "\r\n";

	if (!dt_handler.validatePacket(string(buffer))) return false;
	//Recebi WELCOME?
	
	packet = dt_handler.mountPacket(HEAD_HANDSHAKE, addr, port, HS_REGISTER, "1").dump() + '\0';
    send(sock, &packet[0], packet.size(), 0);
	cout << "SENT: " << packet << "\r\n";

	read(sock, buffer, 1024);
	cout << "RECV: " << buffer << "\r\n";

	if (!dt_handler.validatePacket(string(buffer))) return false;
	//Recebi GRANT?

	cout << "AUTH SUCCESS!\r\n";
	return true;
}

void Client::startClient(){

	cout<<"Starting client...\r\n";

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return;
	}


	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
		printf("\nInvalid address/ Address not supported \n");
		return;
	}

	if (connect(sock, (struct sockaddr*)&serv_addr,sizeof(serv_addr))< 0) {
		printf("\nConnection Failed \n");
		return;
	}
	
	// valread = read(sock, buffer, 1024);					
	// printf("[Server] %s\r\n", buffer);


    struct sockaddr_in name;
    socklen_t namelen = sizeof(name);
	char buff[80];
    getsockname(sock, (struct sockaddr*)&name, &namelen);
    inet_ntop(AF_INET, &name.sin_addr, buff, 80);
	port = ntohs(name.sin_port);
	addr = string(buff);
	std::cout << "Local IP address is: " << addr <<":"<<port<< std::endl;

}

void Client::mainLoop(){
    
	string data;
	
	while(true){

		switch (menu()){
			case 1:
				if(!auth){
					auth = authenticateServer();
				}
				break;
			
			case 2:
				if(auth){
					int sensor = readMenu();
					data = requestInfo(sensor);
					cout<<"Data received: "<<data<<endl;

				} else {
					cout<<"Authenticate first!\r\n";
				}
				break;

			
			default:
				break;
		}		

		data.clear();
	}
}