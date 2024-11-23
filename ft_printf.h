/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohchaib <mohchaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 23:26:47 by mohchaib          #+#    #+#             */
/*   Updated: 2024/11/23 09:49:59 by mohchaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <string.h>
# include <unistd.h>

typedef struct s_flags
{
	int	left_align;//number aka width but on the left
	int	zero_pad;// zeros only if width is specified
	int	width; //number of characters to print
	int	precision;//number of characters to print
	int	plus_sign; // '+' sign
	int	space_sign;	// ' ' sign
	int	alt_form; // '#' sign
}		t_flags;

int		ft_putchar(char c);
int		ft_putstr(char *str, int len);
void	ft_reverse_str(char *str, int len);
int		ft_printf(const char *format, ...);
void	parse_flags(const char **format, t_flags *flags);
void	initialize_flags(t_flags *flags);
void	parse_width(const char **format, t_flags *flags);
void	parse_precision(const char **format, t_flags *flags);
void	handle_conversion(va_list args, const char **format, t_flags *flags,
			int *chars_written);
void	character_conversion(va_list args, t_flags *flags, int *chars_written);
void	string_conversion(va_list args, t_flags *flags, int *chars_written);
int		ft_print_padding(char c, int n);

int	ft_convert_number(unsigned long long n, int base, char *buffer, char conv);
void	percent_conversion(t_flags *flags, int *chars_written);
int	ft_print_number(unsigned long long n, int base, int is_signed,
		int is_negative, t_flags *flags, char conv);
void	pointer_conversion(va_list args, t_flags *flags, int *chars_written);

#endif