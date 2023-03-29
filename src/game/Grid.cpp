#include "../../include/game/Grid.hpp"

std::string Grid::getHash() {
    int rows = this->MapArray.size();
    int columns = this->MapArray[0].size();

    std::string resultString = "";

    // reshape
    std::vector<uint8_t> tempVec(rows * columns, 0);
    bool leadingZeros = false;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            // tempVec[i*columns + j] = this->MapArray.at(i).at(j);
            uint8_t value = this->MapArray.at(i).at(j);
            if (leadingZeros || value != 0) {
                leadingZeros = true;
                resultString.push_back(std::to_string(value)[0]);
            }
        }
    }
    return this->ternary_to_hexadecimal(resultString);

}

std::string Grid::ternary_to_hexadecimal(const std::string& ternary) {
    std::string hexadecimal;

    // Convert the ternary number to decimal
    __uint128_t decimal = 0;
    for (auto it = ternary.begin(); it != ternary.end(); ++it) {
        decimal = decimal * 3 + (*it - '0');
    }

    // Convert the decimal number to hexadecimal
    while (decimal != 0) {
        int remainder = decimal % 16;
        char digit = (remainder < 10) ? ('0' + remainder) : ('A' + remainder - 10);
        hexadecimal.push_back(digit);
        decimal /= 16;
    }

    // Reverse the resulting hexadecimal string
    std::reverse(hexadecimal.begin(), hexadecimal.end());

    // Handle the case where the ternary number is zero
    if (hexadecimal.empty()) {
        hexadecimal = "0";
    }

    return hexadecimal;
}