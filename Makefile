# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/12/04 21:18:10 by lfabbro           #+#    #+#              #
#    Updated: 2017/12/21 20:58:58 by lfabbro          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

FT_NM		= ft_nm 
FT_OTOOL	= ft_otool

CC			= gcc
CFLAGS		= -Wall -Wextra -Werror

SRC_NAME	= ft_nm.c
OBJ_NAME	= $(SRC_FT_NM:.c=.o)
LIB_NAME	= -lft

SRC_PATH	= ./src/
OBJ_PATH	= ./obj/
LIB_PATH	= ./libft/ .
INC_PATH	= ./include ./libft/include ./libft/libft

SRC			= $(addprefix $(SRC_PATH),$(SRC_NAME))
OBJ			= $(SRC:%.c=%.o)
LIB 		= $(addprefix -L, $(LIB_PATH))
LIB			+= $(LIB_NAME)
INC 		= $(addprefix -I ,$(INC_PATH))

RED			= \033[0;31m
GREEN		= \033[0;32m
WHITE		= \033[1;37m
ENDC		= \033[0m

.PHONY: all clean fclean re lib libclean libfclean norme

all: lib $(FT_NM) $(FT_OTOOL)

$(FT_NM): $(OBJ)
	@printf "$(WHITE) Making $(FT_NM) $(ENDC)\n"
	$(CC) $(CFLAGS) $(INC) $(OBJ) -o $(FT_NM) $(LIB)
	@printf "$(WHITE) [ $(GREEN)OK $(WHITE)] $(FT_NM) $(ENDC)\n"

$(FT_OTOOL):
	@printf "$(WHITE) Making $(FT_OTOOL) $(ENDC)\n"
#	$(CC) $(CFLAGS) $(INC) $(FT_NM)
	@printf "$(WHITE) [ $(GREEN)OK $(WHITE)] $(FT_OTOOL) $(ENDC)\n"

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -o $@ -c $<

clean:
	@rm -vrf $(OBJ) $(OBJ_PATH) $(SRC_PATH)/*.o

fclean: clean libfclean
	@rm -fv $(FT_NM) $(FT_OTOOL)
	@printf "$(WHITE) [ $(RED)FCLEAN $(WHITE)]$(ENDC)\n"

lib:
	@make -C ./libft

libclean:
	@make -C ./libft clean

libfclean:
	@make -C ./libft fclean

re: fclean all

norme :
	@printf "$(WHITE) Norminette $(ENDC)\n"
	norminette **/*.[ch]
