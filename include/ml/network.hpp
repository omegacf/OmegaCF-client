#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <tensorflow/c/c_api.h>
#include <vector>

#include "../game/Grid.hpp"

using namespace tensorflow;
using namespace tensorflow::ops;

class Network {
    private:
        Scope _model;
        std::vector<int8_t> _GridToInput(Grid& grid);



    private:
        auto evalPosition(Grid& grid);

};
#endif