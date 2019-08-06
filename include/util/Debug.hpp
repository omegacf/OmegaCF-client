#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <string>

class Debug{
    private:
        static bool _debug;
    public:
        static void setFlag(bool flag);
        static bool getFlag();
        static void print(std::string s);
        static void printLine(std::string s);
        static void printVersion(); 
        static void printErrorLine(std::string s); 
};
#endif