/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohchaib <mohchaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 16:58:08 by mohchaib          #+#    #+#             */
/*   Updated: 2024/12/22 05:15:12 by mohchaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

int	ft_printf(const char *format, ...)
{
	va_list	args;
	t_flags	flags;
	int		chars_written;

	chars_written = 0;
	va_start(args, format);
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			parse_flags(&format, &flags);
			handle_conversion(args, &format, &flags, &chars_written);
			format++;
		}
		else
			chars_written += ft_putchar(*format++);
	}
	va_end(args);
	return (chars_written);
}
