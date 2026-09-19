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


int is_simulation_running(t_phil_info* phils_info) {
    pthread_mutex_lock(phils_info->stop_flag_mutex);
    int running = phils_info->stop_flag;
    pthread_mutex_unlock(phils_info->stop_flag_mutex);
    return running;
}
void stop_simulation(t_phil_info* phils_info) {
    pthread_mutex_lock(phils_info->stop_flag_mutex);
    phils_info->stop_flag = 0;
    pthread_mutex_unlock(phils_info->stop_flag_mutex);
}
void print_sync(t_phil_info* info, char* s, long long time, int id) {
    pthread_mutex_lock(&info->print_mutex);
    if (is_simulation_running(info)) printf(s, time, id);
    pthread_mutex_unlock(&info->print_mutex);
}
long long get_time_ms(t_phil_info* phils_info) {
    struct timeval time;
    if (gettimeofday(&time, NULL) != 0) return -1;
    return (((long long)time.tv_sec * 1000) + time.tv_usec/1000) - phils_info->start_time;
}


int parse_phil(t_phil_info* phil, int argc, char** argv) {
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

/*
    Resource hierachy implementation according to Dijkstra. Each phil must pick up
    the lower numbered fork first
*/
int eat(t_phil* phil) {
    long long time;
    // 1 phil case, annoying
    if (phil->phil_info->number_of_philosophers == 1) {
        pthread_mutex_lock(&phil->forks[0]);
        if (!is_simulation_running(phil->phil_info)) {
            pthread_mutex_unlock(&phil->forks[0]);
            return 0;
        }
        time = get_time_ms(phil->phil_info);
        print_sync(phil->phil_info, "%lld ms Professor %d has taken a fork\n", time, phil->id);
        
        while (is_simulation_running(phil->phil_info)) {
            usleep(1000);
        }
        pthread_mutex_unlock(&phil->forks[0]);
        return 0;
    }
    int left_fork = phil->id;
    int right_fork = (phil->id+1);
    if (right_fork >= phil->phil_info->number_of_philosophers) {
        right_fork = left_fork;
        left_fork = 0;
    }
    // fork 1
    pthread_mutex_lock(&phil->forks[left_fork]);
    if (!is_simulation_running(phil->phil_info)) {pthread_mutex_unlock(&phil->forks[left_fork]); return 0;}
    time = get_time_ms(phil->phil_info); if (time == -1) return -1;
    print_sync(phil->phil_info, "%lld ms Professor %d has taken a fork\n", time, phil->id);
    // fork 2
    pthread_mutex_lock(&phil->forks[right_fork]);
    if (!is_simulation_running(phil->phil_info)) {pthread_mutex_unlock(&phil->forks[right_fork]);pthread_mutex_unlock(&phil->forks[left_fork]); return 0;}
    time = get_time_ms(phil->phil_info); if (time == -1) return -1;
    print_sync(phil->phil_info, "%lld ms Professor %d has taken a fork\n", time, phil->id);

    // now eating
    time = get_time_ms(phil->phil_info); if (time == -1) return -1;
    pthread_mutex_lock(&phil->phil_info->last_meals_mutex[phil->id]);
    phil->phil_info->last_meals_time[phil->id] = time;
    pthread_mutex_unlock(&phil->phil_info->last_meals_mutex[phil->id]);
    print_sync(phil->phil_info, "%lld ms Professor %d is eating\n", time, phil->id);
    if (is_simulation_running(phil->phil_info)) usleep(phil->phil_info->time_to_eat * 1000);
    pthread_mutex_unlock(&phil->forks[left_fork]);
    pthread_mutex_unlock(&phil->forks[right_fork]);
    
    // increase ate amount
    pthread_mutex_lock(phil->phil_info->phils_ate_mutex);
    phil->phil_info->amount_phils_ate++;
    pthread_mutex_unlock(phil->phil_info->phils_ate_mutex);
    return 0;
}

void* phil_life(void* arg) {
    t_phil* phil = (t_phil*) arg;
    while (is_simulation_running(phil->phil_info) == 0) {
        usleep(100); // all phils now start together roughly
    }
    long long time = get_time_ms(phil->phil_info); if (time == -1) return NULL;
    
    if ((phil->id + 1) % 2 != 0) usleep(phil->phil_info->time_to_eat * 500);

    pthread_mutex_lock(&phil->phil_info->last_meals_mutex[phil->id]);
    phil->phil_info->last_meals_time[phil->id] = time;
    pthread_mutex_unlock(&phil->phil_info->last_meals_mutex[phil->id]);

    while (is_simulation_running(phil->phil_info)) {
        //eating
        if (eat(phil) != 0) break;
        if (!is_simulation_running(phil->phil_info)) break;
        //sleeping
        time = get_time_ms(phil->phil_info); if (time == -1) break;
        print_sync(phil->phil_info, "%lld ms Professor %d is sleeping\n", time, phil->id);

        usleep(phil->phil_info->time_to_sleep * 1000); // convert ms to micros
        if (!is_simulation_running(phil->phil_info)) break;
        //thinking
        time = get_time_ms(phil->phil_info); if (time == -1) break;
        print_sync(phil->phil_info, "%lld ms Professor %d is thinking\n", time, phil->id);
        usleep(500);
    }
    return NULL;
}

void* monitor_phils(void* arg) {
    t_phil_info* phils_info = (t_phil_info*)arg;
    long long current_time;
    while(is_simulation_running(phils_info) == 0) usleep(200);
    while (is_simulation_running(phils_info)) {
        for (int i = 0; i < phils_info->number_of_philosophers && is_simulation_running(phils_info); i++) {
            pthread_mutex_lock(&phils_info->last_meals_mutex[i]);
            current_time = get_time_ms(phils_info); if (current_time == -1) {stop_simulation(phils_info); break;}

            if (phils_info->last_meals_time[i] != -1 && current_time - phils_info->last_meals_time[i] >= phils_info->time_to_die) {
                print_sync(phils_info, "%lld ms Professor %d died\n", current_time, i);
                stop_simulation(phils_info);
            }
            pthread_mutex_unlock(&phils_info->last_meals_mutex[i]);
        }
        if (phils_info->number_of_times_each_philosopher_must_eat != -1) {
            pthread_mutex_lock(phils_info->phils_ate_mutex);
            if (phils_info->amount_phils_ate >= phils_info->number_of_times_each_philosopher_must_eat * phils_info->number_of_philosophers) {
                stop_simulation(phils_info);
            }
            pthread_mutex_unlock(phils_info->phils_ate_mutex);
        }
        usleep(2000);
    }
    return NULL;
}


void eat_spaghitti(t_phil_info* phils) {
    // phils are numbered from 1 to N
    // phil n sits next to n-1 and n+1 and phil 1 sits next to 2 and N
    // #forks = #phil, to eat, phil needs 2 forks
    long long time;
    int i;
    int N = phils->number_of_philosophers;
    //pthread_t* threads = calloc(N, sizeof(pthread_t));
    pthread_mutex_t stop_flag_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t phils_ate_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t* forks = malloc(N * sizeof(pthread_mutex_t)); // each fork a mutex key
    pthread_mutex_t* last_meals_mutex = malloc(N * sizeof(pthread_mutex_t)); // each phil has his own mutex for last meal
    t_phil** phils_array = malloc(N * sizeof(t_phil*)); // all phils
    long long* last_meals_time = malloc(N * sizeof(long long)); // time of last meal for each phil

    if (forks == NULL || last_meals_mutex == NULL || last_meals_time == NULL || phils_array == NULL) {printf("Malloc failed\n"); return;}
    phils->last_meals_mutex = last_meals_mutex;
    phils->last_meals_time = last_meals_time;
    phils->amount_phils_ate = 0;
    phils->phils_ate_mutex = &phils_ate_mutex;
    phils->stop_flag = 0;
    phils->phils_array = phils_array;
    phils->stop_flag_mutex = &stop_flag_mutex;
    phils->start_time = 0;
    phils->print_mutex = print_mutex;

    for (i = 0; i < N; i++) {
        int mutex1 = pthread_mutex_init(&forks[i], NULL);
        int mutex2 = pthread_mutex_init(&last_meals_mutex[i], NULL);

        t_phil *phil = malloc(sizeof(t_phil));
        phils_array[i] = phil;
        if (phil == NULL || mutex1 != 0 || mutex2 != 0) {
            printf("Malloc/mutexkey failed, exiting\n");
            phils->stop_flag = 0;
            break;
        }
        
        phil->phil_info = phils;
        phil->forks = forks;
        time = get_time_ms(phils); if (time == -1) break;
        last_meals_time[i] = -1;
        phil->id = i;
        
        if (pthread_create(&phil->thread_id, NULL, phil_life, phil) != 0) {
            printf("Failed thread creation, exiting\n");
            phils->stop_flag = 0;
            break;
        }
    }
    pthread_t monitor_thread_id;
    if (pthread_create(&monitor_thread_id, NULL, monitor_phils, phils) != 0) {
        printf("moniter thread failed\n");
    }
    time = get_time_ms(phils); if (time == -1) return;
    phils->start_time = time;
    phils->stop_flag = 1;
    while (is_simulation_running(phils) == 1) usleep(10000);
    for (int j = 0; j < i; j++) {
        pthread_join(phils_array[j]->thread_id, NULL);
    }
    pthread_join(monitor_thread_id, NULL);

    for (int j = 0; j < i; j++) {
        free(phils_array[j]);
        pthread_mutex_destroy(&forks[j]);
        pthread_mutex_destroy(&last_meals_mutex[j]);
    }
    pthread_mutex_destroy(&phils_ate_mutex);
    pthread_mutex_destroy(&stop_flag_mutex);
    pthread_mutex_destroy(&print_mutex);
    free(forks); free(last_meals_mutex); free(last_meals_time); free(phils_array);
}