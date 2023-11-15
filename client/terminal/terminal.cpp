#include "terminal.hpp"
#include "Stream.h"

Terminal::Terminal(bool init, Stream * terminalStream) {
    this->debug = init;
    this->terminalStream = terminalStream;
}

bool Terminal::get() {
    return this->debug;
}

void Terminal::set(bool init) {
    this->debug = init;
}

void Terminal::print(const char * str) {
    if(!this->debug) {
        return;
    }
    
    this->terminalStream->print(str);
}

void Terminal::println(const char * str) {
    if(!this->debug) {
        return;
    }
    
    this->terminalStream->println(str);
}

void Terminal::println(String str) {
    if(!this->debug) {
        return;
    }
    
    this->terminalStream->println(str);
}

void Terminal::print(String str) {
    if(!this->debug) {
        return;
    }
    
    this->terminalStream->print(str);
}

void Terminal::addCommand(char * name, commandFunc command) {
    // commandFunc * newCommands = (commandFunc *)malloc(sizeof(commandFunc) * (commandTotal ++));
    // for (int i = 0; i < commandTotal; i++) {
    //     newCommands[i] = this->commands[i];
    // }

    // commandFunc newCommands[1] = {command };

    // newCommands[commandTotal] = command;
    // this->commands = commands;

}