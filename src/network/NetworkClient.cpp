#include "../../include/network/NetworkClient.hpp"

int NetworkClient::connect(){
    if ((_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        std::cout << "Socket creation error" << std::endl;
        exit(0); 
    } 
   
    memset(&_server, '0', sizeof(_server)); 
   
    _server.sin_family = AF_INET; 
    _server.sin_port = htons(_port); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, _address.c_str(), &_server.sin_addr)<=0)  
    { 
        std::cout << "Invalid address" << std::endl;
        exit(0); 
    } 
   
    //try to connect
    std::stringstream ss;
    ss << "Try to connect to: " << _address << ':' << _port << std::endl;
    Debug::print(ss.str());

    if (::connect(_sock, (struct sockaddr *)&_server, sizeof(_server)) < 0) 
    { 
        std::cout << "connection failed" << std::endl;
        return -1; 
    }


    return 0; 
}

std::string NetworkClient::read(int size){
    //Receive a reply from the server
    char mp[size];
	if(size > 0 && ::recv(_sock, mp, size , 0) < 0)
	{
        Debug::printLine("recv failed");
        return "";
	}
    std::string s = std::string(mp, size);
    return s;
}

void NetworkClient::send(std::string message){
    if(_sock){
        ::send(_sock, message.c_str(), message.length(), 0);
        std::stringstream ss;
        ss << "NetworkClient: Send message to server" << std::endl;
        for(char& c : message){
            ss << (int)c << '\t';
        }
        Debug::printLine(ss.str());
    }
}

NetworkClient::~NetworkClient(){
    _sock = 0;
}
