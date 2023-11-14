#include "Stream.h"
#include "terminal.hpp"

Terminal::Terminal(Stream * stream) {
    // this->debugPtr = debug;
    this->stream = stream;
}

void Terminal::addCommand(char * name, commandFunc command) {
    
    commandFunc * newCommands = (commandFunc *)malloc(sizeof(commandFunc) * (commandTotal + 1));
    for (int i = 0; i < commandTotal; i++) {
        newCommands[i] = this->commands[i];
    }

    newCommands[commandTotal] = command;
    this->commands = commands;

}