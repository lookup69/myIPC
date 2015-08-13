/*
 * @brief   
 *
 * @author  Archer Chang
 * @file    
 * @date    2013-Feb-19
 *
 */

#include "unixSocket.h"

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


#define MAX_CLIENT     1000

using namespace lookup69;

UnixSocket::UnixSocket(const char *srvFilePath, bool bBeServer) 
    : m_socket(-1), m_bBeServer(bBeServer), m_isConnected(false)
{
    memset(&m_srvAddr, 0, sizeof(m_srvAddr));
    m_srvAddr.sun_family = AF_UNIX;
    strncpy(m_srvAddr.sun_path, srvFilePath, sizeof(m_srvAddr.sun_path) - 1);
}

UnixSocket::~UnixSocket()
{
    if(m_bBeServer) { 
        if(strlen(m_srvAddr.sun_path) != 0) {
            remove(m_srvAddr.sun_path);
        }
    }

    if(m_socket != -1) 
        close(m_socket);
}

int UnixSocket::ipcInit(bool bAutoListenOrConnect)
{
    m_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    if(m_socket < 0) 
        return -1;

    if(m_bBeServer) {
        remove(m_srvAddr.sun_path);

        if(bind(m_socket, (struct sockaddr *) &m_srvAddr, sizeof(struct sockaddr_un)) == -1) {
            close(m_socket);
            m_socket = -1;
            return -1;
        }
        // let everybody can read/write
        chmod(m_srvAddr.sun_path, S_IRWXU|S_IRWXG|S_IRWXO);
        if(bAutoListenOrConnect) 
            return Listen();
    } else {
        if(bAutoListenOrConnect) 
            return Connect();
    }

    return 0;
}

int UnixSocket::Connect(void)
{
    int ret = connect(m_socket, (struct sockaddr *) &m_srvAddr, sizeof(struct sockaddr_un));
    
    if(ret == 0)
        m_isConnected = true;
    else
        m_isConnected = false;
    return ret;
}

int UnixSocket::Read(void *buf, size_t len, int cliSocket)
{
    memset(buf, 0, len);

    if(m_bBeServer) {
        if(cliSocket < 0) 
            return -1;
        return read(cliSocket, buf, len);
    }
    
    if(m_isConnected)
        return -1;
        
    return read(m_socket, buf, len);
}

int UnixSocket::Write(void *buf, size_t len, int cliSocket)
{
    if(m_bBeServer) {
        if(cliSocket < 0) 
            return -1;
        return write(cliSocket, buf, len);
    }

    if(m_isConnected)
        return -1;

    return write(m_socket, buf, len);
}

