#include "../include/GameHandler.hpp"
#include "../include/util/FightClubTest.hpp"

GameHandler::GameHandler(std::string host, unsigned short port)
{
    Debug::printLine("Init ai");
    if(DataHandlingService::getInstance().start(host, port) < 0){
        //cant connect to the server
        exit(0);
    }
}

void GameHandler::run(int groupNumber)
{
    Debug::printLine("Run AI");
    //send groupNumber to server
    DataHandlingService::getInstance().sendMessage(ClientNetworkMessage(GroupNumber, groupNumber));
    //receive Map
    ServerNetworkMessage mapMessage = DataHandlingService::getInstance().receiveMessage();
    while(mapMessage.getMessageType() != NetworkMessageType::Map){
        if(mapMessage.getMessageType() == NetworkMessageType::Disqualification){
            handleDisqualification(mapMessage);
        }else{
            //unexpected message type
            Debug::printLine("Unexpected message type");
            return;
        }
        mapMessage = DataHandlingService::getInstance().receiveMessage();
    }
    //init game
    this->_mapGame =  MapGrid::initGrid(mapMessage.getMessage(), mapMessage.getBufferSize());

    if(Debug::getFlag()){
        this->_mapGame->printConsole();
    }

    //receive PlayerNumber
    ServerNetworkMessage playerNumberMessage = DataHandlingService::getInstance().receiveMessage();
    while(playerNumberMessage.getMessageType() != NetworkMessageType::PlayerNumber){
        if(playerNumberMessage.getMessageType() == NetworkMessageType::Disqualification){
            handleDisqualification(playerNumberMessage);
        }else{
            //unexpected message type
            Debug::printLine("Unexpected message type");
            return;
        }
        playerNumberMessage = DataHandlingService::getInstance().receiveMessage();
    }
    //init players
    this->_playerNumber = playerNumberMessage.getMessage().at(0);

    PlayerSimulateBot* ptrsimulateBot = new PlayerSimulateBot(this->_playerNumber);
    this->_player = std::shared_ptr<PlayerInterface>(ptrsimulateBot);
    ptrsimulateBot->setTiefe(2);

    //Phase one
    bool endOfGame = false;
    while(!endOfGame){
        //receive Message
        ServerNetworkMessage serverMessage = DataHandlingService::getInstance().receiveMessage();
        endOfGame = handleMessage(serverMessage);
    }

    Debug::printLine("End of the game. Bye!");
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
            move(message);
            break;
        case NetworkMessageType::Disqualification:
            Debug::printLine("Disqualification");
            handleDisqualification(message);
            break;
        case NetworkMessageType::EndOne:
            Debug::printLine("EndOne");
            this->_phaseOne = false;
            break;
        case NetworkMessageType::EndTwo:
            Debug::printLine("EndTwo");
            endOfGame = true;
        default:
            break;
    }
    return endOfGame;
}

void GameHandler::move(ServerNetworkMessage message){
    if(this->_phaseOne){
        this->_mapGame->execute(this->_mapGame->getGrid()->getUIndex(message.Move.x, message.Move.y), message.Move.playerNumber, message.Move.specialField);
        if(FightClubTest::getFlag){
            std::stringstream moveStringStream;
            moveStringStream << message.Move.x << message.Move.y << message.Move.specialField;

            std::vector<int> overrides = this->_mapGame->toOverrideVector(); 
            std::vector<int> bombs = this->_mapGame->toBombVector();
            JSONFormatter::addMove(moveStringStream.str(), message.Move.playerNumber+'0', this->_mapGame->toVector(), overrides, bombs);
        }
    }else{
        if(FightClubTest::getFlag){
            std::stringstream moveStringStream;
            moveStringStream << message.Move.x << message.Move.y << message.Move.specialField;

            std::vector<int> overrides = this->_mapGame->toOverrideVector(); 
            std::vector<int> bombs = this->_mapGame->toBombVector();
            JSONFormatter::addBombMove(moveStringStream.str(), message.Move.playerNumber+'0', this->_mapGame->toVector(), bombs);
        }
        this->_mapGame->executeBomb(this->_mapGame->getGrid()->getUIndex(message.Move.x, message.Move.y), message.Move.playerNumber);
    }    

    if(Debug::getFlag()){
        this->_mapGame->printConsole();
    }
}

void GameHandler::handleDisqualification(ServerNetworkMessage message){
    uint8_t disNumber = message.getMessage().at(0);
    if(this->_playerNumber == disNumber){
        Debug::printLine("Wir wurden disqualifiziert!");
        exit(0);
    }
}

void GameHandler::handleMoveRequest(ServerNetworkMessage message){
    uint32_t timeLimit = NetworkMessageConverter::convertFrom8To32(message.getMessage().at(0), message.getMessage().at(1), message.getMessage().at(2), message.getMessage().at(3));
    uint8_t maxDeep = message.getMessage().at(4);

    intSpielerId playerNumber = this->_playerNumber;
    char special = '\0';
    short x, y;
    short feldUIndex = -1; 
    if(this->_phaseOne){
        //calculate our move
        std::shared_ptr<MapOption> optionen = this->_mapGame->doEveryThing(playerNumber);
        this->_player->execute(feldUIndex, special, this->_mapGame, optionen, timeLimit, maxDeep);
    }else{
        this->_player->executeBomb(feldUIndex, this->_mapGame, timeLimit);
        if(FightClubTest::getFlag()){
            std::vector<std::string> validMoves;
            JSONFormatter::addBombMoveRequest(this->_mapGame->toVector(), this->_playerNumber+'0', validMoves);
        }
    }

    this->_mapGame->getGrid()->getXY(feldUIndex, x, y);
    //send the move
    if(special > 48){
        special -= 48;
    }
    DataHandlingService::getInstance().sendMessage(ClientNetworkMessage(x, y, special));
}


GameHandler::~GameHandler()
{
}