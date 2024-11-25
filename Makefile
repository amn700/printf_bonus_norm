CC = cc
CFLAGS = -Wall -Wextra -Werror

SRCS = ft_printf.c helper_functions0.c helper_functions1.c helper_functions2.c helper_functions3.c helper_functions4.c parse_flags.c conversion_handler.c
SRCS_BONUS = ft_printf_bonus.c helper_functions0_bonus.c helper_functions1_bonus.c helper_functions2_bonus.c helper_functions3_bonus.c helper_functions4_bonus.c parse_flags_bonus.c conversion_handler_bonus.c

OBJS = $(SRCS:.c=.o)
OBJS_BONUS = $(SRCS_BONUS:.c=.o)

NAME = libftprintf.a

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

bonus: $(OBJS_BONUS)
	ar rcs $(NAME) $(OBJS_BONUS)

%.o: %.c ft_printf.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS) $(OBJS_BONUS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: clean