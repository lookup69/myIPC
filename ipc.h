#ifndef _MY_IPC_H_
#define _MY_IPC_H_

#include <unistd.h>
#include <sys/types.h>


class CIpc
{
public:
    CIpc(void){};
    virtual ~CIpc(void){};
    
    virtual const char *getIpcTypeName(void) const = 0;
    virtual int getFd(void) const = 0;
    virtual int Read(void *buf, size_t len, int fd) = 0;
    virtual int Write(void *buf, size_t len, int fd) = 0;
    virtual void fdClear(fd_set &set) = 0;
    virtual void fdSet(fd_set &set) = 0;
    virtual int isFDSet(fd_set &set) = 0;
};

#endif