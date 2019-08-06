#include "../../include/util/Helper.hpp"

/** reads all integer values in a string 
 * @param const char * data the string we are searching in 
 * @param int * resultIntArray (W) the output array 
 * @param int maximum the limit (size of resultIntArray) 
 * @return int count of integers read */
int Helper::readInts(const char * data, int * resultIntArray, int maximum) {
	register char cc; 
	bool readSomething = false; 
	int zahl = 0; 
	int anzahl = 0;
	if (maximum <= 0) {
		return 0;
	}

	while (true) {
		cc = *data; 
		if (cc >= '0' && cc <= '9') {
			zahl *= 10;
			zahl += cc - '0';
			readSomething = true;
		} else if (cc == '.') {
			//Tausendertrennzeichen
		} else if (readSomething) {
			*resultIntArray = zahl; 
			resultIntArray++;
			--maximum; 
			++anzahl; 
			if (maximum <= 0) {
				return anzahl; 
			} else {
				zahl = 0;
				readSomething = false;
			}
		}

		if (cc == '\0') {
			return anzahl;
		}
		++data;
	}
}

/** copys all valid chars and ignores all other chars 
 * @param const char * data the string we are searching in (0-terminated)
 * @param char * (W) the output string (0-terminated)
 * @param int maximum the limit of chars (resultCharArrays size incl 0-term.) 
 * @param const char * validChars the accepted chars we are searching for 
 * @return int the count of copied chars (without 0-term.) */
int Helper::readValidChars(const char * data, char * resultCharArray, int maximum, const char * validChars) {
	int anzahl = 0; 
	register char cc; 
	if (maximum <= 0) {
		*resultCharArray = '\0'; 
		return 0; 
	}
	while (true) {
		cc = *data; 
		if (cc == '\0') {
			*resultCharArray = '\0'; 
			return anzahl; 
		} else if (Helper::isCharInCharArray(cc, validChars)) {
			*resultCharArray = cc; 
			resultCharArray++; 
			++anzahl; 
			--maximum;
			if (maximum <= 0) {
				*resultCharArray = '\0'; 
				return anzahl; 
			}
		}
		++data; 
	}
}

/** check, if a char is part of a string 
 * @param const char needle the char we are searching for 
 * @param const char * haystack the string we are searching in 
 * @return bool the search result */
bool Helper::isCharInCharArray(const char needle, const char * haystack) {
	register char cc; 
	while (1) {
		cc = *haystack; 
		if (cc == needle) {
			return true;
		} else if (cc == '\0') {
			return false; 
		}
		haystack++;
	}
}

char Helper::byteToAscii(int8_t byte){
    std::string s;
    s = std::to_string(byte);
    return s.at(0);
}