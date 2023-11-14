#ifndef _DEBUG_H
#define _DEBUG_H

#include "Stream.h"

class Debug {
    
    public :
        Debug(bool init, Stream * debugStream);

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
        Stream * debugStream;
};

#endif