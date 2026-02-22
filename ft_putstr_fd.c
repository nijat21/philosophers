#include "philo.h"

static size_t ft_strlen(char *str)
{
    size_t len;

    len = -1;
    while (str[++len])
        ;
    return len;
}

void ft_putstr_fd(char *s, int fd)
{
    if (!s || fd < 0)
        return;
    write(fd, s, ft_strlen(s));
}