#include "../../include/ml/networkAgent.hpp"

void NetworkAgent::evalPosition(Grid& grid) {
    // create input from grid
    torch::Tensor input = torch::rand({1, 1, 6, 7});
    std::cout << input << std::endl;

    std::pair<torch::Tensor, torch::Tensor> output = this->_model->forward(input);
    std::cout << output.first << std::endl;
    std::cout << output.second << std::endl;
}

void NetworkAgent::save() {
    std::string fullPathString = this->_pathToModels + this->_model->getName();
    std::filesystem::path filepath = fullPathString;
    if (std::filesystem::is_directory(filepath.parent_path())) {
        std::filesystem::create_directory(filepath.parent_path());
    }
    this->_model->saveModel(fullPathString);
}