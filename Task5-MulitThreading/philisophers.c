#include "./mutli_threading.h"

int parse_string_to_int(int argc, char** argv) {
    char* s = argv[argc];
    int result = 0;
    int counter = 0;
    int isNegative = -1; // dont think i need this
    while (s[counter]) {
        if (s[counter] == '-') isNegative = 1;
        else if (s[counter] < '0' || s[counter] > '9') {
            printf("Error: argument has non integer in it");
            return -1;
        }
        else result = result * 10 + s[counter]-'0';
        counter++;
    }
    if (isNegative == 1) result *= -1;
    return result;
} 

int parse_phil(t_phil* phil, int argc, char** argv) {
    // prob can get away with a for loop but meh
    // TO DO: Should check weather they are all positive and return error
    phil->number_of_philosophers = parse_string_to_int(1, argv);
    phil->time_to_die = parse_string_to_int(2, argv);
    phil->time_to_eat = parse_string_to_int(3, argv);
    phil->time_to_sleep = parse_string_to_int(4, argv);
    phil->number_of_times_each_philosopher_must_eat = -1;
    if (argc >= 6) phil->number_of_times_each_philosopher_must_eat = parse_string_to_int(5, argv);
    return 0;
}


void eat_spaghitti(t_phil*) {
    // phils are numbered from 1 to N
    // phil n sits next to n-1 and n+1 and phil 1 sits next to 2 and N
}