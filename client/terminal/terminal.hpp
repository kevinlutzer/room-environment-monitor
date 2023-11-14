#ifndef _TERMINAL_H
#define _TERMINAL_H

#include "Stream.h"

// First argument is the command that was run, the rest are the space dellimited arguments
typedef int (commandFunc)(int argc, char* argv[]);     

class Terminal {
    Terminal(Stream * stream);
    void addCommand(char * name, commandFunc command);

    private: 
        Stream * stream;
        bool * debugPtr;
        commandFunc * commands;
        int commandTotal = 0;
};

#endif