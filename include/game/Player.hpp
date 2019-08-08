#ifndef PLAYER_HPP
#define PLAYER_HPP
class Player{
    private:

    public:
        uint8_t Id;

        Player(int id) : Id(id){};
        Player() : Id(1){};
};
#endif