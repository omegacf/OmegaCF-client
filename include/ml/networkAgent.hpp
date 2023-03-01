#ifndef NETWORKAGENT_HPP
#define NETWORKAGENT_HPP

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
        std::string const _targetNetName = "targetNet";

        int const _batchSize = 100;
        int const _memorySize = 1000;

        ReplayMemory<std::tuple<int, int>> const _memory;
    
        void _saveModel(Network& model, std::string const& name);
        torch::Tensor _gridToInput(Grid& grid, uint8_t playerNumber);

    public:
        NetworkAgent(Network model): _qNet(model), _targetNet(model), _memory(ReplayMemory<std::tuple<int, int>>(this->_memorySize)) {
            torch::manual_seed(0);
            this->_qNet->eval();
        };
        NetworkOutput evalPosition(Grid& grid, uint8_t playerNumber);
        void optimize(); 
        void load();
        void save();
};
#endif

/*
import numpy as np
from keras.models import Sequential, load_model
from abc import ABC, abstractmethod
import os

save_dir = '../saved_models'


class Network(ABC):
    def __init__(self, model):
        self.model = model
        self.load()

    def eval_position(self, board_state):
        input = self._board_states_to_inputs([board_state])
        result = self.model.predict(input)
        return result

    def update(self, board_states, rewards):
        inputs = self._board_states_to_inputs(board_states)
        outputs = np.array(rewards)
        self.model.train_on_batch(inputs, outputs)

    def load(self):
        model_path = os.path.join(save_dir, self.get_save_file())
        if os.path.isfile(model_path):
            self.model = load_model(model_path)
            print('Loaded model from', model_path)
    
    def save(self):
        if not os.path.isdir(save_dir):
            os.makedirs(save_dir)
        model_path = os.path.join(save_dir, self.get_save_file())
        self.model.save(model_path)

    def _board_states_to_inputs(self, board_states):
        inputs = np.array(board_states)
        inputs = np.expand_dims(inputs, axis=3)

        return inputs

    @abstractmethod
    def get_save_file(self):
        pass

    @abstractmethod
    def get_name(self):
        pass
*/