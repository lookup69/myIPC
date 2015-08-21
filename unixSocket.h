/*
 *
 * @brief   
 *
 * @author  Archer Chang
 * @file    
 * @date    
 *
 */

#ifndef _LOOKUP69_UNIX_SOCKET_H_
#define _LOOKUP69_SOCKET_H_

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

namespace lkup69 {
    class UnixSocket
    {
    private:
        struct sockaddr_un  m_srvAddr;
        int                 m_socket;
        bool                m_bBeServer;
        bool                m_isConnected;

    public:
        explicit UnixSocket(const char *srvFilePath, bool bBeServer = false);
        virtual ~UnixSocket();

        int ipcInit(bool bAutoListenOrConnect = true);

        virtual bool isServer(void) const
        {
            return m_bBeServer;
        }

        virtual int getSockFd(void) const
        {
            return m_socket;
        }

        int Listen(int maxConnection = 1)
        {
            return listen(m_socket, maxConnection);
        }

        int Accept(void)
        {
            return accept(m_socket, NULL, 0);
        }

        int Connect(void);
        int Read(void *buf, size_t len, int cliSocket = -1);
        int Write(void *buf, size_t len, int cliSocket = -1);

        static int Close(int socket)
        {
            int ret = close(socket);

            socket = -1;
            return ret;
        }

        void fdClear(fd_set &set)
        {
            FD_CLR(m_socket, &set);
        }

        void fdSet(fd_set &set)
        {
            FD_SET(m_socket, &set);
        }

        int isFDSet(fd_set &set)
        {
            return FD_ISSET(m_socket, &set);
        }
    };
}
#endif
