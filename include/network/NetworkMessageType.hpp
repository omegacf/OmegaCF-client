#ifndef NETWORKMESSAGETYPE_H
#define NETWORKMESSAGETYPE_H
    typedef enum NetworkMessageType{
        GroupNumber = 1,
        Map = 2,
        PlayerNumber = 3,
        Request = 4,
        Answer = 5,
        Move = 6,
        Disqualification = 7,
        EndOne = 8,
        EndTwo = 9
    }
    NetworkMessageType;
#endif