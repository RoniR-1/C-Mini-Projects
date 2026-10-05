#include "shell.h"

/*
    Note for myself:
    -Standardize return values meaning, weather -1, 0 or 1 is true, false or error
    -Any I/O operation needs to be checked if successfull
*/


int main(void) {
    int success = loop_shell();
    return success;
}

/*
testing:    
./MingShell < inputs.txt > actual_outputs.txt 2>&1
diff -u expected_outputs.txt actual_outputs.txt
*/