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

ClientNetworkMessage::ClientNetworkMessage(int8_t x){
    this->_type = NetworkMessageType::Answer;
    this->_size = 1;

    this->_message = std::string((char)x, this->_size);
}

ServerNetworkMessage::ServerNetworkMessage(std::string serverMessage){

}

ServerNetworkMessage::ServerNetworkMessage(NetworkMessageType type, unsigned int size, std::string serverMessage){
    this->_type = type;
    this->_size = size;
    
    this->_message = serverMessage;

    if(type == NetworkMessageType::Configuration) {
        this->GameConfig.width = serverMessage.at(0);
        this->GameConfig.height = serverMessage.at(1);
        this->GameConfig.players = serverMessage.at(2);
        this->GameConfig.playerNumber = serverMessage.at(3);
    }

    if(type == NetworkMessageType::Move){
        this->Move.x = serverMessage.at(0);
        this->Move.playerNumber = serverMessage.at(1);
    }
}