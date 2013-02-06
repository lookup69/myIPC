#include "debugUtility.h"

#include <stdlib.h>
#include <stdio.h>

#include "fifo.h"


using namespace std;

CFifo::CFifo(const char *filePath,  bool bCreateFifo, 
             int fileFlags, mode_t fifoMode)
    : m_typeName("FIFO"), m_filePath(filePath), m_bCreateFifo(bCreateFifo)
{
    if(bCreateFifo) {
        unlink(filePath);
        
        if(mkfifo(filePath, fifoMode) < 0) {
            DEBUG_PRINT("mkfifo(%s) ... fail\n", filePath);
            throw("CFifo::CFifo");
        }
    }
    
    if((m_fd = open(filePath, fileFlags)) < 0) {
        DEBUG_PRINT("open(%s) ... fail\n", filePath);
        throw("CFifo::CFifo");
    }
    
}

CFifo::~CFifo()
{      
    if(m_bCreateFifo)
        unlink(m_filePath.c_str());
}


