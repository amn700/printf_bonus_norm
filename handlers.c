#include "ft_printf.h"

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

int	ft_print_number(unsigned long long n, int base, int is_signed,
		int is_negative, t_flags *flags, char conv)
{
	char	buffer[64];
	int		len;
	int		chars_written;
	int		is_zero;
	int		precision_padding;
	int		prefix_len;
	int		total_len;
	int		width_padding;

	chars_written = 0;
	is_zero = (n == 0);
	// Convert number to string
	len = ft_convert_number(n, base, buffer, conv);
	// Handle precision 0 for zero value
	if (is_zero && flags->precision == 0)
		len = 0;
	// Calculate padding
	precision_padding = (flags->precision > len) ? flags->precision - len : 0;
	// Calculate total length including prefix/sign
	prefix_len = 0;
	if (flags->alt_form && !is_zero && (conv == 'x' || conv == 'X'))
		prefix_len = 2;
	if (is_signed && (is_negative || flags->plus_sign || flags->space_sign))
		prefix_len = 1;
	total_len = len + precision_padding + prefix_len;
	width_padding = (flags->width > total_len) ? flags->width - total_len : 0;
	// Right-aligned space padding
	if (!flags->left_align && (!flags->zero_pad || flags->precision != -1))
	{
		while (width_padding-- > 0)
		{
			ft_putchar(' ');
			chars_written++;
		}
	}
	// Print sign/prefix
	if (is_signed)
	{
		if (is_negative)
			chars_written += ft_putchar('-');
		else if (flags->plus_sign)
			chars_written += ft_putchar('+');
		else if (flags->space_sign)
			chars_written += ft_putchar(' ');
	}
	// Print hex prefix
	if (flags->alt_form && !is_zero && (conv == 'x' || conv == 'X'))
	{
		ft_putchar('0');
		ft_putchar(conv);
		chars_written += 2;
	}
	// Right-aligned zero padding
	if (!flags->left_align && flags->zero_pad && flags->precision == -1)
	{
		while (width_padding-- > 0)
		{
			ft_putchar('0');
			chars_written++;
		}
	}
	// Precision padding
	while (precision_padding-- > 0)
	{
		ft_putchar('0');
		chars_written++;
	}
	// Print number (unless it's 0 with precision 0)
	if (len > 0)
		chars_written += ft_putstr(buffer, len);
	// Left-aligned padding
	if (flags->left_align)
	{
		while (width_padding-- > 0)
		{
			ft_putchar(' ');
			chars_written++;
		}
	}
	return (chars_written);
}

void	percent_conversion(t_flags *flags, int *chars_written)
{
	int	width_padding;

	width_padding = (flags->width > 1) ? flags->width - 1 : 0;
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

void	pointer_conversion(va_list args, t_flags *flags, int *chars_written)
{
	void	*ptr;
	char	buffer[64];
	int		len;
	int		width_padding;

	ptr = va_arg(args, void *);
	if (ptr == NULL)
	{
		len = 3;
		width_padding = (flags->width > len) ? flags->width - len : 0;
		if (!flags->left_align)
			*chars_written += ft_print_padding(' ', width_padding);
		*chars_written += ft_putstr("0x0", len);
		if (flags->left_align)
			*chars_written += ft_print_padding(' ', width_padding);
		return ;
	}
	len = ft_convert_number((unsigned long)ptr, 16, buffer, 'x');
	int total_len = len + 2;
	width_padding = (flags->width > total_len) ? flags->width - total_len : 0;
	if (!flags->left_align)
		*chars_written += ft_print_padding(' ', width_padding);
	*chars_written += ft_putstr("0x", 2);
	*chars_written += ft_putstr(buffer, len);
	if (flags->left_align)
		*chars_written += ft_print_padding(' ', width_padding);
}

void	handle_conversion(va_list args, const char **format, t_flags *flags,
		int *chars_written)
{
	char	specifier;
	long	num;

	specifier = **format;
	if (specifier == 'd' || specifier == 'i')
	{
		num = va_arg(args, int);
		if (num < 0)
			*chars_written += ft_print_number(-(unsigned long long)num, 10, 1,
					1, flags, specifier);
		else
			*chars_written += ft_print_number(num, 10, 1, 0, flags, specifier);
	}
	else if (specifier == 'u')
		*chars_written += ft_print_number(va_arg(args, unsigned int), 10, 0, 0,
				flags, specifier);
	else if (specifier == 'x' || specifier == 'X')
		*chars_written += ft_print_number(va_arg(args, unsigned int), 16, 0, 0,
				flags, specifier);
	else if (specifier == 's')
		string_conversion(args, flags, chars_written);
	else if (specifier == 'c')
		character_conversion(args, flags, chars_written);
	else if (specifier == 'p')
		pointer_conversion(args, flags, chars_written);
	else if (specifier == '%')
		percent_conversion(flags, chars_written);
}
