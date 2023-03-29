#include "../include/GameHandler.hpp"
#include "../include/util/Debug.hpp"
#include "../include/util/enums.hpp"
#include <iostream>
#include <string>
#include <exception>


#include "../include/ml/networkAgent.hpp"
#include "../include/ml/network.hpp"
#include "../include/ml/qLearningAgent.hpp"
#include <tuple>
#include <utility>
#include "../include/ml/replayMemory.hpp"
#include "../include/game/Game.hpp"
#include "../include/ml/trainer.hpp"

static void printHelp(){
	std::cout << "-i --ip\t IP-Address (default 127.0.0.1)" << std::endl;
	std::cout << "-p --port\t Port (default 7777)" << std::endl;
	std::cout << "-q --quiet\t Disable all output" << std::endl; 
	std::cout << "-b --batchSize\t Define size of batch" << std::endl;
	std::cout << "-r --replayMemorySize\t Define size of replay memory" << std::endl;
	std::cout << "-m --mode\t Application Mode: training, gaming" << std::endl;
	std::cout << "-pl --player\t Player: minmax, random, neuronal" << std::endl;
	std::cout << "-t --trainingSize\t Define size of training loops" << std::endl;
	std::cout << "-h --help\t print help" << std::endl;
}

int main(int argc, char *argv[]) {

	Game game = GameFactory::create(7, 6, 2);
	// create test board
	game.setStone(game.getPlayer(1), 0, game.CurrentMap);
	game.setStone(game.getPlayer(1), 0, game.CurrentMap);
	game.setStone(game.getPlayer(1), 0, game.CurrentMap);
	game.setStone(game.getPlayer(2), 0, game.CurrentMap);
	game.setStone(game.getPlayer(2), 0, game.CurrentMap);
	game.setStone(game.getPlayer(2), 0, game.CurrentMap);

	game.setStone(game.getPlayer(2), 1, game.CurrentMap);
	game.setStone(game.getPlayer(2), 1, game.CurrentMap);
	game.setStone(game.getPlayer(2), 1, game.CurrentMap);
	game.setStone(game.getPlayer(1), 1, game.CurrentMap);
	game.setStone(game.getPlayer(2), 1, game.CurrentMap);

	game.setStone(game.getPlayer(2), 2, game.CurrentMap);
	game.setStone(game.getPlayer(1), 2, game.CurrentMap);
	game.setStone(game.getPlayer(1), 2, game.CurrentMap);
	game.setStone(game.getPlayer(2), 2, game.CurrentMap);

	game.setStone(game.getPlayer(1), 3, game.CurrentMap);
	game.setStone(game.getPlayer(2), 3, game.CurrentMap);
	game.setStone(game.getPlayer(1), 3, game.CurrentMap);

	game.setStone(game.getPlayer(2), 4, game.CurrentMap);
	game.setStone(game.getPlayer(1), 4, game.CurrentMap);
	game.setStone(game.getPlayer(2), 4, game.CurrentMap);

	game.setStone(game.getPlayer(2), 5, game.CurrentMap);
	game.setStone(game.getPlayer(1), 5, game.CurrentMap);

	game.setStone(game.getPlayer(2), 6, game.CurrentMap);
	game.setStone(game.getPlayer(1), 6, game.CurrentMap);


	std::cout << game.CurrentMap << std::endl;

	std::vector<std::tuple<LineType, std::pair<int, int>, std::pair<int, int>>> result = Game::checkLine(2, game.CurrentMap, game.getPlayer(2));
	std::cout << "Count: " << result.size() << std::endl;

	for(auto& v : result){
		LineType type = get<0>(v);
		switch (type)
		{
		case LineType::AscendingDiagonal:
			std::cout << "AscendingDiagonal: " << get<1>(v).first << " | " << get<1>(v).second << " -> " << get<2>(v).first << " | " << get<2>(v).second << std::endl;
			break;
		case LineType::DescendingDiagonal:
			std::cout << "DescendingDiagonal: " << get<1>(v).first << " | " << get<1>(v).second << " -> " << get<2>(v).first << " | " << get<2>(v).second << std::endl;
			break;
		case LineType::Vertical:
			std::cout << "Vertical: " << get<1>(v).first << " | " << get<1>(v).second << " -> " << get<2>(v).first << " | " << get<2>(v).second << std::endl;
			break;
		case LineType::Horizontal:
			std::cout << "Horizontal: " << get<1>(v).first << " | " << get<1>(v).second << " -> " << get<2>(v).first << " | " << get<2>(v).second << std::endl;
			break;
		default:
			break;
		}
	}
/*
	    // define variables and model
    torch::Tensor inputs = torch::randn({1, 1, 6, 7});
    torch::Tensor targets = torch::randn({1, 7}).to(torch::kFloat16);
    std::vector<torch::Tensor> batch = {inputs, targets};

	Network net;
	NetworkAgent netAgent(net, 10, 100);
	netAgent.load();

    // what are the variables?
    std::cout << "Our list of parameters: ";
    for (const auto& pair : netAgent.getQNet()->named_parameters()) {
        std::cout << pair.key() << " ";
    }
    std::cout << std::endl;

    // do they change after a training step?
    // let's run a train step and see
    auto optimizer = torch::optim::Adam(netAgent.getQNet()->parameters(), torch::optim::AdamOptions(0.01));
    auto loss_fn = torch::nn::CrossEntropyLoss();
    torch::Tensor loss = loss_fn(netAgent.getQNet()->forward(inputs).second, targets);
    optimizer.zero_grad();
    loss.backward();
    optimizer.step();
    torch::Tensor new_loss = loss_fn(netAgent.getQNet()->forward(inputs).second, targets);
    std::cout << "Assert statement passes if parameters are updated: ";
    if (new_loss.item<float>() != loss.item<float>()) {
        std::cout << "true" << std::endl;
		std::cout << new_loss.item<float>() << std::endl;
		std::cout << loss.item<float>() << std::endl;
    } else {
        std::cout << "false" << std::endl;
    }

    return 0;
*/
	// set seed for all upcoming random decisions
	srand(time(NULL));

	// set default values
	Debug::setFlag(true);

	std::string ip = "127.0.0.1";
	unsigned short port = 7777;

	int batchSize = 64;
	int replayMemorySize = 10000;
	int amountTraining = 1000;

	MoveCalculator moveCalculator = MoveCalculator::Random;
	ApplicationMode mode = ApplicationMode::Gaming; 

	if(argc > 1){
		for(int i = 1; i < argc; ++i) {
			std::string arg = argv[i];
			if((arg == "-h") || (arg == "--help")){
				printHelp();
				return 0;
				break;
			}else if((arg == "-i") || (arg == "--ip")){
				if (i + 1 < argc) {
					ip = argv[i+1];
				}
			}else if((arg == "-p") || (arg == "--port")){
				if (i + 1 < argc) {
					std::string s = argv[i+1];
					port = std::stoi(s, nullptr);
				}				
			}
			else if((arg == "-d") || (arg == "--debug")){
				Debug::setFlag(true);				
			}
			else if((arg == "-q") || (arg == "--quiet")){
				Debug::setFlag(false);			
			}
			else if((arg == "-b") || (arg == "--batchSize")){
				if (i + 1 < argc) {
					std::string s = argv[i+1];
					batchSize = std::stoi(s, nullptr);
				}		
			}
			else if((arg == "-r") || (arg == "--replayMemorySize")){
				if (i + 1 < argc) {
					std::string s = argv[i+1];
					replayMemorySize = std::stoi(s, nullptr);
				}		
			}
			else if((arg == "-t") || (arg == "--trainingSize")){
				if (i + 1 < argc) {
					std::string s = argv[i+1];
					amountTraining = std::stoi(s, nullptr);
				}		
			}
			else if((arg == "-m") || (arg == "--mode")){
				if (i + 1 < argc) {
					std::string s = argv[i+1];
					if (s == "training") {
						mode = ApplicationMode::Training;
					} else if (s == "gaming") {
						mode = ApplicationMode::Gaming;
					}	
				}
			}
			else if((arg == "-pl") || (arg == "--player")){
				if (i + 1 < argc) {
					std::string s = argv[i+1];
					if (s == "neuronal") {
						moveCalculator = MoveCalculator::MachineLearning;
					} else if (s == "minmax") {
						moveCalculator = MoveCalculator::MinMax;
					} else if (s == "random") {
						moveCalculator = MoveCalculator::Random;
					}
				}
			}
		}
	}

	if (mode == ApplicationMode::Training) {
		Network qNet;
		Network targetNet;
		NetworkAgent netAgent(qNet, batchSize, replayMemorySize, targetNet);
		
		Trainer trainer(moveCalculator, netAgent);
		trainer.train(amountTraining);

	} else if (mode == ApplicationMode::Gaming) {
		try {
			GameHandler *gameHandler = new GameHandler(ip, port);
			gameHandler->run(moveCalculator);
		}catch(std::exception &err){
			std::cout << err.what() << std::endl;
		}catch(const char* c){
			std::cout << "Error: " << c << std::endl;
		}
		catch(...){
			std::cout << "Unknown error";
		}
	}
	
	return 0; 
} 