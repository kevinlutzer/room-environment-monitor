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

bool Terminal::indexOfCommand(String name) {
    for (int i = 0; i < this->commandTotal; i++) {
        if (this->commandOrder[i] == name) {
            return i;
        }
    }

    return -1;
}

void Terminal::waitForCommand() {
    String commandWithArgs = this->terminalStream->readStringUntil('\n');

    String command;
    String * arguments;

    int spaceIndex = commandWithArgs.indexOf(' ');
    while(spaceIndex >= 0) {
        int len = commandWithArgs.length(); 
        command = commandWithArgs.substring(0, spaceIndex);
        commandWithArgs = commandWithArgs.substring(spaceIndex + 1, len);

        spaceIndex = command.indexOf(' ');
    }

    if (command.length() == 0) {
        this->println("The command specified does not exist");
    }
}

void Terminal::addCommand(char * name, commandFunc command) {
    // int newCommandTotal = commandTotal + 1;
    // commandFunc * newCommands = (commandFunc *)malloc(sizeof(commandFunc) * (newCommandTotal));
    
    // if(this->commands != NULL) {
    //     for (int i = 0; i < commandTotal; i++) {
    //         newCommands[i] = this->commands[i];
    //     }

    //     free(this->commands);
    // }

    // newCommands[commandTotal + 1] = command;
    // this->commands = newCommands;

    // * newCommandOrder = (String *)malloc(sizeof(String) * newCommandTotal);
    // if(this->commandOrder != NULL) {
    //     for (int i = 0; i < commandTotal; i++) {
    //         newCommandOrder[i] = this->commandOrder[i];
    //     }

    //     free(this->commandOrder);
    // }

    // newCommandOrder[commandTotal] = name;
    // this->commandOrder = newCommandOrder;
    // this->commandTotal = newCommandTotal;
}