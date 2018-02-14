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

using namespace std;
using namespace lkup69;

UnixSocket::UnixSocket(const std::string &path, bool bBeServer)
    : m_socket(-1), m_bServer(bBeServer), m_isConnected(false), m_path(path)
{
}

UnixSocket::~UnixSocket()
{
    if(m_bServer) {
        if(strlen(m_socketPath.sun_path) != 0) {
            remove(m_socketPath.sun_path);
        }
    }

    if(m_socket != -1) 
        close(m_socket);
}

int UnixSocket::init(bool bAutoListenOrConnect, const string &path)
{
    if(path.size() > 0)
        m_path = path;

    if((m_path.size() == 0) || (m_path.size() > sizeof(m_socketPath.sun_path) - 1))
        return -1;

    memset(&m_socketPath, 0, sizeof(m_socketPath));
    m_socketPath.sun_family = AF_UNIX;
    strncpy(m_socketPath.sun_path, m_path.c_str(), sizeof(m_socketPath.sun_path) - 1);

    m_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if(m_socket < 0) 
        return -1;

    if(m_bServer) {
        remove(m_socketPath.sun_path);

        if(bind(m_socket, (struct sockaddr *)&m_socketPath, sizeof(struct sockaddr_un)) == -1) {
            close(m_socket);
            m_socket = -1;
            return -1;
        }
        // let everybody can read/write
        chmod(m_socketPath.sun_path, S_IRWXU | S_IRWXG | S_IRWXO);
        if(bAutoListenOrConnect)
            return this->listen();
    } else {
        if(bAutoListenOrConnect) 
            return this->connect();
    }

    return 0;
}

int UnixSocket::connect(void)
{
    int ret = ::connect(m_socket, (struct sockaddr *)&m_socketPath, sizeof(struct sockaddr_un));
    
    if(ret == 0)
        m_isConnected = true;

    return ret;
}

int UnixSocket::read(void *buf, size_t len, int cliSocket)
{
    memset(buf, 0, len);
    if(m_bServer) {
        if(cliSocket < 0)
            return -1;
        return ::read(cliSocket, buf, len);
    }
    
    if(m_isConnected)
        return ::read(m_socket, buf, len);

    return -1;
}

int UnixSocket::write(void *buf, size_t len, int cliSocket)
{
    if(m_bServer) {
        if(cliSocket < 0)
            return -1;
        return ::write(cliSocket, buf, len);
    }

    if(m_isConnected) 
        return ::write(m_socket, buf, len);

    return -1;
}

#ifdef UNIT_TEST
static void print_help(void)
{
    printf("Usage:\n");
    printf("\tunixSocket {server|client} file_path\n");
    exit(0);
}

int main(int argc, char *argv[])
{
    if(argc < 3)
        print_help();

    if(strcmp(argv[1], "server") == 0) {
        UnixSocket us(argv[2], true);
        int        cs;
        if(us.init() < 0) {
            fprintf(stderr, "us.init() ... err\n");
            exit(0);
        }

        cs = us.accept();
        if(cs < 0) {
            fprintf(stderr, "us.init() ... err\n");
            exit(0); 
        }

        while(1) {
            const char *str = "hello";

            us.write((void *)str, strlen(str), cs);
            sleep(1);
        }
    }

    if(strcmp(argv[1], "client") == 0) {
        UnixSocket us(argv[2]);

        if(us.init() < 0) {
            fprintf(stderr, "us.init() ... err\n");
            exit(0);
        }

        while(1) {
            char buf[4096] = {0};

            if(us.read(buf, sizeof(buf)) < 0) {
                fprintf(stderr, "us.read() ... err\n");
                break;
            }
            fprintf(stdout, "buf:%s\n", buf);
        }
    }

    return 0;
}
#endif
