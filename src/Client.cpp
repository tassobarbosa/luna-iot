#include "Client.hpp"	

Client::Client(){

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);	
    auth = false;
    
	sensor_state = {
		{ SA_POWER, "none"},
		{ SA_TEMPERATURE, "none"},
		{ SA_DATE, "none"},
		{ SA_GPS, "none"},
		{ SA_ACCEL, "none"},
		{ SA_GYRO , "none"},
		{ SA_CAMERA, "none"},
		{ SA_BLUETOOTH, "none"},
		{ SA_WIFI, "none"},
		};

}

Client::~Client(){

}

int Client::menu(){
	int opt;
	cout<<"\nSelect option\n"
		<<"1: Auth     2: Read     3: Write     4: Show\n"
		<<"=> ";
	cin >> opt;
	cout<<"\r\n";

	return opt;
}


int Client::readMenu(){
	int opt;
	cout<<"Select a sensor\n"
	<<"1: Wifi       2: Bluetooth  3: Camera\n"
	<<"4: Date       5: Temp       6: GPS\n"
	<<"7: Accel      8: Gyro       9: All\n"
	<<"=> ";
	cin >> opt;	
	cout<<"\r\n";

	return opt + 192;
}

int Client::writeMenu(){
	int opt;
	cout<<"Select a module\n"
	<<"0: Power          1: Wifi\n"
	<<"2: Bluetooth      3: Camera\n"
	<<"=> ";
	cin >> opt;	
	cout<<"\r\n";

	return opt + 192;
}

void Client::showReadings(){
	cout<<"+---------------------------------+\n"
	    <<"| Power:        "<<sensor_state[SA_POWER]      <<"\n"
		<<"| Wifi:         "<<sensor_state[SA_WIFI]       <<"\n"
		<<"| Bluetooth:    "<<sensor_state[SA_BLUETOOTH]  <<"\n"
		<<"| Camera:       "<<sensor_state[SA_CAMERA]	  <<"\n"
		<<"| Date:         "<<sensor_state[SA_DATE]	      <<"\n"
		<<"| Temperature:  "<<sensor_state[SA_TEMPERATURE]<<"\n"		
		<<"| GPS:          "<<sensor_state[SA_GPS]        <<"\n"  
		<<"| Accel:        "<<sensor_state[SA_ACCEL]      <<"\n"
		<<"| Gyro:         "<<sensor_state[SA_GYRO]       <<"\n"
		<<"+---------------------------------+\r\n\n";				
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

void Client::writeInfo(int sensor, int cmd){
	string packet;	

	string data = cmd ? "on" : "off";

	packet = dt_handler.mountPacket(HEAD_WRITE, addr, port, sensor, data).dump() + '\0';
    send(sock, &packet[0], packet.size(), 0);
	cout << "SENT: " << packet << "\r\n";

	sensor_state[sensor] = data;
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
	sensor_state[SA_POWER] = "on";
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

	//Get my credentials
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
			case 1: //AUTH
				if(!auth){
					auth = authenticateServer();
				}
				break;
			
			case 2: //READ
				if(auth){
					int sensor = readMenu();
					data = requestInfo(sensor);
					sensor_state[sensor] = data;
				} else {
					cout<<"Authenticate first!\r\n";
				}
				break;
			
			case 3: //WRITE
				if(auth){
					int sensor = writeMenu();
					cout<<"0: off\t\t1: on\r\n";
					int cmd;
					cin >> cmd;
					writeInfo(sensor, cmd);
				} else {
					cout<<"Authenticate first!\r\n";
				}				
				break;
			
			case 4: //SHOW
				showReadings();
				break;

			
			default:
				break;
		}		
		data.clear();
	}
}