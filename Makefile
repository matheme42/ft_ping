# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maxence <maxence@student.42lyon.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/11/24 13:54:31 by matheme           #+#    #+#              #
#    Updated: 2021/05/28 14:33:01 by maxence          ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

# nom de l'executable
NAME = ft_ping

#sources path
SRC_PATH		= srcs

#objects path
OBJ_PATH		= obj

#includes
INC_PATH		= includes
HEADER			= $(INC_PATH)/error.h $(INC_PATH)/ft_ping.h

# sources
NAME_SRC		=	main.c dnslookup.c send.c receive.c error.c socket.c statistic.c ft_bzero.c

# objects
NAME_OBJ		= $(addprefix $(OBJ_PATH)/,$(NAME_SRC:.c=.o))

#compilateur + flags
CC			=	gcc #-g3 -fsanitize=address -g3
CFLAGS		=  -Wall -Wextra -Werror

#librairies
# LIBFT				= libft
# LIBFT_INC			= libft/includes
# LIBFT.A			= $(LIBFT)/libft.a

all : $(NAME)

$(NAME) : $(NAME_OBJ)
	@$(CC) $^ -o $@ -lm && printf "\n\n${B}[EXECUTABLE \"$(NAME)\" COMPILED]${N}\n\n"


$(OBJ_PATH)/%.o : $(SRC_PATH)/%.c $(HEADER)
	@mkdir $(OBJ_PATH) 2> /dev/null || true
	@$(CC) -I $(INC_PATH) -c $< -o $@ -lm
	@printf "\033[2K\r${G} >>Compiling ${N}$<\033[36m \033[0m"

clean:
	@rm -f $(NAME_OBJ)
	@rmdir $(OBJ_PATH) 2> /dev/null || true
	@printf "${R}[CLEANING $(NAME_OBJ) BINARIES]${N}\n\n"

fclean: clean
	@rm -f $(NAME)
	@printf "${R}[REMOVING $(NAME) BINARIES]${N}\n\n"

re: fclean all

authorize : $(NAME)
		@sudo setcap cap_net_raw=pe $(NAME)
		@getcap $(NAME)