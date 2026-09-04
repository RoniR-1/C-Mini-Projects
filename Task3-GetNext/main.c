#include <stdio.h>
#include <fcntl.h>
#include "getNext.h"

int main(void) {
    int fd = open("/data", O_RDONLY);
    if (fd == -1) {
        printf("error with file opening");
        return 1;
    }
    printf(get_next_line(fd));
    printf(get_next_line(fd));
    printf(get_next_line(fd));
    printf(get_next_line(fd));
    printf(get_next_line(fd));
    printf(get_next_line(fd));
    return 0;
}