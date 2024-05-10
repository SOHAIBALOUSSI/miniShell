CFLAGS =	-Wall -Wextra -Werror

NAME =	libft.a

SRC =	ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c \
		ft_isascii.c ft_isdigit.c ft_isprint.c ft_memchr.c ft_memcmp.c \
		ft_memcpy.c ft_memmove.c ft_memset.c ft_strchr.c ft_strdup.c \
		ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strjoin.c ft_strncmp.c \
		ft_strnstr.c ft_strrchr.c ft_substr.c ft_striteri.c ft_tolower.c \
		ft_toupper.c ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
		ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c get_next_line.c \
		ft_printf.c ft_puthex.c ft_putchar.c ft_putnbr.c ft_putstr.c ft_putadress.c \
		ft_base16.c get_next_line_utils.c


BSRC =	ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c \
		ft_lstlast_bonus.c ft_lstadd_back_bonus.c ft_lstdelone_bonus.c ft_lstclear_bonus.c \
		ft_lstiter_bonus.c ft_lstmap_bonus.c

OBJ =	$(SRC:.c=.o)

BOBJ =	$(BSRC:.c=.o)

all:	$(NAME)

.c.o:
		cc $(CFLAGS) -c $< -o $@
		ar rc $(NAME) $@

$(NAME):	$(OBJ)
		
bonus:	$(BOBJ)

clean:
		rm -f $(OBJ) $(BOBJ)

fclean: clean
		rm -f $(NAME)

re: fclean all