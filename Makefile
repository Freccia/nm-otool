# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/01/04 15:18:12 by lfabbro           #+#    #+#              #
#    Updated: 2018/01/08 14:52:22 by lfabbro          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

FT_NM		= ft_nm
FT_OTOOL	= ft_otool

NAME		= $(FT_NM) $(FT_OTOOL)

SRC_NM		= ft_nm.c list.c handle32.c handle64.c handle-fat.c \
				tools.c swap.c sections.c
SRC_OT		= ft_otool.c

SRC_DIR		= src
OBJ_DIR		= obj
INC_DIR		= include libft/include libft/libft
LIB_DIR		= libft

CC			= clang
CFLAGS		= -Wall -Wextra -Werror -g
INC			= $(addprefix -I./,$(INC_DIR))
LIB			= $(addprefix -L./,$(LIB_DIR))
LIB			+= -lft

SRC_DIR_NM	= $(addprefix $(SRC_DIR)/,$(FT_NM))
SRC_DIR_OT	= $(addprefix $(SRC_DIR)/,$(FT_OTOOL))
OBJ_DIR_NM	= $(addprefix $(OBJ_DIR)/,$(FT_NM))
OBJ_DIR_OT	= $(addprefix $(OBJ_DIR)/,$(FT_OTOOL))

SRC_FILES_NM	= $(addprefix $(SRC_DIR_NM)/,$(SRC_NM))
SRC_FILES_OT	= $(addprefix $(SRC_DIR_OT)/,$(SRC_OT))
OBJ_FILES_NM	= $(patsubst $(SRC_DIR_NM)/%.c,$(OBJ_DIR_NM)/%.o,$(SRC_FILES_NM))
OBJ_FILES_OT	= $(patsubst $(SRC_DIR_OT)/%.c,$(OBJ_DIR_OT)/%.o,$(SRC_FILES_OT))

RED			= \033[0;31m
GREEN		= \033[0;32m
YELLOW		= \033[0;33m
WHITE		= \033[1;37m
ENDC		= \033[0m

.PHONY:	all directories clean fclean re lib libclean libre norme

all: directories $(FT_NM) $(FT_OTOOL)

$(FT_NM): $(OBJ_FILES_NM)
	$(CC) $(CFLAGS) $(INC) $^ -o $@ $(LIB)
	@printf "$(WHITE) [ $(GREEN)OK $(WHITE)] $(FT_NM) $(ENDC)\n"

$(FT_OTOOL): $(OBJ_FILES_OT)
	$(CC) $(CFLAGS) $(INC) $^ -o $@ $(LIB)
	@printf "$(WHITE) [ $(GREEN)OK $(WHITE)] $(FT_OTOOL) $(ENDC)\n"

$(OBJ_DIR_NM)/%.o: $(SRC_DIR_NM)/%.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(OBJ_DIR_OT)/%.o: $(SRC_DIR_OT)/%.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

directories:
	@mkdir -p $(OBJ_DIR) $(OBJ_DIR_NM) $(OBJ_DIR_OT)

clean:
	@printf "$(WHITE) [ $()CLEAN $(WHITE)]$(ENDC)\n"
	@rm -fvr $(OBJ_DIR)

fclean: clean
	@printf "$(WHITE) [ $(RED)FCLEAN $(WHITE)]$(ENDC)\n"
	@rm -fv $(FT_NM) $(FT_OTOOL)

re: fclean all

lib:
	@make -C $(LIB_DIR)

libclean:
	@make -C $(LIB_DIR) clean

libre:
	@make -C $(LIB_DIR) re

norme :
	@printf "$(WHITE) Norminette $(ENDC)\n"
	norminette $(SRC_DIR)/**/*.[ch] $(INC_DIR)/*.[ch] $(LIB_DIR)/**/*.[ch]
