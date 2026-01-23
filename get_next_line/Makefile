CC = cc
CFLAGS = -Wall -Wextra -Werror
#SIZE = -D BUFFER_SIZE=
AR = ar rcs
NAME = libgnl.a

SRCS = get_next_line.c get_next_line_utils.c
BONUS_SRCS = get_next_line_bonus.c get_next_line_utils_bonus.c
OBJS = $(SRCS:.c=.o)
BONUS_OBJS = $(BONUS_SRCS:.c=.o)

all: $(OBJS)
	@$(AR) $(NAME) $(OBJS)

bonus: $(BONUS_OBJS)
	@$(AR) $(NAME) $(BONUS_OBJS)

%.o: %.c
	@$(CC) $(CFLAGS) $(SIZE) -c $< -o $@

clean:
	@rm -f *.o

fclean: clean
	@rm -f $(NAME)

re: fclean all
