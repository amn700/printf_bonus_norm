/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conversions_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohchaib <mohchaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 08:09:19 by mohchaib          #+#    #+#             */
/*   Updated: 2024/11/23 09:08:14 by mohchaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	handle_conversion(va_list args, const char **format, t_flags *flags,
	int *chars_written)
{
	long	num;

	if (**format == 'd' || **format == 'i')
	{
		num = va_arg(args, int);
		if (num < 0)
			*chars_written += ft_print_number(-num, 10, 1,
					1, flags, **format);
		else
			*chars_written += ft_print_number(num, 10, 1, 0, flags, **format);
	}
	else if (**format == 'u')
		*chars_written += ft_print_number(va_arg(args, unsigned int), 10, 0, 0,
				flags, **format);
	else if (**format == 'x' || **format == 'X')
		*chars_written += ft_print_number(va_arg(args, unsigned int), 16, 0, 0,
				flags, **format);
	else if (**format == 's')
		string_conversion(args, flags, chars_written);
	else if (**format == 'c')
		character_conversion(args, flags, chars_written);
	else if (**format == 'p')
		pointer_conversion(args, flags, chars_written);
	else if (**format == '%')
		percent_conversion(flags, chars_written);
}

int	ft_convert_number(unsigned long long n, int base, char *buffer, char conv)
{
	char	*digits;
	int		len;

	if (conv == 'x')
		digits = "0123456789abcdef";
	else
		digits = "0123456789ABCDEF";
	len = 0;
	if (n == 0)
		buffer[len++] = '0';
	while (n > 0)
	{
		buffer[len++] = digits[n % base];
		n /= base;
	}
	ft_reverse_str(buffer, len);
	return (len);
}

void	percent_conversion(t_flags *flags, int *chars_written)
{
	int	width_padding;
	if (flags->width > 1)
		width_padding = flags->width - 1;
	else
		width_padding = 0;
	if (!flags->left_align)
	{
		while (width_padding-- > 0)
		{
			ft_putchar(' ');
			(*chars_written)++;
		}
	}
	*chars_written += ft_putchar('%');
	if (flags->left_align)
	{
		while (width_padding-- > 0)
		{
			ft_putchar(' ');
			(*chars_written)++;
		}
	}
}
