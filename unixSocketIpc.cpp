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
#include <errno.h>
#include "unixSocketIpc.h"

#define MAX_CLIENT     1000

using namespace std;


CUnixSocketIpc::CUnixSocketIpc(const char *srvFilePath, bool bIsServer) 
    : m_sockfd(-1), m_bIsServer(bIsServer)

{
    memset(&m_srvAddr, 0, sizeof(m_srvAddr));

    m_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

    if(m_sockfd < 0) {
        throw ("CUnixSocketIpc::CUnixSocketIpc");
    }

    m_srvAddr.sun_family = AF_UNIX;
    strncpy(m_srvAddr.sun_path, srvFilePath, sizeof(m_srvAddr.sun_path) - 1);

    if(bIsServer) {
        remove(srvFilePath);

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
    if(m_bIsServer) { 
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
#if 0
    struct timeval timeout;      
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    if (setsockopt (m_sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
        throw ("CUnixSocketIpc::CUnixSocketIpc");
    if (setsockopt (m_sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
        throw ("CUnixSocketIpc::CUnixSocketIpc");
#endif
    return connect(m_sockfd, (struct sockaddr *) &m_srvAddr, sizeof(struct sockaddr_un));
}

int CUnixSocketIpc::Accept(void)
{
_again:
    if((m_cliSockFd = accept(m_sockfd, NULL, 0)) < 0)
        if(errno == EINTR)
            goto _again;

    return m_cliSockFd;
}

int CUnixSocketIpc::Read(void *buf, size_t len, int sd)
{
    int ret;

    memset(buf, 0, len);
_again:
    if(m_bIsServer) {
        if(sd >= 0) 
            ret = read(sd, buf, len);
        else 
            ret = read(m_cliSockFd, buf, len);
    } else {
        ret = read(m_sockfd, buf, len);
    }

    if((ret < 0) && (errno == EINTR)) 
        goto _again;

    return ret;
}

int CUnixSocketIpc::Write(void *buf, size_t len, int sd)
{
    int ret;

_again:
    if(m_bIsServer) {
        if(sd >= 0) {
            ret = write(sd, buf, len);
        } else {
            ret = write(m_cliSockFd, buf, len);
        }
    } else {
        ret = write(m_sockfd, buf, len);
    }
	
    if((ret < 0) && (errno == EINTR)) 
        goto _again;
		
    return ret;
}

void CUnixSocketIpc::Close(int sockfd)
{
    close(sockfd);
}

