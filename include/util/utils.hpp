#ifndef UTILS_HPP
#define UTILS_HPP
template<typename T>
T getMaxFromArray(T* arr, int size) {
    T max = arr[0];
    for (int i = 1; i<size; ++i) {
        if (max < arr[i]) {
            max = arr[i];
        }
    }
    return max;
}
#endif