#include <stdlib.h>
#include <unistd.h>
/*
    Read the next line according to the file descriptor fd
    fd- The file descriptor
    return the next line in the file, on error or end of page return null
*/
char *get_next_line(int fd) {
    int charCount = 0;
    char* result = calloc(10, sizeof(char));
    ssize_t charsRead = 0;
    while(1) {
        charsRead = read(fd, result + sizeof(char) * charCount, 1);
        if (charsRead <= 0) break;
        charCount += charsRead;
        if (charCount % 10 == 0) {
            result = realloc(result, (charCount + 10) * sizeof(char));
            if (result == 0) return NULL;
        }
    }
    if (charsRead == -1) return NULL;
    // shrink down the memory and add null at the end
    result = realloc(result, (charCount + 1) * sizeof(char));
    if (result == 0) return NULL;
    result[(charCount) * sizeof(char)] = 0;
    return result;
}