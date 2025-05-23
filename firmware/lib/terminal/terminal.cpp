#include "terminal.hpp"

#include "Arduino.h"
#include "FreeRTOS_CLI.hpp"
#include "Stream.h"

Terminal::Terminal(bool init, Stream *stream) {
  this->_debug = init;
  this->stream = stream;
  this->txMutex = xSemaphoreCreateMutex();
}

bool Terminal::isDebug() { return this->_debug; }

bool Terminal::toggleDebug() {
  this->_debug = !this->_debug;
  return this->_debug;
}

size_t Terminal::getTime(char *buf) {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    strcpy(buf, "[N/A] ");
    return 6;
  }

  return strftime(buf, 128, "[%m/%d/%Y %H:%M:%S] ", &timeinfo);
}

size_t Terminal::debugf(const char *format, ...) {
  int len = 0;

  if (!this->_debug) {
    return len;
  }

  // Lock on writting to the serial interface so we don't have contention with
  // multiple commands/tasks logging at the same time if( xSemaphoreTake(
  // this->txMutex, TERMINAL_TIMEOUT_TICK ) == pdTRUE ) {
  //     return len;
  // }

  char loc_buf[64];
  char *temp = loc_buf;

  va_list arg;
  va_list copy;

  char time_buf[TIME_BUFFER_SIZE_MAX];
  size_t time_len;

  char *write_buf;

  // Effectively copy the implementation of printf from Print.cpp so we can wrap
  // it with our own specific debugging information

  va_start(arg, format);
  va_copy(copy, arg);

  len = vsnprintf(temp, sizeof(loc_buf), format, copy);

  va_end(copy);
  if (len < 0) {
    va_end(arg);
    len = 0;
    goto memcleanup;
  }

  if (len >=
      (int)sizeof(loc_buf)) { // comparation of same sign type for the compiler
    temp = (char *)malloc(len + 1);
    if (temp == NULL) {
      va_end(arg);
      len = 0;
      goto memcleanup;
    }
    len = vsnprintf(temp, len + 1, format, arg);
  }

  va_end(arg);

  time_len = this->getTime(time_buf);
  write_buf = (char *)malloc(len + time_len);
  if (write_buf == NULL) {
    len = 0;
    goto memcleanup;
  }

  memcpy(write_buf, time_buf, time_len);
  memcpy(write_buf + time_len, temp, len);

  len = this->stream->write((uint8_t *)write_buf, len + time_len);

memcleanup:

  // if the buf addr is no longer the same as it was,
  // assume that the memory was therefore allocated from the heap and we need to
  // free it
  if (temp != loc_buf) {
    free(temp);
  }

  // xSemaphoreGive( this->txMutex );
  return len;
}

void Terminal::debug(const char *str) { this->debugf("%s", str); }

void Terminal::debugln(const char *str) { this->debugf("%s\r\n", str); }

void Terminal::debugln(String str) { this->debugf("%s\r\n", str.c_str()); }

void Terminal::debug(String str) { this->debugf("%s", str.c_str()); }

void Terminal::handleCharacter() {

  // Create a buffer to store the input string and the last input string
  static char cInputString[cmdMAX_INPUT_SIZE];

  // Grab and store grab the ptr reference to where the output buf lives
  char *pcOutputString = FreeRTOS_CLIGetOutputBuffer();
  uint8_t rxCmdIndex = 0;
  char rxChar = '\0';
  int rx = 0;

  // Print the prompt character
  this->stream->printf("\r\n> ");

  while (true) {
    // No data, so just yield and wait
    if (!this->stream->available()) {
      yield();
      continue;
    }

    rxChar = (char)this->stream->read();

    // Grab the mutex so we can echo and return command results
    if (xSemaphoreTake(this->txMutex, TERMINAL_TIMEOUT_TICK) == pdTRUE) {

      // Are we one character away from reaching buffer length or was the last
      // char a '\r'?. If so execute commands
      if (rxCmdIndex == cmdMAX_INPUT_SIZE - 2 || rxChar == '\r') {

        this->stream->println();

        /* Pass the received command to the command interpreter.  The
        command interpreter is called repeatedly until it returns
        pdFALSE	(indicating there is no more output) as it might
        generate more than one string. */
        BaseType_t xReturned = pdTRUE;
        for (;;) {
          // Get the command output from the processing function, this goes to
          // std out
          xReturned = FreeRTOS_CLIProcessCommand(
              cInputString, pcOutputString,
              (size_t)configCOMMAND_INT_MAX_OUTPUT_SIZE);

          this->stream->println(pcOutputString);

          // Short circuit when we finally don't have any more commands to
          // execute
          if (xReturned == pdFALSE) {
            break;
          }
        }

        // Print the prompt character
        this->stream->print("> ");

        // Clean up the input string buffer and the output buffer
        memset(cInputString, 0x00, cmdMAX_INPUT_SIZE);
        memset((void *)pcOutputString, 0x00, configCOMMAND_INT_MAX_OUTPUT_SIZE);
        rxCmdIndex = 0;

        // Check if the character is a printable ascii character between space
        // and ~
      } else if (rxChar >= 32 && rxChar <= 126) {

        // Echo the character back and copy it to the cmd buffer
        this->stream->print(rxChar);
        cInputString[rxCmdIndex] = rxChar;
        rxCmdIndex++;
      }

      xSemaphoreGive(this->txMutex);
    }

    yield();
  }
}