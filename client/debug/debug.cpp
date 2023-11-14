#include "debug.hpp"
#include "Stream.h"

Debug::Debug(bool init, Stream * debugStream) {
    this->debug = init;
    this->debugStream = debugStream;
}

bool Debug::get() {
    return this->debug;
}

void Debug::set(bool init) {
    this->debug = init;
}

void Debug::print(const char * str) {
    if(!this->debug) {
        return;
    }
    
    this->debugStream->print(str);
}

void Debug::println(const char * str) {
    if(!this->debug) {
        return;
    }
    
    this->debugStream->println(str);
}

void Debug::println(String str) {
    if(!this->debug) {
        return;
    }
    
    this->debugStream->println(str);
}

void Debug::print(String str) {
    if(!this->debug) {
        return;
    }
    
    this->debugStream->print(str);
}