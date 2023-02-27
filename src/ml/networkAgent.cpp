#include "../../include/ml/networkAgent.hpp"

void NetworkAgent::evalPosition(Grid& grid) {
    // create input from grid
    torch::Tensor input = torch::rand({1, 1, 6, 7});
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