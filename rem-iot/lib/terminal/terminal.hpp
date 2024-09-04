#ifndef _TERMINAL_H
#define _TERMINAL_H

#include "Stream.h"
#include "Arduino.h"
#include "FreeRTOS_CLI.hpp"

#define TERMINAL_TIMEOUT_TICK pdMS_TO_TICKS(10)

// First argument is the command that was run, the rest are the space dellimited arguments
typedef int (*commandFunc)(int, char*);

class Terminal {
    
    public:
        Terminal(bool init, Stream * terminalStream);
        
        // Print functions
        void debugln(const char[]);
        void debugln(String str);
        void debug(const char[]);
        void debug(String str);

        bool toggleDebug();

        // Debug getter
        bool isDebug();

        // Task handler for the terminal
        void terminalProc(void * userData);

    private:

        bool _debug;
        SemaphoreHandle_t txMutex;
        Stream * stream;
};

#endif