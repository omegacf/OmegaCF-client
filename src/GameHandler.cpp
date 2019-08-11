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
    this->_game.setStone(this->_game.Players[0], 0, this->_game.CurrentMap);
    this->_game.setStone(this->_game.Players[1], 0, this->_game.CurrentMap);

    for(PossibleMove move : this->_game.getPossibleMoves(this->_game.Players[0], this->_game.CurrentMap)){
        std::cout << "Possible Move: "  << move.Move << std::endl;
        std::cout << "Map (after it):" << std::endl;
        std::cout << move.AfterGrid << std::endl;
    }
    
    Debug::printLine("End of the game. Bye!");
}

GameHandler::~GameHandler(){
}