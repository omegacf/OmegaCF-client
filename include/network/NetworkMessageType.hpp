#ifndef NETWORKMESSAGETYPE_H
#define NETWORKMESSAGETYPE_H
    typedef enum NetworkMessageType{
        Configuration = 1,
        Request = 2,
        Answer = 3,
        Move = 4,
        Disqualification = 5,
        EndGame = 6
    }
    NetworkMessageType;
#endif