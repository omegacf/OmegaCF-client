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
	// set seed for all upcoming random decisions
	srand(time(NULL));

	// set default values
	Debug::setFlag(true);

	std::string ip = "127.0.0.1";
	unsigned short port = 7777;

	int batchSize = 100;
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
		Network net;
		NetworkAgent netAgent(net, batchSize, replayMemorySize);
		
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