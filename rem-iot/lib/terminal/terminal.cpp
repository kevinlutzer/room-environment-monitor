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

    
    // Create a buffer to store the input string and the last input string
    static char cInputString[ cmdMAX_INPUT_SIZE ];
    static char lInputString[ cmdMAX_INPUT_SIZE ];
    
    // Grab and store grab the ptr reference to where the output buf lives
    char * pcOutputString = FreeRTOS_CLIGetOutputBuffer();
    uint8_t rxCmdIndex = 0;
    char rxChar = '\0';
    int rx = 0;

    while(true) {
        // No data, so just yield and wait
        if (!this->stream->available()) {
            yield();
            continue;
        }

        rxChar = (char)this->stream->read();

        // Grab the mutex so we can echo and return command results
        if( xSemaphoreTake( this->txMutex, TERMINAL_TIMEOUT_TICK ) == pdTRUE ) {

            // Are we one character away from reaching buffer length or was the last char
            // a '\r'?. If so execute commands
            if (rxCmdIndex == cmdMAX_INPUT_SIZE - 2 || rxChar == '\r') {

                    this->stream->println();

                    /* Pass the received command to the command interpreter.  The
                    command interpreter is called repeatedly until it returns
                    pdFALSE	(indicating there is no more output) as it might
                    generate more than one string. */
                    BaseType_t xReturned = pdTRUE;
                    for(;;)
                    {
                        // Get the command output from the processing function, this goes to std out
                        xReturned = FreeRTOS_CLIProcessCommand( cInputString, pcOutputString, (size_t)200 );
                        
                        this->stream->print(pcOutputString);

                        // Short circuit when we finally don't have any more commands to execute
                        if (xReturned == pdFALSE) {
                            break;
                        }

                    }

                    // Clean up the input string buffer and release the mutex
                    // Also store the last input string for command replay when pressing enter
                    memcpy(lInputString, cInputString, cmdMAX_INPUT_SIZE);
                    memset(cInputString, 0x00, cmdMAX_INPUT_SIZE);
                    memset((void *)pcOutputString, 0x00, configCOMMAND_INT_MAX_OUTPUT_SIZE);
                    rxCmdIndex = 0;

            // Check if the character is a printable ascii character between space and ~
            } else if (rxChar >= 32 && rxChar <= 126) {
    
                // Echo the character back and copy it to the cmd buffer 
                this->stream->print(rxChar);
                cInputString[rxCmdIndex] = rxChar;
                rxCmdIndex++;
            }

            xSemaphoreGive( this->txMutex);
        }

        delay(10);
    }
}