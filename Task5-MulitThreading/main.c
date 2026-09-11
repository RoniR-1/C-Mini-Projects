#include "./mutli_threading.h"

int main(int argc, char** argv) {
    t_phil phil;
    if (argc < 5) {
        printf("Too few arguments given.\n");
    } 
    else if (parse_phil(&phil, argc, argv) == 0) {
        eat_spaghitti(&phil);
    } else {
        printf("Error occured whilst parsing arguements\n");
    }
    return 0;
}