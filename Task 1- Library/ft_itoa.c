#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

char *ft_itoa(int n) {
    int isNegative = 0;
    int counter = 0;
    if (n == 0) {
        char *result = malloc(2);
        result[0] = '0';
        result[1] = 0;
        return result; 
    }
    if (n < 0) {
        isNegative = 1;
    }
    
    char charachter;
    int i = 0;
    int array[(int)log10(INT_MAX) + 1];
    while (n != 0) {
        charachter = abs((n - ((n/10)*10)));
        array[i] = charachter;
        n /= 10;
        i++;
    } // now each number is in their own spot, but reversed
    if (isNegative) {
        i++; //for the malloc
    }
    char *result = malloc(i);
    if (result == 0) {
        return 0;
    }
    if (isNegative) {
        result[0] = '-';
        counter++;
        i--;
    }
    result[i] = 0; //set end of string to 0
    i--;
    while (i >= 0) {
        result[counter] = (char) (array[i] + 48);
        i--;
        counter++;
    }
    return result;
}
