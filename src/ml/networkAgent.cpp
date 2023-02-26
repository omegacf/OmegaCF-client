#include "../../include/ml/networkAgent.hpp"

void NetworkAgent::evalPosition(Grid& grid) {
    // create input from grid
    torch::Tensor input = torch::rand({1, 1, 6, 7});
    std::cout << input << std::endl;

    std::pair<torch::Tensor, torch::Tensor> output = this->_model->forward(input);
    std::cout << output.first << std::endl;
    std::cout << output.second << std::endl;
}