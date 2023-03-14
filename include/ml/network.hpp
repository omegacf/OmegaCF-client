#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <torch/torch.h>
#include <string>
#include <utility>

class NetworkImpl : public torch::nn::Module{
    private:
        torch::nn::Conv2d _conv1{nullptr}, _conv2{nullptr}, _conv3{nullptr};
        torch::nn::Linear _linear1{nullptr}, _outputPolicy{nullptr};
    public:
        NetworkImpl()
        {
            this->_conv1 = register_module("conv1", torch::nn::Conv2d(torch::nn::Conv2dOptions(1, 64, (4, 4)).stride(1).padding(0).bias(true)));
            this->_conv2 = register_module("conv2", torch::nn::Conv2d(torch::nn::Conv2dOptions(64, 64, (2, 2)).stride(1).padding(0).bias(true)));
            this->_conv3 = register_module("conv3", torch::nn::Conv2d(torch::nn::Conv2dOptions(64, 64, (2, 2)).stride(1).padding(0).bias(true)));
            this->_linear1 = register_module("linear1", torch::nn::Linear(128, 64));
            this->_outputPolicy = register_module("outputPolicy", torch::nn::Linear(64, 7));
        };

        std::pair<torch::Tensor, torch::Tensor> forward(torch::Tensor input) {
            input = torch::relu(_conv1(input));
            input = torch::relu(_conv2(input));
            input = torch::relu(_conv3(input));
            // Flatten the output
            input = input.view({input.size(0), -1});
            //std::cout << input << std::endl;
            input = torch::relu(_linear1(input));
            torch::Tensor value = torch::ones({1, 3});
            //torch::Tensor value = _outputValue(input);
            torch::Tensor policy = torch::nn::functional::softmax(_outputPolicy(input), torch::nn::functional::SoftmaxFuncOptions(1));
            return std::make_pair(value, policy);
        }

        void saveModel(std::string& path) {
            const torch::nn::Module* tmpModule = this;
            //torch::save(*this, path);
            //torch::Tensor t = torch::rand({1, 1, 6, 7});
            //torch::save(t, path);
            torch::serialize::OutputArchive output_archive;
            this->save(output_archive);
            output_archive.save_to(path);
        }

        std::string saveMemory(){
            std::ostringstream oss;
            torch::serialize::OutputArchive archive;

            this->save(archive);
            archive.save_to(oss);

            return oss.str();
        }

        void loadMemory(const std::string& mem){
            torch::serialize::InputArchive archive;
            std::istringstream iss(mem);
            archive.load_from(iss);
            this->load(archive);
        }

        ~NetworkImpl(){};
};
TORCH_MODULE(Network);
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