#include "../include/GameHandler.hpp"

GameHandler::GameHandler(std::string host, unsigned short port){
    Debug::printLine("Init ai");
    /*
    if(DataHandlingService::getInstance().start(host, port) < 0){
        //cant connect to the server
        exit(0);
    }
    */
}

void GameHandler::run(){
    Debug::printLine("Run AI");
    //this is for testing purposes only!!!
    this->_game = GameFactory::create(10, 10, 2);
    
    if(Debug::getFlag()){
        std::cout << this->_game.CurrentMap;
    }
    
    Debug::printLine("End of the game. Bye!");
}

GameHandler::~GameHandler(){
}