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
        if(configurationMessage.getMessageType() == NetworkMessageType::EndGame) {
            handeEndGame(configurationMessage);
        } else {
            Debug::printLine("Unexpected message type!");
            exit(0);
        }
        configurationMessage = DataHandlingService::getInstance().receiveMessage();
    }
    // init player
    this->_playerNumber = configurationMessage.GameConfig.playerNumber;
    std::stringstream ss;
    ss << "Got playerNumber from server: " << (int) configurationMessage.GameConfig.playerNumber;
    Debug::printLine(ss.str());

    // init game
    // widht height amountOfPlayers
    this->_game = GameFactory::create(7, 6, 2);

    this->_bmc = BestMoveCalculator(this->_game, this->getPlayer(this->_playerNumber));

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
        case NetworkMessageType::EndGame:
            Debug::printLine("EndGame");
            handeEndGame(message);
            endOfGame = true;
            break;
        default:
            Debug::printLine("receiving weird messages");
            endOfGame = true;
            break;
    }
    return endOfGame;
}

void GameHandler::handleMoveRequest(ServerNetworkMessage message) {
    PossibleMove move = this->_bmc.minimax(this->_game.CurrentMap, 4, true);
    Debug::printLine("Move to send to the server:");
    Debug::printLine(std::to_string(move.Move));
    DataHandlingService::getInstance().sendMessage(ClientNetworkMessage((int8_t)move.Move));
}

void GameHandler::handleMove(ServerNetworkMessage message) {
    this->_game.setStone(this->getPlayer(message.Move.playerNumber), message.Move.x, this->_game.CurrentMap);

    std::stringstream ss;
    ss << this->_game.CurrentMap;
    Debug::printLine(ss.str());
}

void GameHandler::handeEndGame(ServerNetworkMessage message) {
    if(message.EndGame.playerNumber == 0) {
        Debug::printLine("Draw!");
    } else {
        Debug::printLine("We have a winner: ");
        Debug::printLine(std::to_string(message.EndGame.playerNumber));
        if(message.EndGame.playerNumber == this->_playerNumber) {
            // We have won!
            Debug::printLine("We are the champions");
        }
    }
}

Player GameHandler::getPlayer(int8_t playerNumber){
    for (Player player : this->_game.Players) {
        if(player.Id == playerNumber) {
            return player;
        }
    }
    Debug::printLine("Player not found!");
    Debug::printLine("Continue with player 1");
    return Player();
}

GameHandler::~GameHandler(){
}