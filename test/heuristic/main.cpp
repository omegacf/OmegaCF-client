#include "../../include/factory/GameFactory.hpp"
#include "../../include/game/BestMoveCalculator.hpp"
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>

namespace fs = std::filesystem;
void convertVector(std::vector<uint8_t>&);

int main(int argc, char *argv[]) {
    Game game;
    game = GameFactory::create(7, 6, 2);

    std::string path = "test/heuristic/grids";
    for (const auto & entry : fs::directory_iterator(path)) {
        std::cout << entry.path() << std::endl;
        // Read grid
        std::vector<std::vector<uint8_t>> gridData;
        std::ifstream in(entry.path());
        std::string str;
        while (std::getline(in, str)) {
            if(str.size() > 0) {
                // convert string to vector
                std::vector<uint8_t> line(str.begin(), str.end());
                convertVector(line);
                gridData.push_back(line);
            }
        }
        Grid grid = Grid(gridData);
        std::cout << grid;

        // Best Move Calculator
        BestMoveCalculator bmc(game, game.Players[0]);

        std::cout << "Score of this grid: " <<  bmc.evaluateBoard(grid, game.Players[0], game.Players[1]) << std::endl;
    }
    return 0;
}

void convertVector(std::vector<uint8_t> &line) {
    for (uint8_t &ui : line)
    {
        ui = ui - '0';
    }
}