#ifndef _DEBUG_H
#define _DEBUG_H

#include "Stream.h"

// First argument is the command that was run, the rest are the space dellimited arguments
typedef int (*commandFunc)(int, char*);

class Terminal {
    
    public:
        Terminal(bool init, Stream * terminalStream);
        void addCommand(char * name, commandFunc command);
        void waitForCommand();

        // Getter and Setter for the debug status
        bool get();
        void set(bool init);

        // Print functions
        void println(const char[]);
        void println(String str);
        void print(const char[]);
        void print(String str);

    private:

        bool indexOfCommand(String name);

        bool debug;
        Stream * terminalStream;    
        commandFunc * commands;

        // The order in which the commands were added by there name
        // It appears that std::map doesn't exist for this toolchain and even
        // if it did, it wouldn't be the best to use it as it requires dynamic memory
        String * commandOrder; 

        int commandTotal = 0;
};

#endif