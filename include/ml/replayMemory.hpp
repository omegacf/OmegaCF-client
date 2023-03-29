#ifndef REPLAY_MEMORY_HPP
#define REPLAY_MEMORY_HPP

#include <vector>
#include <random>
#include <algorithm>

template<typename T> 
class ReplayMemory {
    private:
        int const _capacity;
        T* _buffer;
        int _pos = 0;
        int _size = 0;
    public:
        ReplayMemory():_capacity(0) {};
        ReplayMemory(int capacity):_capacity(capacity) {
            this->_buffer = new T[capacity];
        }
        void push_back(T& element) {
            this->_buffer[this->_pos] = element;
            this->_pos = (this->_pos + 1) % this->_capacity;
            this->_size = std::min(this->_size + 1, this->_capacity);
        }
        int size() {
            return this->_size;
        }

        T& operator[](int index) {
            int pos = index % this->_capacity;
            return this->_buffer[pos];
        }

        bool getSample(int sampleSize, std::vector<T>& container) {
            if (sampleSize > this->_size && container.size() != sampleSize)
                return false;
            
            std::random_device rd;
            std::mt19937 gen(rd());
            
            std::vector<int> indices(this->_size);
            std::iota(indices.begin(), indices.end(), 0);
            std::shuffle(indices.begin(), indices.end(), gen);

            for (int i = 0; i < sampleSize; ++i) {
                int pos = indices[i] % this->_capacity;
                container.at(i) = this->_buffer[pos];
            }
            return true;
        }

        ReplayMemory& operator=(const ReplayMemory& other) {
            if (this != &other) {
                // Freigabe des alten Speichers
                delete[] this->_buffer;
                // Initialisierung von Werten
                *const_cast<int*> (&this->_capacity)= other._capacity;
                this->_buffer = new T[_capacity];
                this->_pos = other._pos;
                this->_size = other._size;
                // Kopieren von Werten
                std::copy(other._buffer, other._buffer + _capacity, this->_buffer);
            }
            return *this;
        }

        ~ReplayMemory() {
            delete[] this->_buffer;
        }

};
#endif