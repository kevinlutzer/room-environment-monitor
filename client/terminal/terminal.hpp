#ifndef _DEBUG_H
#define _DEBUG_H

#include "Stream.h"
#include "stdlib.h"

// First argument is the command that was run, the rest are the space dellimited arguments
typedef int (commandFunc)(int argc, char* argv[]);    

using namespace std;

class Terminal {
    
    public :
        Terminal(bool init, Stream * terminalStream);
        void addCommand(char * name, commandFunc command);

        // Getter and Setter for the debug status
        bool get();
        void set(bool init);

        // Print functions
        void println(const char[]);
        void println(String str);
        void print(const char[]);
        void print(String str);

    private:
        bool debug;
        Stream * terminalStream;    
        commandFunc * commands;
        int commandTotal = 0;
        // std::map<int, std::function<void(Pipeline*)>> handlers;
};

#endif