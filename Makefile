# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/17 17:13:13 by msaadidi          #+#    #+#              #
#    Updated: 2024/07/17 17:13:14 by msaadidi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -fsanitize=address

SRCS = minishell.c \
	$(addprefix ./built-ins/, export.c export_fts.c env.c unset.c exit.c pwd.c echo.c cd.c builtins_utils.c) \
	$(addprefix ./AST/, ast_utils.c ast_parser.c ast_builders.c) \
	$(addprefix ./Tokenizer/, token_creation.c token_utils.c tokenizer.c) \
	$(addprefix ./execution/, execute_pipeline.c execute_command.c execution_errors.c execution_helpers.c execution_helpers2.c executors.c open_files.c) \
	$(addprefix ./expander/, expander.c expand_arg.c expand_fts.c  expand_fts2.c expand_heredoc.c wildcard.c wildcard_redir.c wildcard_utils.c) \
	$(addprefix ./syntax_error/, syntax_errors.c syntax_utils.c read_heredoc.c ) \
	$(addprefix ./common/, gc.c  env_init.c  env_utils.c type_check.c helpers.c)

OBJS = $(SRCS:.c=.o)

.c.o :
	@$(CC) $(CFLAGS) -c $< -o $@ 

all : $(NAME) 

$(NAME) : $(OBJS)
	@make -C ./libft --silent
	@$(CC) $(CFLAGS)  $^ -o $@ -lreadline ./libft/libft.a

clean :
	@rm -f $(OBJS)
	@make clean -C ./libft --silent

fclean : clean
	@rm -f $(NAME)
	@make fclean -C ./libft --silent

re : fclean all

.PHONY : all clean fclean
