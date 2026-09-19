#include "./mutli_threading.h"
/*
    Test cases;
    ./phils 1 800 200 200
    ./phils 5 800 200 200
    ./phils 5 800 200 200
    ./phils 4 310 200 100
    ./phils 5 800 200 200 7
    ./phils 200 1000 200 200
    ./phils 200 1000 200 200 10
*/
int main(int argc, char** argv) {
    t_phil_info phil;
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