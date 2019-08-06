#ifndef __HELPER_H_OFREVERSI__
#define __HELPER_H_OFREVERSI__

#include<string>
#include<vector>
#include <sstream>

struct Helper {
  static int readInts(const char * data, int * resultIntArray, int maximum); 
	static int readValidChars(const char * data, char * resultCharArray, int maximum, const char * validChars); 
	static bool isCharInCharArray(const char needle, const char * haystack);
	
	static char byteToAscii(int8_t byte);
};
#endif //__HELPER_H_OFREVERSI__