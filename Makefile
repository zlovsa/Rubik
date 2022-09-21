# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mcarolee <mcarolee@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/11/07 19:13:40 by mcarolee          #+#    #+#              #
#    Updated: 2022/08/29 16:22:27 by mcarolee         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC_FILES	=	main.cpp Cube.cpp CubeSolver.cpp CubeSolverInit.cpp Utils.cpp

NAME	= rubik

CC		= g++
RM		= rm -f

CFLAGS	= -Wall -Wextra -Werror -O3

INCLUDES_DIR	= ./incl
SRCS_DIR		= ./srcs
OBJS_DIR		= ./objs

SRCS = $(addprefix $(SRCS_DIR)/, $(SRC_FILES))
OBJS = $(patsubst $(SRCS_DIR)/%.cpp,$(OBJS_DIR)/%.o, $(SRCS))
DEPS = $(OBJS:.o=.d)

all:		$(NAME)

-include $(DEPS)
$(OBJS_DIR)/%.o:	$(SRCS_DIR)/%.cpp | $(OBJS_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDES_DIR) -MMD -MP -c -o $@ $<

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

$(NAME):	$(OBJS)
			$(CC) $(CFLAGS) -I$(INCLUDES_DIR) -MMD -MP -o $(NAME) $(OBJS)

clean:
			if [ -d "$(OBJS_DIR)" ]; then rm -rfv $(OBJS_DIR); fi

fclean:		clean
			if [ -f "$(NAME)" ]; then rm -rfv $(NAME); fi
			
re:			fclean all

.PHONY:		all clean fclean re