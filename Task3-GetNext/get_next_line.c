#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "getNext.h"

/*
    Read the next line according to the file descriptor fd
    fd- The file descriptor
    return the next line in the file, on error or end of page return null
*/
char *get_next_line(int fd) {
    static char* globalBuffer;
    char* localBuffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
    if (localBuffer == NULL) return NULL;
    
    // if newline is in global buffer, get it and advance global buffer and return new line
    char* newLine = getLine(globalBuffer);
    if (newLine != NULL) {
        globalBuffer = advanceBuffer(globalBuffer);
        free(localBuffer);
        return newLine;
    }
    int charsRead = 0;
    while(newLine == NULL) {
        charsRead = read(fd, localBuffer, BUFFER_SIZE);
        if (charsRead <= 0) break;

        localBuffer[charsRead] = 0;
        globalBuffer = strnJoin(globalBuffer, localBuffer);
        newLine = getLine(globalBuffer);
    }
    if (charsRead < 0) {
        return ((free(globalBuffer)), (globalBuffer = NULL), (free(localBuffer)), NULL);
    }
    if (charsRead == 0) {
        // end of file
        int size = strlength(globalBuffer);
        if (size == 0) return ((free(globalBuffer)), (free(localBuffer)), NULL);
        newLine = malloc(sizeof(char) * (size+1));
        int i = 0;
        while(i < size) {
            newLine[i] = globalBuffer[i];
            i++;
        }
        newLine[i] = 0;
        return ((free(globalBuffer)), (globalBuffer = NULL), (free(localBuffer)), newLine);
    }

    
    if (newLine != NULL) {
        free(localBuffer);
        globalBuffer = advanceBuffer(globalBuffer);
        return newLine;
    }
    return NULL;
}
