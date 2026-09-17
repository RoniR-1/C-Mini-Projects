#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct s_phil_info {
    int     number_of_philosophers;
    int     time_to_die;
    int     time_to_eat;
    int     time_to_sleep;
    int     number_of_times_each_philosopher_must_eat;
} t_phil_info;

typedef struct s_phil {
    int*            forks;
    t_phil_info*    phil_info;
    pthread_mutex_t*mutex;
    pthread_t       id;
} t_phil;


void eat_spaghitti(t_phil_info*);
int parse_phil(t_phil_info* phil, int argc, char** argv);