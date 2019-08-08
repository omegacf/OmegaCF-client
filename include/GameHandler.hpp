#ifndef GAMEHANDLER_H
#define GAMEHANDLER_H

#include <string>
#include "network/DataHandlingService.hpp"
#include <stdint.h>

class GameHandler{
    private:
        short _playerNumber;
    public:
        GameHandler(std::string host, unsigned short port);
        void run();
        ~GameHandler();
};
#endif