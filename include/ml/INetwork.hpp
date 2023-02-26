#ifndef INETWORK_HPP
#define INETWORK_HPP

#include <torch/torch.h>
#include <string>
#include <utility>

class INetwork {
    public:
        virtual std::pair<torch::Tensor, torch::Tensor> forward(torch::Tensor input) = 0;
        virtual std::string getName() = 0;
        virtual ~INetwork() {};
};
#endif