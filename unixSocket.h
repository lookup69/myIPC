/*
 *
 * @brief   
 *
 * @author  Archer Chang
 * @file    
 * @date    
 *
 */

#ifndef _LKUP69_UNIX_SOCKET_H_
#define _LKUP69_UNIX_SOCKET_H_

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string>

namespace lkup69 {
    class UnixSocket
    {
    private:
        struct sockaddr_un  m_socketPath;
        int                 m_socket;
        bool                m_bServer;
        bool                m_isConnected;
        std::string         m_path;

        UnixSocket(const UnixSocket &);
        UnixSocket &operator=(const UnixSocket &);
    public:
        explicit UnixSocket(const std::string &path = std::string{}, bool bBeServer = false);
        virtual ~UnixSocket();

        int init(bool bAutoListenOrConnect = true, const std::string &path = std::string{});

        bool isServer(void) const
        {
            return m_bServer;
        }

        int getSockFd(void) const
        {
            return m_socket;
        }

        int listen(int maxConnection = 1)
        {
            return ::listen(m_socket, maxConnection);
        }

        int accept(void)
        {
            return ::accept(m_socket, NULL, 0);
        }

        int connect(void);
        int read(void *buf, size_t len, int cliSocket = -1);
        int write(void *buf, size_t len, int cliSocket = -1);

        int close(int socket)
        {
            return ::close(socket);
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
