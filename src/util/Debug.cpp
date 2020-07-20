#include "../../include/util/Debug.hpp"

bool Debug::_debug = false;

void Debug::print(std::string s){
    if(_debug){
        std::cout << s << std::flush;   
    }
}

void Debug::printLine(std::string s){
    if(_debug){
        std::cout << s << std::endl << std::flush;   
    }
}

void Debug::printErrorLine(std::string s) {
	if (_debug) {
		std::cerr << s << std::endl; 
	}
}

void Debug::setFlag(bool flag){
    _debug = flag;
}

bool Debug::getFlag(){
    return _debug;
}