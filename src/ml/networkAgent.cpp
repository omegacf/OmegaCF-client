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

bool NetworkAgent::_loadModel(Network& model, std::string const& name) {
    std::string fullPathString = this->_pathToModel + name + this->_fileExt;
    std::filesystem::path filepath = fullPathString;
    if (std::filesystem::exists(filepath)) {
        torch::load(model, fullPathString);
        return true;
    }
    return false;
}

void NetworkAgent::updateTargetNet() {
    torch::autograd::GradMode::set_enabled(false);  // make parameters copying possible
    torch::OrderedDict<std::string, at::Tensor> new_params = this->_qNet->named_parameters(); // implement this
    torch::OrderedDict<std::string, at::Tensor> params = this->_targetNet->named_parameters(true /*recurse*/);
    torch::OrderedDict<std::string, at::Tensor> buffers = this->_targetNet->named_buffers(true /*recurse*/);
    for (auto& val : new_params) {
        std::string name = val.key();
        at::Tensor* t = params.find(name);
        if (t != nullptr) {
            t->copy_(val.value());
        } else {
            t = buffers.find(name);
            if (t != nullptr) {
                t->copy_(val.value());
            }
        }
    }
    torch::autograd::GradMode::set_enabled(true);  // make parameters copying possible
    this->_targetNet->eval();
}

void NetworkAgent::load() {
    if (!this->_loadModel(this->_qNet, this->_qNetName)) {
        // no model was found
        Debug::printLine("No Model was found");
    }
}

void NetworkAgent::save() {
    this->_saveModel(this->_qNet, this->_qNetName);
}

float NetworkAgent::optimize() {
    if (this->_memory.size() < this->_batchSize) {
        return 0;
    }

    torch::optim::Adam optimizer(this->_qNet->parameters(), 0.01);

    std::vector<MEMORY_TYPE>batch(this->_batchSize);
    this->_memory.getSample(this->_batchSize, batch);


    std::vector<torch::Tensor> states;
    std::vector<torch::Tensor> new_states;
    std::vector<torch::Tensor> actions;
    std::vector<torch::Tensor> rewards;
    std::vector<torch::Tensor> dones;

    for (MEMORY_TYPE& i : batch) {
        states.push_back(std::get<0>(i));
        actions.push_back(torch::tensor({std::get<1>(i)}));
        new_states.push_back(std::get<2>(i));
        rewards.push_back(torch::tensor({std::get<3>(i)}));
        dones.push_back(torch::tensor({(int)std::get<4>(i)}));
    }

    torch::Tensor states_tensor;
    torch::Tensor new_states_tensor;
    torch::Tensor actions_tensor;
    torch::Tensor rewards_tensor;
    torch::Tensor dones_tensor;

    states_tensor = torch::cat(states, 0);
    new_states_tensor = torch::cat(new_states, 0);
    actions_tensor = torch::cat(actions, 0);
    rewards_tensor = torch::cat(rewards, 0);
    dones_tensor = torch::cat(dones, 0);


    torch::Tensor q_values = this->_qNet->forward(states_tensor).second;
    torch::Tensor next_target_q_values = this->_targetNet->forward(new_states_tensor).second;
    torch::Tensor next_q_values = this->_qNet->forward(new_states_tensor).second;

    actions_tensor = actions_tensor.to(torch::kInt64);

    torch::Tensor q_value = q_values.gather(1, actions_tensor.unsqueeze(1)).squeeze(1);
    torch::Tensor maximum = std::get<1>(next_q_values.max(1));
    torch::Tensor next_q_value = next_target_q_values.gather(1, maximum.unsqueeze(1)).squeeze(1);
    torch::Tensor expected_q_value = rewards_tensor + this->_gamma*next_q_value*(1-dones_tensor);

    torch::Tensor loss = torch::mse_loss(q_value, expected_q_value);

    optimizer.zero_grad();
    loss.backward();
    optimizer.step();

    std::cout << "Loss: " << loss.item<float>() << std::endl;
    return loss.item<float>();

    /*
    torch::optim::Adam optimizer(this->_qNet->parameters(), 0.01);
    std::vector<MEMORY_TYPE>batch(this->_batchSize);
    this->_memory.getSample(this->_batchSize, batch);

    for (MEMORY_TYPE& mem : batch) {
        // calculate Q-Values (with policy net) from previous states. Only select the Q-Values that correspond to the actions that where previously taken
        torch::Tensor target = this->_qNet->forward(std::get<0>(mem)).second;
        // check if it was last move
        if (std::get<5>(mem)) {
            target[0][std::get<1>(mem)] = std::get<4>(mem);
            // self.q_network.fit(state, target, epochs=1, verbose=0)
            torch::Tensor t = this->_targetNet->forward(std::get<0>(mem)).second;
            torch::Tensor loss = torch::mse_loss(target, t);
            optimizer.zero_grad();
            loss.backward();
            for (auto& param : this->_qNet->parameters()) {
                param.grad().data().clamp(-1, 1);
            }
            optimizer.step();
        } else {
            torch::Tensor t = this->_targetNet->forward(std::get<3>(mem)).second;
            float tMax = getMaxFromArray<float>(t.data_ptr<float>(), t.numel());
            t[0][std::get<1>(mem)] = std::get<4>(mem) + this->_gamma * tMax;

            torch::Tensor loss = torch::smooth_l1_loss(target, t);
            
            optimizer.zero_grad();
            loss.backward();
            for (auto& param : this->_qNet->parameters()) {
                param.grad().data().clamp(-1, 1);
            }
            optimizer.step();
        }
    }
    

    // calculate Q-Values (with policy net) from previous states. Only select the Q-Values that correspond to the actions that where previously taken

    // Calculate Q-Values (with target net) from nextState and select max (e.g. q-value of action)

*/
}

void NetworkAgent::addMemory(int playerNumber, Grid& state, int action, Grid& newState, int reward, bool isLastMove) {
    MEMORY_TYPE mem = std::make_tuple(this->_gridToInput(state, playerNumber), action, this->_gridToInput(newState, playerNumber), reward, isLastMove);
    this->_memory.push_back(mem);
}


void NetworkAgent::test() {
    for (int i = 0; i < 10000; ++i) {
        torch::Device device = torch::kCPU;
        



        // Create some input data and targets for a training iteration
        torch::Tensor input = torch::rand({1, 1, 6, 7}, torch::kFloat);
        torch::Tensor target = torch::ones({1, 7}, torch::kFloat);


        // Define a loss function and an optimizer
        torch::nn::CrossEntropyLoss criterion;
        torch::optim::SGD optimizer(this->_qNet->parameters(), /*lr=*/0.1);
        // Set the model to training mode
        this->_qNet->train();
        this->_qNet->to(device);

        // Perform a training iteration to update the model's weights
        optimizer.zero_grad();
        torch::Tensor output = this->_qNet->forward(input).second;

        std::cout << output << std::endl;

        torch::Tensor loss = criterion(output, target);
        loss.backward();
        for (auto& param : this->_qNet->parameters()) {
            param.grad().data().clamp(-1, 1);
        }
        optimizer.step();
        std::cout << "Loss: " << loss.item<float>() << std::endl;
        
    }
    this->_qNet->eval();
    for (auto& param : this->_qNet->named_parameters()) {
        std::cout << param->name() << std::endl;
    }

    // Save the model's weights to a file
    std::ofstream output_file("qNet.pt", std::ios::binary);
    torch::save(this->_qNet, output_file);

}