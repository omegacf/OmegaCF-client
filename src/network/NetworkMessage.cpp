#include "../../include/network/NetworkMessage.hpp"

std::string NetworkMessage::getNetworkMessage()
{
    //TODO stringSize is unused, probably missing terminating zero, maybe change to char cp[1000] {0};
    char cp[NetworkMessage::typeSize + NetworkMessage::sizeSize];

    //type
    cp[0] = this->_type;
    //size
    cp[1] = this->_size;

    std::stringstream ss;
    ss << std::string(cp, NetworkMessage::typeSize + NetworkMessage::sizeSize) << this->_message;

    return ss.str();
}

unsigned int NetworkMessage::getLength()
{
    return ServerNetworkMessage::typeSize + ServerNetworkMessage::sizeSize + this->_size;
}

NetworkMessageType NetworkMessage::getMessageType()
{
    return this->_type;
}

std::string NetworkMessage::getMessage()
{
    return this->_message;
}

NetworkMessage::~NetworkMessage()
{

}

ClientNetworkMessage::ClientNetworkMessage(NetworkMessageType type, int8_t column)
{
    this->_type = type;
    this->_size = 1;
    // char pointer for payload
    char cp[this->_size];
    *cp = column;

    this->_message = std::string(cp, this->_size);
}

ServerNetworkMessage::ServerNetworkMessage(NetworkMessageType type, unsigned int size, std::string serverMessage)
{
    this->_type = type;
    this->_size = size;

    this->_message = serverMessage;

    switch (type)
    {
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