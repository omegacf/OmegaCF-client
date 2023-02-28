#include "../../include/ml/networkAgent.hpp"

torch::Tensor NetworkAgent::_gridToInput(Grid& grid, uint8_t playerNumber) {
    std::cout << grid << std::endl;
    std::vector<int> vec(grid.SizeX*grid.SizeY);
    for(std::vector<uint8_t>& v : grid.MapArray) {
        for(auto&& i : v) {
            int tmp = 0;
            if (i != 0) {
                if (i == playerNumber)
                    tmp = 1;
                else
                    tmp = -1;
            }
            vec.push_back(tmp);
        }
    }

    return torch::from_blob(vec.data(), {1, 1, grid.SizeY, grid.SizeX}, torch::TensorOptions().dtype(torch::kFloat16));
}

NetworkOutput NetworkAgent::evalPosition(Grid& grid, uint8_t playerNumber) {
    std::cout << "Eval position" << std::endl;
    // create input from grid
    torch::Tensor input = this->_gridToInput(grid, playerNumber);
    std::cout << input << std::endl;

    std::pair<torch::Tensor, torch::Tensor> output = this->_qNet->forward(input);
    std::cout << output.first << std::endl;
    std::cout << output.second << std::endl;
}

void NetworkAgent::_saveModel(Network& model, std::string const& name) {
    std::string fullPathString = this->_pathToModel + name + this->_fileExt;
    std::filesystem::path filepath = this->_pathToModel;
    if (!std::filesystem::is_directory(filepath)) {
        std::filesystem::create_directory(filepath);
    }
    torch::save(model, fullPathString);
}

void NetworkAgent::save() {
    this->_saveModel(this->_qNet, this->_qNetName);
    this->_saveModel(this->_targetNet, this->_targetNetName);
}