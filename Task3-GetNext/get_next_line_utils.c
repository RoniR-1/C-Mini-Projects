#include <stdlib.h>

int strlength(char* strn) {
    if (strn == NULL) return 0;
    int len = 0;
    while(strn[len] != 0) len++;
    return len;
}

// str1 globalBuffer, str2 local buffer
char* strnJoin(char* str1, char* str2) {
    if(!str2) return (free(str1), NULL);
    char* str = malloc(strlength(str1) + strlength(str2) + 1);
    if (!str) return (free(str1), NULL);
    int i = 0; int j = 0;
    while (str1 && str1[i]) {
        str[i] = str1[i];
        i++;
    }
    while (str2 && str2[j]) {
        str[i+j] = str2[j];
        j++;
    }
    str[i+j] = 0;
    free(str1);
    return str;
}

char* advanceBuffer(char* buffer) {
    // the end of the buffer has to be null terminated
    int firstNewLine = -1;
    int len = 0;
    while(buffer[len] != 0) {
        if (buffer[len] == '\n' && firstNewLine == -1) firstNewLine = len+1; 
        len++;
    }
    if (firstNewLine == -1) return NULL;
    
    // if new line is null, return null
    if (len == firstNewLine) {
        free(buffer);
        buffer = NULL;
        return NULL;
    }

    char* newBuffer = malloc((len - firstNewLine + 1) * sizeof(char));
    if (newBuffer == NULL) return NULL;
    
    int i = 0;
    while(firstNewLine + i < len) {
        newBuffer[i] = buffer[firstNewLine+i];
        i++;
    }
    newBuffer[i] = 0;
    return newBuffer;
}

char* getLine(char* buffer) {
    if (buffer == NULL || buffer[0] == 0) return NULL;
    int index = 0;

    while (buffer[index] != '\0' && buffer[index] != '\n') {
        index++;
    }
    if (buffer[index] == 0) return NULL;
    char* newLine = malloc(sizeof(char) * (index+2));
    if (newLine == NULL) return NULL;

    for(int i = 0; i <= index; i++) {
        newLine[i] = buffer[i];
    }
    newLine[index+1] = 0;
    return newLine;
}
