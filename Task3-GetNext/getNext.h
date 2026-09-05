#ifndef BUFFER_SIZE
# define BUFFER_SIZE 10000000
#endif

char *get_next_line(int fd);
int strlength(char* strn);
char* strnJoin(char* str1, char* str2);
char* advanceBuffer(char* buffer);
char* getLine(char* buffer);