#include "../../include/ml/network.hpp"

void Network::evalPosition(Grid& grid) {
    // create input from grid
    torch::Tensor input = torch::rand({1, 1, 6, 7});
    std::cout << input << std::endl;

    torch::Tensor output = this->_model->forward(input);
    std::cout << output << std::endl;
}