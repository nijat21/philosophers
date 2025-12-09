.PHONY: all clean fclean re

NAME=philo
CC=cc
CFLAGS= -g -Wall -Wextra -Werror

SRCS:= philo.c dine.c utils.c parser.c setters_getters.c sync_utils.c \
		tracker.c write.c
OBJS:=$(SRCS:%.c=%.o)


all: $(NAME)

$(NAME): $(LIBFT_LIB) $(MLX_LIB) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all