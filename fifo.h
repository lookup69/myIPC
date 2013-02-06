#ifndef _MY_FIFO_H_
#define _MY_FIFO_H_

#include "ipc.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <string>

#define DEFAULT_FIFO_MODE   S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP


class CFifo : public CIpc
{
private:
    std::string m_typeName;
    std::string m_filePath;
    int         m_fd;
    bool        m_bCreateFifo;
public:
    /* BCreateFifo = true 
       if fifo not exist then crate, if fifo exist
       delete and re-create
    */
    CFifo(const char *filePath,  bool bCreateFifo = 0, 
          int fileFlags = O_RDWR, mode_t fifoMode = DEFAULT_FIFO_MODE);

    virtual ~CFifo();
    
    virtual const char *getIpcTypeName(void) const
    {
        return m_typeName.c_str();
    }

    virtual int getFd(void) const
    {
        return m_fd;
    }

    virtual int Read(void *buf, size_t len)
    {
        return read(m_fd, buf, len);
    }

    virtual int Write(void *buf, size_t len)
    {
        return write(m_fd, buf, len);
    }
    
    virtual void fdClear(fd_set &set)
    {
        return FD_CLR(m_fd, &set);
    }

    virtual void fdSet(fd_set &set)
    {
        return FD_SET(m_fd, &set);
    }

    virtual int isFDSet(fd_set &set)
    {
        return FD_ISSET(m_fd, &set);
    }
};

#endif