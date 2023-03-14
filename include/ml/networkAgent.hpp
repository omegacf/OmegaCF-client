#ifndef NETWORKAGENT_HPP
#define NETWORKAGENT_HPP

// state, value, nextState, reward, isLastMove
#define MEMORY_TYPE std::tuple<torch::Tensor, int, torch::Tensor, int, bool>

#include <vector>
#include <string>
#include <iostream>
#include <utility>
#include <tuple>
#include <filesystem>
#include <array>
#include <algorithm>

#include "../game/Grid.hpp"
#include "replayMemory.hpp"
#include "network.hpp"
#include "../util/utils.hpp"
#include "../util/enums.hpp"

struct NetworkOutput{
    std::array<float, 3> val; // loss, draw, win
    std::array<float, 7> action;
};

class NetworkAgent {
    private:
        Network _qNet, _targetNet;
        std::string const _pathToModel = "models/";
        std::string const _fileExt = ".pt";

        std::string const _qNetName = "qNet";

        float const _gamma = 0.6;

        int _batchSize = 10;
        int _memorySize = 100000;

        ReplayMemory<MEMORY_TYPE> _memory;
    
        void _saveModel(Network& model, std::string const& name);
        void _loadModel(Network& model, std::string const& name);
        torch::Tensor _gridToInput(Grid& grid, uint8_t playerNumber);

    public:
        NetworkAgent(Network model, int batchSize = 10, int memorySize = 100000): 
            _qNet(model), 
            _targetNet(model),
            _batchSize(batchSize),
            _memorySize(memorySize),
            _memory(ReplayMemory<MEMORY_TYPE>(memorySize)) {
                this->_targetNet->eval();
        };
        NetworkOutput evalPosition(Grid& grid, uint8_t playerNumber);
        void optimize(); 
        void load();
        void save();
        void addMemory(int playerNumber, Grid& state, int action, Grid& newState, int reward, bool isLastMove);
        void inline setMode(NetworkMode mode) {
            if(mode == NetworkMode::Training) {
                this->_qNet->train();
            } else {
                this->_qNet->eval();
            }
        }

        Network inline getQNet() {
            return this->_qNet;
        }

        void inline setBatchSize(int batchSize) {
            this->_batchSize = batchSize;
        }

        void inline setMemorySize(int memSize) {
            this->_memorySize = memSize;
            // reset replay memory
            this->_memory = ReplayMemory<MEMORY_TYPE>(this->_memorySize);
        }


        void test();
};

#endif