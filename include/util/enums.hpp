#ifndef ENUMS_HPP
#define ENUMS_HPP
typedef enum class NetworkMode{
    Training = 0,
    Production = 1
} NetworkMode;

typedef enum class ApplicationMode{
    Training = 0,
    Gaming = 1
} ApplicationMode;

typedef enum class MoveCalculator{
    MinMax = 0,
    Random = 1,
    MachineLearning = 2
} MoveCalculator;
#endif