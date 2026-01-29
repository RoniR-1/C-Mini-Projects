#include <stddef.h>
#include <stdlib.h>
#include "libft.h"

static inline void freeDoublePointer(char **s, size_t len) {
    size_t i = 0;
    while (i < len) {
        free(s[i]);
        i++;
    }
    free(s);
}

char **ft_split(char const *s, char c) {
    size_t totalIndex = 0;
    size_t i = 0;
    size_t sLength = ft_strlen(s);
    // devide by 2, add 1 for odd numbers, add 1 for null
    char** result = malloc((((sLength >> 1) + 2) * (sizeof(char*))));
    if (result == 0) {return 0;} //null check
    //first pass: get indices
    //second pass: use ft_substr
    int *array = malloc(sizeof(int) * ((sLength >> 1) + 2));
    if (array == 0) {return 0;} //null check
    while (s[i] != 0) {
        if (s[i] == c) {
            array[totalIndex] = i;
            totalIndex++;
        }
        i++;
    }
    if (totalIndex == 0) {
        char *newWord = ft_strdup(s);
        if (newWord == 0) {
            free(array);
            free(result);
            return 0;
        } else {
            result[0] = newWord;
            result[1] = 0;
            return result;
        }
        
    }

    // should have array like [1,5,6,7,10]
    // if diff between n and n + 1 < 2, skip: 1, 2 -> skip
    // 
    // ft_substr
    size_t currentIndex = 0;
    i = 0;
    size_t iplus1= 0;
    size_t currentWord = 0;
    if (array[0] > 0) {
        char* newWord = ft_substr(s, 0, array[0]);
        if (newWord == 0) {
            freeDoublePointer(result, 0);
            free(array);
            return 0;
        }
        if (newWord != 0) {
            result[currentWord] = newWord;
            currentWord++;
        }
    }
    while (currentIndex < totalIndex) { // so while array[] still has elements
        i = array[currentIndex];
        if (currentIndex < totalIndex-1) {
            iplus1 = array[currentIndex + 1];
        }
        if (currentIndex >= totalIndex-1) {
            iplus1 = sLength;
        }
        if (iplus1 - i <= 1) { // so if 4,5 -> skip
            currentIndex++;
            continue;
        }
        char* newWord = ft_substr(s, i + 1, iplus1 - i -1);
        if (newWord == 0) {
            freeDoublePointer(result, currentWord + 1);
            free(array);
            return 0;
        }
        if (newWord != 0) {
            result[currentWord] = newWord;
            currentWord++;
        }
        currentIndex++;
    }
    result[currentWord] = 0;
    free(array);
    return result;
}
