#include "debugUtility.h"

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#include "unixSocket.h"

using namespace std;

CUnixIpc::CUnixIpc(const char *filePath, bool bCreateUnixSockServer, bool bUnblock)
    : m_typeName("UNIX Sock"), m_filePath(filePath), m_bCreateUnixSockServer(bCreateUnixSockServer)

{
    m_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(m_sockfd < 0) {
        DEBUG_PRINT("socket() ... fail\n");
        throw ("CUnixIpc::CUnixIpc");
    }

    if(bUnblock) {
        if(fcntl(m_sockfd, O_NONBLOCK) < 0) {
            DEBUG_PRINT("fcntl() ... fail\n");
            throw ("CUnixIpc::CUnixIpc");
        }
    }
    
    memset(&m_addr, 0, sizeof(m_addr));
    m_addr.sun_family = AF_UNIX;
    strncpy(m_addr.sun_path, filePath, sizeof(m_addr.sun_path) - 1);

    // will become to server
    if(bCreateUnixSockServer) {
        remove(m_addr.sun_path);

        if(bind(m_sockfd, (struct sockaddr *) &m_addr, sizeof(struct sockaddr_un)) == -1) {
            DEBUG_PRINT("bind() ... fail\n");
            throw("CUnixIpc::CUnixIpc");
        }

        if(listen(m_sockfd, 10) == -1) {
            DEBUG_PRINT("listen() ... fail\n");
            throw("CUnixIpc::CUnixIpc");
        }
    // will become to client
    } else {
        if (connect(m_sockfd, (struct sockaddr *) &m_addr, sizeof(struct sockaddr_un)) == -1) {
            DEBUG_PRINT("connect() ... fail\n");
            throw("CUnixIpc::CUnixIpc");
        }
    }
}

CUnixIpc::~CUnixIpc()
{
    if(m_bCreateUnixSockServer)
        remove(m_addr.sun_path);
}



