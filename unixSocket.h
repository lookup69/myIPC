#ifndef _MY_UNIX_SOCKET_H_
#define _MY_UNIX_SOCKET_H_

#include "ipc.h"

#include <sys/socket.h>
#include <sys/un.h>
#include <string>

class CUnixIpc : public CIpc
{
private:
    std::string         m_typeName;
    std::string         m_filePath;
    struct sockaddr_un  m_addr;
    int                 m_sockfd;
    bool                m_bCreateUnixSockServer;

public:
    CUnixIpc(const char *filePath, bool bCreateFifo = false, bool bUnblock = false);
    virtual ~CUnixIpc();

    int Accept(void)
    {
        return accept(m_sockfd, NULL, NULL);
    }
    
    virtual const char *getIpcTypeName(void) const
    {
        return m_typeName.c_str();
    }
    
    virtual int getFd(void) const
    {
        return m_sockfd;
    }

    virtual int Read(void *buf, size_t len)
    {
        return read(m_sockfd, buf, len);
    }
    
    virtual int Write(void *buf, size_t len)
    {
        return write(m_sockfd, buf, len);
    }
    
    virtual void fdClear(fd_set &set)
    {
        return FD_CLR(m_sockfd, &set);
    }
    
    virtual void fdSet(fd_set &set)
    {
        return FD_SET(m_sockfd, &set);
    }
    
    virtual int isFDSet(fd_set &set)
    {
        return FD_ISSET(m_sockfd, &set);
    }
};

#endif
