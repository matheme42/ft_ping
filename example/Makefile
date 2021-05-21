# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jdaufin <jdaufin@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/10/15 18:18:37 by jdaufin           #+#    #+#              #
#    Updated: 2021/04/30 10:26:03 by jdaufin          ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

.PHONY: demo re clean bclean fclean authorize

CC = clang
CFLAGS = -Wall -Wextra -Werror

NAME = ft_ping

SRCDIR = srcs/
OBJDIR = builds/
HDRDIR = includes/
LIBDIR = libft/
LIBHDRDIR = $(addsuffix includes/, $(LIBDIR))

FILES = ft_ping parse_args set_options show_help show_errors parse_address \
		resolve_address dynarray_init dynarray_add dynarray_free \
		dynarray_getbuflen handle_cycle send_echo handle_reply \
		handle_reply_utils exit_ping exit_ping_utils send_echo_reply \
		icmp_checksum wait_cooldown translate_icmp_codes
SRC = $(addprefix $(SRCDIR), $(addsuffix .c, $(FILES)))
OBJ = $(addprefix $(OBJDIR), $(addsuffix .o, $(FILES)))
HDR = $(addprefix $(HDRDIR), ft_ping.h)
LIBHDR = $(addprefix $(LIBHDRDIR), libft.h)
LIB = $(addprefix $(LIBDIR), libft.a)

$(NAME) :$(OBJ) $(LIB)
		$(CC) $(CFLAGS) -o $@ $^ -I $(HDRDIR) -L $(LIBDIR) -lft

$(LIB) :
		make -C $(LIBDIR)

$(OBJDIR)%.o : $(SRCDIR)%.c $(HDR) $(LIBHDR)
		[ -d $(OBJDIR) ] || mkdir $(OBJDIR)
		$(CC) $(CFLAGS) -o $@ -c $< -I $(HDRDIR) -I $(LIBHDRDIR)

bclean : 
		rm -rf $(OBJDIR)

clean : bclean
		make -C $(LIBDIR) clean

fclean : bclean
		make -C $(LIBDIR) fclean
		rm -rf $(NAME)

re : fclean $(NAME)

demo : authorize
		@echo "\n\e[32mBasic usage\e[0m : $(NAME) -c 4 8.8.8.8" && ./$(NAME) -c 4 8.8.8.8
		@echo "\n\e[31mInsufficient TTL\e[0m : $(NAME) -c 5 google.com -t 3" && ./$(NAME) -c 5 google.com -t 3
		@echo "\n\e[31mInsufficient TTL (verbose mode)\e[0m : $(NAME) -c 5 google.com -t 3 -v" && ./$(NAME) -c 5 google.com -t 3 -v

authorize : $(NAME)
		@sudo setcap cap_net_raw=pe $(NAME)
		@getcap $(NAME)
