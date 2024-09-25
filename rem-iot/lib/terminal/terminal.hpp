#ifndef _TERMINAL_H
#define _TERMINAL_H

#include "Stream.h"
#include "Arduino.h"
#include "providers.hpp"
#include "FreeRTOS_CLI.hpp"

#define TERMINAL_TIMEOUT_TICK pdMS_TO_TICKS(10)

// The max lenght of the time buffer we would allow
#define TIME_BUFFER_SIZE_MAX 128

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
        size_t debugf(const char * format, ...)  __attribute__ ((format (printf, 2, 3)));

        // Debug setter and getter
        bool toggleDebug();
        bool isDebug();

        // Util for getting time
        size_t getTime(char * buf);

        // Task handler for the terminal
        void handleCharacter();

    private:

        bool _debug;
        SemaphoreHandle_t txMutex;
        Stream * stream;
};

#endif