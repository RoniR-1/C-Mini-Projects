#include <unistd.h>
#include <stdlib.h>
#include "libft.h"

void ft_putnbr_fd(int n, int fd) {
    char *nString = ft_itoa(n);
    if (nString == 0) {return;}
    write(fd, nString, ft_strlen(nString));
    free(nString);
}
