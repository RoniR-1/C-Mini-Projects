#include <stdarg.h>
#include <stdio.h>

typedef struct s_phil {
    int     number_of_philosophers;
    int     time_to_die;
    int     time_to_eat;
    int     time_to_sleep;
    int     number_of_times_each_philosopher_must_eat;
} t_phil;

void eat_spaghitti(t_phil*);
int parse_phil(t_phil* phil, int argc, char** argv);