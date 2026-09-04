#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "getNext.h"

void test_fd(int fd, const char *title) // AI generated test cases for ease
{
    char *line;
    int line_num = 1;

    printf("\n--- %s ---\n", title);
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("Line %d: %s", line_num++, line);
        
        int len = 0;
        while (line[len])
            len++;
        if (len > 0 && line[len - 1] != '\n')
            printf("\n");

        free(line);
    }
    printf("Line %d: (NULL) [EOF / End of file]\n", line_num);
}

int main(void)
{
    int fd;

    // 1. Standard Multi-line File
    fd = open("data.txt", O_RDONLY);
    if (fd >= 0)
    {
        test_fd(fd, "Standard File Test");
        close(fd);
    }

    // 2. File Ending WITHOUT '\n'
    fd = open("no_newline.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    write(fd, "Line 1\nLine 2 without newline", 29);
    close(fd);

    fd = open("no_newline.txt", O_RDONLY);
    test_fd(fd, "No Trailing Newline Test");
    close(fd);
    unlink("no_newline.txt");

    // 3. Single Newline Only File ("\n")
    fd = open("single_nl.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    write(fd, "\n", 1);
    close(fd);

    fd = open("single_nl.txt", O_RDONLY);
    test_fd(fd, "Single Newline File Test");
    close(fd);
    unlink("single_nl.txt");

    // 4. Invalid File Descriptor Error Handling
    test_fd(-1, "Invalid FD (-1) Test");

    return (0);
}