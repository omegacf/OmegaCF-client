#include "../../include/network/DataHandlingService.hpp"

int DataHandlingService::start(std::string host, unsigned short port){
    this->_networkClient = new NetworkClient(host, port);
    return this->_networkClient->connect();
}

void DataHandlingService::sendMessage(ClientNetworkMessage message){
    std::string networkMessage = message.getNetworkMessage();
    Debug::printLine("Hey iam sending");
    this->_networkClient->send(networkMessage);
}

ServerNetworkMessage DataHandlingService::receiveMessage(){
    //type
    std::string typeString = this->_networkClient->read(ServerNetworkMessage::typeSize);

    std::stringstream typeSStream;
    typeSStream << "Type: " << (uint8_t)typeString.at(0);

    Debug::printLine(typeSStream.str());

    //size
    std::string sizeString = this->_networkClient->read(ServerNetworkMessage::sizeSize);

    //TODO: Could be better
    unsigned int size = 0;
    int i = 0;
    for(char& c : sizeString){
        //size >>= 8;
        //size |= (unsigned int)c; 
        size += ((uint8_t)c)*(pow(256,(ServerNetworkMessage::sizeSize - 1 - i)));
        i++;
    }

    std::stringstream sizeSStream;
    sizeSStream << "Size: " << size;

    Debug::printLine(sizeSStream.str());

    //message
    std::string messageString = this->_networkClient->read(size);

    ServerNetworkMessage sm((NetworkMessageType)(uint8_t)typeString.at(0), size, messageString);
    return sm;
}

DataHandlingService::~DataHandlingService(){
    delete _networkClient;
}