#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

struct s_phil;

typedef struct s_phil_info {
    int     number_of_philosophers;
    int     time_to_die;
    int     time_to_eat;
    int     time_to_sleep;
    int     number_of_times_each_philosopher_must_eat;
    int     amount_phils_ate;
    int     stop_flag;
    pthread_mutex_t     print_mutex;
    long long           start_time;
    pthread_mutex_t*    stop_flag_mutex;
    struct s_phil**     phils_array;
    pthread_mutex_t*    phils_ate_mutex;
    long long*          last_meals_time;
    pthread_mutex_t*    last_meals_mutex;
} t_phil_info;

typedef struct s_phil {
    pthread_mutex_t*    forks;
    t_phil_info*        phil_info;
    int                 id;
    pthread_t           thread_id;
} t_phil;


void eat_spaghitti(t_phil_info*);
int parse_phil(t_phil_info* phil, int argc, char** argv);