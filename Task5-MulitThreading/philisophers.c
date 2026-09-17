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

int eat() {

}
int think() {

}
int sleep() {
}

void phil_life(void* arg) {
    t_phil* phil = (t_phil*) arg;
    struct timeval time1;
    struct timeval time2;
    if (gettimeofday(&time1, NULL) != 0) {free(phil); return;}
    while (1) {
        if (gettimeofday(&time2, NULL) != 0) break;
        //eating
        if (eat() != 0) break; // eating
        //sleeping
        if (gettimeofday(&time2, NULL) != 0) break;
        printf("%ld.%06ld Professor %d is sleeping\n", time2.tv_sec, time2.tv_usec, phil->id);
        usleep(phil->phil_info->time_to_sleep);
        //thinking
        if (gettimeofday(&time2, NULL) != 0) break;
        printf("%ld.%06ld Professor %d is sleeping\n", time2.tv_sec, time2.tv_usec, phil->id);
    }



    free(phil);
}


void eat_spaghitti(t_phil_info* phils) {
    // phils are numbered from 1 to N
    // phil n sits next to n-1 and n+1 and phil 1 sits next to 2 and N
    // #forks = #phil, to eat, phil needs 2 forks
    int N = phils->number_of_philosophers;
    pthread_mutex_t mutex_key = PTHREAD_MUTEX_INITIALIZER; // lock
    //pthread_t* threads = calloc(N, sizeof(pthread_t));
    int* forks = calloc(N, sizeof(int)); // if return --, take if == 0 and ++, else wait
    
    for (int i = 0; i < N; i++) {
        t_phil *phil = malloc(sizeof(t_phil));
        if (phil == NULL) {
            printf("Malloc failed, exiting\n");
            pthread_mutex_destroy(&mutex_key);
            break;
        }

        phil->phil_info = phils;
        phil->mutex = &mutex_key;
        phil->forks = forks;

        if (pthread_create(&phil->id, NULL, phil_life, phil) != 0) {
            printf("Failed thread creation, exiting\n");
            pthread_mutex_destroy(&mutex_key);
            free(phil);
            break;
        }
    }



    pthread_mutex_destroy(&mutex_key);
    free(forks); //free(phil);
}