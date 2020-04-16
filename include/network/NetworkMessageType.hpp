#ifndef NETWORKMESSAGETYPE_H
#define NETWORKMESSAGETYPE_H
    typedef enum NetworkMessageType{
        Configuration = 1,
        Request = 4,
        Answer = 5,
        Move = 6,
        Disqualification = 7,
        EndGame = 8
    }
    NetworkMessageType;
#endif