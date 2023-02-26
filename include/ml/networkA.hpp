#ifndef NETWORKA_HPP
#define NETWORKA_HPP

#include "INetwork.hpp"
#include <torch/torch.h>
#include <string>

class NetworkA : public torch::nn::Module, public INetwork{
    private:
        std::string _name;
        torch::nn::Conv2d _conv1, _conv2, _conv3;
        torch::nn::Linear _linear1, _output;
    public:
        NetworkA(int amountOutput = 7, std::string name = "NetworkA"):
            _conv1(torch::nn::Conv2dOptions(1, 64, (4, 4)).stride(1).padding(0).bias(true)),
            _conv2(torch::nn::Conv2dOptions(64, 64, (2, 2)).stride(1).padding(0).bias(true)),
            _conv3(torch::nn::Conv2dOptions(64, 64, (2, 2)).stride(1).padding(0).bias(true)),
            _linear1(128, 64),
            _output(64, amountOutput)
        {};

        torch::Tensor forward(torch::Tensor input) {
            input = torch::relu(_conv1(input));
            input = torch::relu(_conv2(input));
            input = torch::relu(_conv3(input));
            // Flatten the output
            input = input.view({input.size(0), -1});
            //std::cout << input << std::endl;
            input = torch::relu(_linear1(input));
            input = _output(input);
            return input;
        }

        std::string getName() {
            return this->_name;
        }
        ~NetworkA(){};
};
#endif



/*

struct DQN : torch::nn::Module{
    DQN(int64_t input_channels, int64_t num_actions)
            :
            conv1(torch::nn::Conv2dOptions(input_channels, 32, 8)
                          .stride(4)
                          ),
            conv2(torch::nn::Conv2dOptions(32, 64, 4)
                          .stride(2)
                          ),
            conv3(torch::nn::Conv2dOptions(64, 64, 3)
                          .stride(1)
                          ),

            linear1(torch::nn::Linear(64*22*16, 512)),
            output(torch::nn::Linear(512, num_actions)){}

    torch::Tensor forward(torch::Tensor input) {
        input = torch::relu(conv1(input));
        input = torch::relu(conv2(input));
        input = torch::relu(conv3(input));
        // Flatten the output
        input = input.view({input.size(0), -1});
        input = torch::relu(linear1(input));
        input = output(input);
        return input;
    }

    torch::Tensor act(torch::Tensor state){
        torch::Tensor q_value = forward(state);
        torch::Tensor action = std::get<1>(q_value.max(1));
        return action;
    }

    torch::nn::Conv2d conv1, conv2, conv3;
    torch::nn::Linear linear1, output;
};

*/