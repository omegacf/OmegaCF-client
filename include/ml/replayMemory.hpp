#ifndef REPLAY_MEMORY_HPP
#define REPLAY_MEMORY_HPP

#include<tuple>

template<typename T> 
class ReplayMemory {
    private:
        int const _capacity;
        T* _buffer;
        int _pos = 0;
        int _size = 0;
    public:
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

        ~ReplayMemory() {
            delete[] this->_buffer;
        }

};
#endif