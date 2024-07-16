NAME = minishell
CC = cc
# CFLAGS = -Wall -Wextra -Werror
CFLAGS = -fsanitize=address

LIBFT_SRC = $(addprefix ./libs/libft/, ft_atoi.c ft_lstadd_back_bonus.c ft_memchr.c ft_putendl_fd.c ft_striteri.c ft_strtrim.c \
ft_bzero.c ft_lstadd_front_bonus.c ft_memcmp.c ft_puthex.c ft_strjoin.c ft_substr.c \
ft_calloc.c ft_lstclear_bonus.c ft_memcpy.c ft_putnbr.c ft_strlcat.c ft_tolower.c \
ft_isalnum.c ft_lstdelone_bonus.c ft_memmove.c ft_putnbr_fd.c ft_strlcpy.c ft_toupper.c \
ft_isalpha.c ft_lstiter_bonus.c ft_memset.c ft_putstr.c ft_strlen.c get_next_line.c \
ft_isascii.c ft_lstlast_bonus.c ft_printf.c ft_putstr_fd.c ft_strmapi.c ft_isdigit.c \
ft_lstmap_bonus.c ft_putadress.c ft_split.c ft_strncmp.c ft_isprint.c ft_lstnew_bonus.c \
ft_putchar.c ft_strchr.c ft_strnstr.c ft_itoa.c ft_lstsize_bonus.c ft_putchar_fd.c \
ft_strdup.c ft_strrchr.c ft_base16.c get_next_line_utils.c)

SRCS = minishell.c Justice.c  shell_env.c type_check.c \
	$(addprefix ./built-ins/, export.c export_fts.c env.c unset.c exit.c pwd.c echo.c cd.c builtins_utils.c) $(LIBFT_SRC) \
	$(addprefix ./AST/, ast_utils.c ast_parser.c ast_builders.c) \
	$(addprefix ./Tokenizer/, token_creation.c token_utils.c tokenizer.c) \
	$(addprefix ./execution/, execution.c open_files.c execute_pipeline.c execute_subshell.c executors.c) \
	$(addprefix ./expander/, expander.c wildcard.c expand_arg.c expand_fts.c  expand_fts2.c expand_heredoc.c) \
	$(addprefix ./syntax_error/, syntax_errors.c syntax_utils.c read_heredoc.c ) \
	$(addprefix ./common/, helpers.c)


OBJS = $(SRCS:.c=.o)

.c.o :
	@$(CC) $(CFLAGS) -c $< -o $@

all : $(NAME)

$(NAME) : $(OBJS)
# stty -echoctl
	@$(CC) $(CFLAGS)  $^ -o $@ -lreadline

clean :
	@rm -f $(OBJS)

fclean : clean
	@rm -f $(NAME)

re : fclean all

.PHONY : all clean fclean
