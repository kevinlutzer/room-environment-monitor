#include "terminal.hpp"

#include "Arduino.h"
#include "FreeRTOS_CLI.hpp"
#include "Stream.h"

Terminal::Terminal(bool init, Stream * stream) {
    this->_debug = init;
    this->stream = stream;
    this->txMutex = xSemaphoreCreateMutex();
}

bool Terminal::isDebug() {
    return this->_debug;
}

void Terminal::debug(const char * str) {
    if(!this->_debug) {
        return;
    }

    if( xSemaphoreTake( this->txMutex, TERMINAL_TIMEOUT_TICK ) == pdTRUE ) {
        this->stream->print(str);
        xSemaphoreGive( this->txMutex );
    }
    
}

void Terminal::debugln(const char * str) {
    if(!this->_debug) {
        return;
    }

    if( xSemaphoreTake( this->txMutex, TERMINAL_TIMEOUT_TICK ) == pdTRUE ) {
        this->stream->println(str);
        xSemaphoreGive( this->txMutex );
    }
}

void Terminal::debugln(String str) {
    if(!this->_debug) {
        return;
    }
    
    if( xSemaphoreTake( this->txMutex, TERMINAL_TIMEOUT_TICK ) == pdTRUE ) {
        this->stream->println(str);
        xSemaphoreGive( this->txMutex );
    }
}

void Terminal::debug(String str) {
    if(!this->_debug) {
        return;
    }
    
    if( xSemaphoreTake( this->txMutex, TERMINAL_TIMEOUT_TICK ) == pdTRUE ) {
        this->stream->print(str);
        xSemaphoreGive( this->txMutex );
    }
}

void Terminal::handleCharacter() {

    // grab the ptr reference to where the output buf lives
    this->pcOutputString = FreeRTOS_CLIGetOutputBuffer();
    static char cInputString[ cmdMAX_INPUT_SIZE ];

    while(true) {
        // No data, so just short circuit
        if (!this->stream->available()) {
            return;
        }

        int rx = this->stream->read();
        char rxChar = (char)(rx & 0xFF);
        uint8_t rxCmdIndex = 0;

        // Grab the mutex so we can echo and return command results
        if( xSemaphoreTake( this->txMutex, TERMINAL_TIMEOUT_TICK ) == pdTRUE ) {

            // Echo the character back and copy it to the cmd buffer 
            this->stream->print(rxChar);
            cInputString[rxCmdIndex] = rxChar;
            rxCmdIndex++;

            // Are we one character away from reaching buffer length or was the last char
            // a '\r' or '\n'?. If so execute commands
            if (rxCmdIndex == cmdMAX_INPUT_SIZE - 2 || rxChar == '\r' || rxChar == '\n') {
                    /* Pass the received command to the command interpreter.  The
                    command interpreter is called repeatedly until it returns
                    pdFALSE	(indicating there is no more output) as it might
                    generate more than one string. */
                    BaseType_t xReturned = pdTRUE;
                    for(;;)
                    {
                        // Get the command output from the processing function, this goes to std out
                        FreeRTOS_CLIProcessCommand( cInputString, this->pcOutputString, (size_t)200 );
                        
                        this->stream->print(this->pcOutputString);

                        // Short circuit when we finally don't have any more commands to execute
                        // if (xReturned == pdFALSE) {
                        //     break;
                        // }

                    }
            }

            // cleanup input string
            memset(cInputString, 0x00, cmdMAX_INPUT_SIZE);
            memset(this->pcOutputString, 0x00, configCOMMAND_INT_MAX_OUTPUT_SIZE);
            xSemaphoreGive( this->txMutex);
        }

        delay(10);
    }
}