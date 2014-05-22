/*
 *
 * @brief   
 *
 * @author  Archer Chang
 * @file    
 * @date    
 *
 */

#ifndef _UNIX_SOCKET_IPC_H_
#define _UNIX_SOCKET_IPC_H_

#include <sys/socket.h>
#include <sys/un.h>
#include <string>
#include <stdio.h>

class CUnixSocketIpc
{
private:
    struct sockaddr_un  m_srvAddr;
    int                 m_sockfd;
    int                 m_cliSockFd;
    bool                m_bIsServer;

public:
    CUnixSocketIpc(const char *srvFilePath, bool bBeServer = false);
    ~CUnixSocketIpc();
    
    bool isServer(void) const
    {
        return m_bIsServer;
    }

    int getSockFd(void) const
    {
        return m_sockfd;
    }

    int Listen(int maxConnection = 1);
    int Accept(void);
    int Connect(void);
    int Read(void *buf, size_t len, int sd = -1);
    int Write(void *buf, size_t len, int sd = -1);
    void Close(int sockfd);

    void fdClear(fd_set &set)
    {
        FD_CLR(m_sockfd, &set);
    }
    
    void fdSet(fd_set &set)
    {
        FD_SET(m_sockfd, &set);
    }
    
    int isFDSet(fd_set &set)
    {
        return FD_ISSET(m_sockfd, &set);
    }
};

#endif
