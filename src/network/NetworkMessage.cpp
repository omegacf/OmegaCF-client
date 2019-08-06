#include "../../include/network/NetworkMessage.hpp"

unsigned int NetworkMessage::getBufferSize(){
    return this->_size;
}

std::string NetworkMessage::getNetworkMessage(){
    int stringSize = ServerNetworkMessage::typeSize + ServerNetworkMessage::sizeSize + this->_size;
    char cp[NetworkMessage::typeSize + NetworkMessage::sizeSize];
    //type
    *cp = this->_type;

    //size
    int s = this->_size;
    cp[1] = (s >> 24) & 0xFF;
    cp[2] = (s >> 16) & 0xFF;
    cp[3] = (s >> 8) & 0xFF;
    cp[4] = s & 0xFF;

    std::stringstream ss;
    ss << std::string(cp, NetworkMessage::typeSize + NetworkMessage::sizeSize) << this->_message;

    return ss.str();
}

unsigned int NetworkMessage::getLength(){
    return ServerNetworkMessage::typeSize + ServerNetworkMessage::sizeSize + this->_size;
}

NetworkMessageType NetworkMessage::getMessageType(){
    return this->_type;
}

std::string NetworkMessage::getMessage(){
    return this->_message;
}

NetworkMessage::~NetworkMessage(){

}

ClientNetworkMessage::ClientNetworkMessage(NetworkMessageType type, int8_t number){
    this->_type = type;
    this->_size = 1; //because it's always just 1 Byte

    char m = number;
    this->_message = std::string(1, m);
}

ClientNetworkMessage::ClientNetworkMessage(int16_t x, int16_t y, int8_t specialField){
    this->_type = NetworkMessageType::Answer;
    this->_size = 5; //5 Byte => 2(x) + 2(y) + 1(sF)

    char c[this->_size];

    NetworkMessageConverter::convertFrom16To8((int8_t*)c, x);
    NetworkMessageConverter::convertFrom16To8((int8_t*)(c+2), y);
    *(c+4) = specialField;

    this->_message = std::string(c, this->_size);
}

ServerNetworkMessage::ServerNetworkMessage(std::string serverMessage){

}

ServerNetworkMessage::ServerNetworkMessage(NetworkMessageType type, unsigned int size, std::string serverMessage){
    this->_type = type;
    this->_size = size;
    
    this->_message = serverMessage;

    if(type == NetworkMessageType::Move || type == NetworkMessageType::Answer){
        this->Move.x = (serverMessage.at(0) * 256) + serverMessage.at(1);
        this->Move.y = (serverMessage.at(2) * 256) + serverMessage.at(3);
        this->Move.specialField = serverMessage.at(4);

        if(type == NetworkMessageType::Move){
            this->Move.playerNumber = serverMessage.at(5);
        }

    }
}