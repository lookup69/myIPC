/*
 * @brief   
 *
 * @author  Archer Chang
 * @file    
 * @date    2013-Feb-19
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "unixSocketIpc.h"

#define MAX_CLIENT     1000

using namespace std;

CUnixSocketIpc::CUnixSocketIpc(const char *srvFilePath, bool bBeServer) 
    : m_sockfd(-1), m_bBeServer(bBeServer)

{
    memset(&m_srvAddr, 0, sizeof(m_srvAddr));

    m_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

    if(m_sockfd < 0) {
        throw ("CUnixSocketIpc::CUnixSocketIpc");
    }

    m_srvAddr.sun_family = AF_UNIX;
    strncpy(m_srvAddr.sun_path, srvFilePath, sizeof(m_srvAddr.sun_path) - 1);

    if(bBeServer) {
        remove(srvFilePath);

        //printf("(server) bind():%s ..................(1)\n", m_srvAddr.sun_path);
        if(bind(m_sockfd, (struct sockaddr *) &m_srvAddr, sizeof(struct sockaddr_un)) == -1) {
            close(m_sockfd);
            throw("CUnixSocketIpc::CUnixSocketIpc");
        }
        // let everybody can read/write
        chmod(m_srvAddr.sun_path, S_IRWXU|S_IRWXG|S_IRWXO);
        
    } 
}

CUnixSocketIpc::~CUnixSocketIpc()
{
    if(m_bBeServer) { 
        if(strlen(m_srvAddr.sun_path) != 0) {
            remove(m_srvAddr.sun_path);
        }
    }

    if(m_sockfd != -1) 
        close(m_sockfd);
}

int CUnixSocketIpc::Listen(int maxConnection)
{
    return listen(m_sockfd, maxConnection);
}

int CUnixSocketIpc::Connect(void)
{
    return connect(m_sockfd, (struct sockaddr *) &m_srvAddr, sizeof(struct sockaddr_un));
}

int CUnixSocketIpc::Accept(void)
{
    m_cliSockFd = accept(m_sockfd, NULL, 0);

    return m_cliSockFd;
}

int CUnixSocketIpc::Read(void *buf, size_t len)
{
    memset(buf, 0, len);

    if(m_bBeServer) {
        return read(m_cliSockFd, buf, len);
    }

    return read(m_sockfd, buf, len);
}

int CUnixSocketIpc::Write(void *buf, size_t len)
{
    if(m_bBeServer) {
        return write(m_cliSockFd, buf, len);
    }

    return write(m_sockfd, buf, len);
}

void CUnixSocketIpc::Close(int sockfd)
{
    close(sockfd);
}

