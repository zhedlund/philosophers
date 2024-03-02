NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror
LFLAGS = -pthread

SRC =	main.c \
		input_check.c \
		init.c \
		utils.c \
		monitor.c \
		threads.c \

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(LFLAGS) -o $(NAME)
	
.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
		rm -rf *.o

fclean:	clean
		rm -f $(NAME)

re:	fclean all

.PHONY: all, clean, fclean, re