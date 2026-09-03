#include "ft_printf.h"
#include <stdio.h>
#include <limits.h>

int main(void) {
    int res1, res2;
    int test_var = 42;
    
    // Test 0: Just a string
    printf("[Test 0: Just a string]\n");
    res1 = printf(      "Hello world\n");
    res2 = ft_printf(   "Hello world\n");
    printf("Return Values -> STD: %d | FT: %d\n\n", res1, res2);

    // Test 1: Simple Characters and Strings
    printf("[Test 1: Single Char & String]\n");
    res1 = printf(      "STD: %c %s %c\n", 'A', "Hello, World!", 'Z');
    res2 = ft_printf(   "FTT: %c %s %c\n", 'A', "Hello, World!", 'Z');
    printf("Return Values -> STD: %d | FT: %d\n\n", res1, res2);

    // Test 2: Null String Handling
    printf("[Test 2: NULL String]\n");
    char *null_str = NULL;
    res1 = printf("STD: %s\n", null_str);
    res2 = ft_printf("FT : %s\n", null_str);
    printf("Return Values -> STD: %d | FT: %d\n\n", res1, res2);

    // Test 3: Standard Integer and Decimal
    printf("[Test 3: Signed Integers %%d and %%i]\n");
    res1 = printf(      "STD: %d %i %d\n", 0, -42, 2147483647);
    res2 = ft_printf(   "FT : %d %i %d\n", 0, -42, 2147483647);
    printf("Return Values -> STD: %d | FT: %d\n\n", res1, res2);

    // Test 4: Integer Minimum Boundary (INT_MIN)
    printf("[Test 4: INT_MIN Boundary]\n");
    res1 = printf(      "STD: %d %i\n", INT_MIN, INT_MIN);
    res2 = ft_printf(   "FT : %d %i\n", INT_MIN, INT_MIN);
    printf("Return Values -> STD: %d | FT: %d\n\n", res1, res2);

    // Test 5: Unsigned Integers (Max Boundary)
    printf("[Test 5: Unsigned Integer %%u]\n");
    res1 = printf("STD: %u %u\n", 0, UINT_MAX);
    res2 = ft_printf("FT : %u %u\n", 0, UINT_MAX);
    printf("Return Values -> STD: %d | FT: %d\n\n", res1, res2);

    // Test 6: Hexadecimal Lowercase and Uppercase
    printf("[Test 6: Hexadecimal %%x and %%X]\n");
    res1 = printf("STD: %x %X %x\n", 0, 2551, 3735928559U); // 0xDEADBEEF
    res2 = ft_printf("FT : %x %X %x\n", 0, 2551, 3735928559U);
    printf("Return Values -> STD: %d | FT: %d\n\n", res1, res2);

    // Test 7: Pointer Hex Printing
    printf("[Test 7: Valid Pointer Address %%p]\n");
    res1 = printf("STD: %p\n", (void *)&test_var);
    res2 = ft_printf("FT : %p\n", (void *)&test_var);
    printf("Return Values -> STD: %d | FT: %d\n\n", res1, res2);

    // Test 8: NULL Pointer Handling
    printf("[Test 8: NULL Pointer %%p]\n");
    res1 = printf("STD: %p\n", (void *)NULL);
    res2 = ft_printf("FT : %p\n", (void *)NULL);
    printf("Return Values -> STD: %d | FT: %d\n\n", res1, res2);

    // Test 9: Percent Escaping
    printf("[Test 9: Percent Escaping %%%%]\n");
    res1 = printf("STD: %%%% %% 100%%\n");
    res2 = ft_printf("FT : %%%% %% 100%%\n");
    printf("Return Values -> STD: %d | FT: %d\n\n", res1, res2);

    // Test 10: Mixed Format String
    printf("[Test 10: Mixed Specs in One String]\n");
    res1 = printf(      "STD: %c %s %p %d %u %x %X %%\n", '4', "two", (void *)&test_var, -42, 4294967295U, 255, 255);
    res2 = ft_printf(   "FT : %c %s %p %d %u %x %X %%\n", '4', "two", (void *)&test_var, -42, 4294967295U, 255, 255);
    printf("Return Values -> STD: %d | FT: %d\n\n", res1, res2);

    return 0;
}