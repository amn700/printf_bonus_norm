#ifndef FT_PRINTF_H
#define FT_PRINTF_H

#include <stdarg.h>
#include <unistd.h>
#include <string.h>

typedef struct s_flags {
	int left_align;
	int zero_pad;
	int width;
	int precision;
	int plus_sign;
	int space_sign;
	int alt_form;
}   t_flags;

int		ft_putchar(char c);
int		ft_putstr(char *str, int len);
void	ft_reverse_str(char *str, int len);

int		ft_printf(const char *format, ...);

void	parse_flags(const char **format, t_flags *flags);
void	initialize_flags(t_flags *flags);
void	parse_width(const char **format, t_flags *flags);
void	parse_precision(const char **format, t_flags *flags);

void	handle_conversion(va_list args, const char **format, t_flags *flags, int *chars_written);
void	character_conversion(va_list args, t_flags *flags, int *chars_written);
void	string_conversion(va_list args, t_flags *flags, int *chars_written);

int		ft_print_padding(char c, int n);

#endif