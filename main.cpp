#include "fifo.h"
#include "unixSocket.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    char buf[PIPE_BUF];

    try{
        CFifo fifo("/tmp/aaaa.xxx", 1, O_RDONLY);

        while(1) {
            fifo.Read(buf, PIPE_BUF);
            printf("%s\n", buf);
        }
    } catch(const char *err) {
        printf("%s\n", err);
    }

}