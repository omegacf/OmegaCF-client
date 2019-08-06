#include "../include/GameHandler.hpp"

GameHandler::GameHandler(std::string host, unsigned short port){
    Debug::printLine("Init ai");
    if(DataHandlingService::getInstance().start(host, port) < 0){
        //cant connect to the server
        exit(0);
    }
}

void GameHandler::run(){
    Debug::printLine("Run AI");

    Debug::printLine("End of the game. Bye!");
}

GameHandler::~GameHandler(){
}