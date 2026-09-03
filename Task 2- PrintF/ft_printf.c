#include <stdarg.h>
#include <unistd.h> // for syscall to write
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <limits.h>
#include "../Task 1- Library/libft.h"


int hexPrinter(void*ptr, int size, int lowerCase, int printPointer);
char* uint_to_string(unsigned int n);
/*
    A copy of printf, %-,%0,%.,%#,%+,% (space) aren't implemented yet
• %c Prints a single character.
• %s Prints a string (as defined by the common C convention). null = (null)
• %p The void * pointer argument has to be printed in hexadecimal format. null = (nil)   
• %d Prints a decimal (base 10) number.
• %i Prints an integer in base 10.
• %u Prints an unsigned decimal (base 10) number.
• %x Prints a number in hexadecimal (base 16) lowercase format.
• %X Prints a number in hexadecimal (base 16) uppercase format.
• %% Prints a percent sign
• %- The result of the conversion shall be left-justified within the field. The conversion is right-justified if this flag is not specified.
• %0 For d, i, o, u, x, X, a, A, e, E, f, F, g, and G conversion specifiers, leading zeros (following any indication of sign or base) are used to pad to the field width; no space padding is performed. If the '0' and '-' flags both appear, the '0' flag is ignored. For d, i, o, u, x, and X conversion specifiers, if a precision is specified, the '0' flag is ignored. [XSI] [Option Start]  If the '0' and '" flags both appear, the grouping characters are inserted before zero padding. For other conversions, the behavior is undefined.
• %. An optional precision that gives the minimum number of digits to appear for the d, i, o, u, x, and X conversion specifiers; the number of digits to appear after the radix character for the a, A, e, E, f, and F conversion specifiers; the maximum number of significant digits for the g and G conversion specifiers; or the maximum number of bytes to be printed from a string in the s [XSI] [Option Start]  and S [Option End]  conversion specifiers. The precision takes the form of a period ( '.' ) followed either by an asterisk ( '*' ), described below, or an optional decimal digit string, where a null digit string is treated as zero. If a precision appears with any other conversion specifier, the behavior is undefined.
• %# Specifies that the value is to be converted to an alternative form. For o conversion, it increases the precision (if necessary) to force the first digit of the result to be zero. For x or X conversion specifiers, a non-zero result shall have 0x (or 0X) prefixed to it. For a, A, e, E, f, F, g , and G conversion specifiers, the result shall always contain a radix character, even if no digits follow the radix character. Without this flag, a radix character appears in the result of these conversions only if a digit follows it. For g and G conversion specifiers, trailing zeros shall not be removed from the result as they normally are. For other conversion specifiers, the behavior is undefined
• %+ The result of a signed conversion shall always begin with a sign ( '+' or '-' ). The conversion shall begin with a sign only when a negative value is converted if this flag is not specified.
• % (space) If the first character of a signed conversion is not a sign or if a signed conversion results in no characters, a <space> shall be prefixed to the result. This means that if the <space> and '+' flags both appear, the <space> flag shall be ignored.
 */
/*if successfull, return amount of bytes transmitted. else return neg number (-1) */
/*write(1, "c", 1); to write c*/
int ft_printf(const char* format, ...) {
    // I won't implement the hard fcounterlags for now
    va_list ap;
    va_start(ap, format);
    // 'ab' -> first = b, second = a in the stack
    int charCounter = 0;
    char firstChar = format[0]; 
    char secondChar;
    int counter = 1;
    while (firstChar != 0) {
        secondChar = firstChar;
        firstChar = format[counter];
        if (secondChar == -1) {
            counter++;
            continue;
        }
        int printedBytes = 0;
        if (secondChar == '%') {
            if (firstChar == '%') printedBytes += write(1, "%", 1);
            else if (firstChar == 'c') {
                char c = (char)va_arg(ap, int);
                printedBytes = write(1, &c, 1);
            }
            else if (firstChar == 's') {
                char* c = (char*)va_arg(ap, char*);
                if (c == NULL) c = "(null)";
                printedBytes = write(1, c, ft_strlen(c));
            }
            else if (firstChar == 'p') {
                void* c = va_arg(ap, void*);
                printedBytes = hexPrinter(c, sizeof(c), 1, 1);
            }
            else if (firstChar == 'i' || firstChar == 'd') {
                char* c = ft_itoa(va_arg(ap, int));
                printedBytes = write(1, c, ft_strlen(c));
                free(c);
            }
            else if (firstChar == 'u') {
                char* c = uint_to_string(va_arg(ap, unsigned int));
                printedBytes = write(1, c, ft_strlen(c));
                free(c);
            }
            else if (firstChar == 'x') {
                unsigned int c = va_arg(ap, unsigned int);
                printedBytes = hexPrinter(&c, sizeof(unsigned int), 1, 0);
            }
            else if (firstChar == 'X') {
                unsigned int c = va_arg(ap, unsigned int);
                printedBytes = hexPrinter(&c, sizeof(unsigned int), 0, 0);
            }
            firstChar = -1; // so when %%u occurs, after processsing %%, it becomes %1u and u isnt seen as special
        }

        else {// if nothing special, print secondChar
            printedBytes = write(1, &secondChar, 1);
        }
        // if error, return -1. else add the printede bytes to the charCounter and increment counter
        if (printedBytes == -1) return -1;  
        charCounter += printedBytes;
        counter++;
    }

    va_end(ap);
    return charCounter;
}
/*
    Prints unsigned integer
    n- the unsigned integer to be 
*/
char* uint_to_string(unsigned int n) {
    // stolen from ft_itoa and removing the negative parts
    int counter = 0;
    if (n == 0) {
        char *result = malloc(2);
        result[0] = '0';
        result[1] = 0;
        return result; 
    }
    
    char charachter;
    int i = 0;
    int array[(int)log10(UINT_MAX) + 1]; // UINT_MAX instead of int_max
    while (n != 0) {
        charachter = (n - ((n/10)*10));
        array[i] = charachter;
        n /= 10;
        i++;
    } // now each number is in their own spot, but reversed

    char *result = malloc(i);
    if (result == 0) {
        return 0;
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


/* 
    Prints the pointer or the data that points to the pointer
    ptr- the pointer
    size- the size of data to be printed in bytes
    lowercase- use lowercase hexadecimal when 1, uppercase when 0
    printPointer- 1 to print the actual pointer, 0 to print the data thats pointed by the pointer
*/
int hexPrinter(void*ptr, int size, int lowerCase, int printPointer) {
    if (printPointer && ptr == NULL) return write(1, "(nil)", 5);
    char* map = "0123456789abcdef";
    if (lowerCase == 0) map = "0123456789ABCDEF";
    int charCounter = 0;
    if (printPointer) {
        charCounter += write(1, "0x", 2);
    }
    
    uintptr_t number = *(uintptr_t*) ptr;
    if (printPointer == 1) number = (uintptr_t) ptr;

    int mask = 0xF;
    int shift = (size*8) - 4; // for example: 64 bit -> 60 bit, so when we shift the num, we shift it 60 to the right, remains 4

    while(shift > 0 && ((number >> shift) & mask) == 0) {
        shift -= 4;
    }

    while(shift >= 0) {
        int digit = (number >> shift) & mask;
        charCounter += write(1, &map[digit], 1);
        shift -= 4;
    }

    return charCounter;
}