#include "../../include/network/NetworkMessage.hpp"

unsigned int NetworkMessage::getBufferSize(){
    return this->_size;
}

std::string NetworkMessage::getNetworkMessage(){
    char cp[NetworkMessage::typeSize + NetworkMessage::sizeSize];
    //type
    *cp = this->_type;

    //size
    cp[1] = this->_size;

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
    char cp[this->_size];
    *cp = x;

    this->_message = std::string(cp, this->_size);
}

ServerNetworkMessage::ServerNetworkMessage(std::string serverMessage){

}

ServerNetworkMessage::ServerNetworkMessage(NetworkMessageType type, unsigned int size, std::string serverMessage){
    this->_type = type;
    this->_size = size;
    
    this->_message = serverMessage;

    switch (type)
    {
        case NetworkMessageType::Configuration:
            this->GameConfig.playerNumber = serverMessage.at(0);
            break;
        case NetworkMessageType::Move:
            this->Move.x = serverMessage.at(0);
            this->Move.playerNumber = serverMessage.at(1);
            break;
        case NetworkMessageType::EndGame:
            this->EndGame.playerNumber = serverMessage.at(0);
            break;
        default:
            break;
    }
}