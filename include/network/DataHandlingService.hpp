#ifndef DATAHANDLINGSERVICE_H
#define DATAHANDLINGSERVICE_H

#include <string>
#include "NetworkMessage.hpp"
#include "NetworkClient.hpp"
#include <math.h>
#include <stdlib.h>

class DataHandlingService{
    private:
        NetworkClient* _networkClient;

        DataHandlingService(){};
        DataHandlingService(DataHandlingService const&); // Don't Implement.
        void operator=(DataHandlingService const&); // Don't implement.
    public:
        static DataHandlingService& getInstance()
        {
            static DataHandlingService instance;
            return instance;
        }
        int start(std::string host, unsigned short port);
        void sendMessage(ClientNetworkMessage message);
        ServerNetworkMessage receiveMessage();
        ~DataHandlingService();
};
#endif