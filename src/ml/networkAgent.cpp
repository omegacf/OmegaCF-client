#include "../../include/ml/networkAgent.hpp"

torch::Tensor NetworkAgent::_gridToInput(Grid& grid, uint8_t playerNumber) {
    float tmpArray[grid.SizeY][grid.SizeX];

    for(int i = 0; i < grid.SizeY; ++i) {
        for(int j = 0; j < grid.SizeX; ++j) {
            float tmp = 0.0;
            uint8_t stone = grid.getStone(j, i);
            if (stone != 0) {
                if (stone == playerNumber)
                    tmp = 1.0;
                else
                    tmp = -1.0;
            }
            tmpArray[i][j] = tmp;
        }
    }
    torch::Tensor t = torch::from_blob(tmpArray, {1, 1, grid.SizeY, grid.SizeX}, torch::kFloat32);
    // https://stackoverflow.com/questions/64284893/libtorch-why-does-my-tensor-change-value-when-returned-from-a-method-into-anoth
    return t.clone();
}

NetworkOutput NetworkAgent::evalPosition(Grid& grid, uint8_t playerNumber) {
    // create input from grid
    torch::Tensor input = this->_gridToInput(grid, playerNumber);

    std::pair<torch::Tensor, torch::Tensor> output = this->_qNet->forward(input);

    std::array<float, 3> valArray;
    int num_elements = output.first.numel();
    assert(num_elements == 3);

    std::array<float, 7> actionArray;
    num_elements = output.second.numel();
    assert(num_elements == 7);

    std::copy(output.first.data_ptr<float>(), output.first.data_ptr<float>()+3, valArray.begin());
    std::copy(output.second.data_ptr<float>(), output.second.data_ptr<float>()+7, actionArray.begin());

    NetworkOutput nwOutput;
    nwOutput.val = valArray;
    nwOutput.action = actionArray;

    return nwOutput;
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