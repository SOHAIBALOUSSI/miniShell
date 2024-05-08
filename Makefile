NAME = minishell
CC = cc 
CFLAGS = -Wall -Wextra -Werror -lreadline ./libft/libft.a

SRCS = minishell.c

OBJS = $(SRCS .c=.o)

.c.o :
	$(CC) $(CFLAGS) -c $< -o $@ 

all : $(NAME)

$(NAME) : $(OBJS)

clean :
	rm -rf $(OBJS)

fclean : clean
	rm -rf $(NAME)

.PHONY : all clean fclean