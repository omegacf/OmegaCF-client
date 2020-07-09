#include "../include/GameHandler.hpp"

GameHandler::GameHandler(std::string host, unsigned short port){
    Debug::printLine("Init ai");
    
    if(DataHandlingService::getInstance().start(host, port) < 0){
        //cant connect to the server
        Debug::printLine("Problems connecting to the server");
        exit(0);
    }
    
}

void GameHandler::run(){
    Debug::printLine("Run AI");
    // receive playerNumber
    ServerNetworkMessage configurationMessage = DataHandlingService::getInstance().receiveMessage();
    while (configurationMessage.getMessageType() != NetworkMessageType::Configuration) {
        if(configurationMessage.getMessageType() == NetworkMessageType::Disqualification) {
            handleDisqualification(configurationMessage);
        } else {
            Debug::printLine("Unexpected message type!");
            exit(0);
        }
        configurationMessage = DataHandlingService::getInstance().receiveMessage();
    }
    // init player
    this->_playerNumber = configurationMessage.Move.playerNumber;

    // init game
    this->_game = GameFactory::create(configurationMessage.GameConfig.width, configurationMessage.GameConfig.height, configurationMessage.GameConfig.players);
    std::cout << this->_game.CurrentMap << std::endl;

    bool endOfGame = false;
    while(!endOfGame) {
        // receive message
        ServerNetworkMessage serverMessage = DataHandlingService::getInstance().receiveMessage();
        endOfGame = handleMessage(serverMessage);
    }
/*
    //this is for testing purposes only!!!
    this->_game.setStone(this->_game.Players[0], 0, this->_game.CurrentMap);
    this->_game.setStone(this->_game.Players[1], 0, this->_game.CurrentMap);

    for(PossibleMove move : this->_game.getPossibleMoves(this->_game.Players[0], this->_game.CurrentMap)){
        std::cout << "Possible Move: "  << move.Move << std::endl;
        std::cout << "Map (after it):" << std::endl;
        std::cout << move.AfterGrid << std::endl;
    }

    */
    
    Debug::printLine("End of the game. Bye!");
    exit(0);
}

bool GameHandler::handleMessage(ServerNetworkMessage message){
    bool endOfGame = false;
    switch (message.getMessageType())
    {
        case NetworkMessageType::Request:
            Debug::printLine("Request");
            handleMoveRequest(message);
            break;
        case NetworkMessageType::Move:
            Debug::printLine("Move");
            handleMove(message);
            break;
        case NetworkMessageType::Disqualification:
            Debug::printLine("Disqualification");
            handleDisqualification(message);
            break;
        case NetworkMessageType::EndGame:
            Debug::printLine("EndGame");
            endOfGame = true;
        default:
            Debug::printLine("receiving weird messages");
            endOfGame = true;
            break;
    }
    return endOfGame;
}

void GameHandler::handleMoveRequest(ServerNetworkMessage message) {
    std::vector<PossibleMove> possibleMoves = this->_game.getPossibleMoves(this->_game.Players[0], this->_game.CurrentMap);
    DataHandlingService::getInstance().sendMessage(ClientNetworkMessage((int8_t)possibleMoves.at(0).Move));
}

void GameHandler::handleMove(ServerNetworkMessage message) {
    int i = 0;
    for (Player player : this->_game.Players) {
        if(player.Id == message.Move.playerNumber) {
            break;
        }
        i++;
    }
    this->_game.setStone(this->_game.Players[i], message.Move.x, this->_game.CurrentMap);
}

void GameHandler::handleDisqualification(ServerNetworkMessage message) {
    uint8_t disNumber = message.getMessage().at(0);
    if (this->_playerNumber == disNumber) {
        Debug::printLine("DISQUALIFIED!");
        exit(0);
    }
}

GameHandler::~GameHandler(){
}