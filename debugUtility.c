/*
 * The following license idents are currently accepted as indicating free
 * software 
 *
 *  "GPL"               [GNU Public License v2 or later]
 *
 *
 * @Author	Archer Chang
 * @file
 * date
 *
 */

#include "debugUtility.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>


int debug_write_log(char *file, char *log)
{
    struct tm *t;
    time_t time_;
    char *line;
    FILE *fp;

    line = (char *) malloc(strlen(log) + 20);
    if(line){
        memset(line, 0, strlen(log) + 20);
    } else {
        return -1;
    }

    fp = fopen(file, "a+");
    if(!fp) {
        free(line);
        return -1;
    }
    time(&time_);
    t = gmtime(&time_);
    sprintf(line, "[%2d:%2d:%2d] %s\n", t->tm_hour, t->tm_min, t->tm_sec, log);
    fwrite(line, 1, strlen(line), fp);
    fflush(fp);
    fclose(fp);
    free(line);

    return 0;
}

void memory_dump(const char *buf, int len)
{
    char hex_buf[256];
    char ascii_buf[256];
    char isFlush = 0;
    int i;
    int offset;
    
    printf("Len:%d\n", len);
    for(offset = 0, i = 0; i < len; i++)
    {
        if((i % 16) == 0){
            if(i != 0) {
                isFlush = 1;
                printf("[%04X]: %s   %s\n", i - 16, hex_buf, ascii_buf);
                memset(hex_buf, 0, sizeof(hex_buf));
                memset(ascii_buf, 0, sizeof(ascii_buf));
            }
        }
        offset = i % 16;
        sprintf((hex_buf + (offset * 3)), "%02X ", (unsigned  char)*(buf + i));
        isFlush = 0;
        if((buf[i] >= 32) && (buf[i] <= 126))
            sprintf((ascii_buf + offset), "%c", buf[i]);
        else
            sprintf((ascii_buf + offset), ".");
    }
    if(!isFlush) {
        int stuffCnt = (i % 16) ? 16 - (i % 16) : 0;;

        printf("[%04X]: %s", i, hex_buf);
        for(i = 0; i < stuffCnt; i++)
            printf("   ");
        printf("   %s\n", ascii_buf);
    }
}

