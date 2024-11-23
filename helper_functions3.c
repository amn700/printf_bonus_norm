#include "ft_printf.h"

int	ft_print_number(unsigned long long n, int base, int is_signed,
		int is_negative, t_flags *flags, char conv)
{
	char	buffer[10];
	int		len;
	int		chars_written;
	int		is_zero;
	int		precision_padding;
	int		prefix_len;
	int		total_len;
	int		width_padding;

	chars_written = 0;
	is_zero = (n == 0);
	len = ft_convert_number(n, base, buffer, conv);
	if (is_zero && flags->precision == 0)
		len = 0;
	if (flags->precision > len)
		precision_padding = flags->precision - len;
	else
		precision_padding = 0;
	prefix_len = 0;
	if (flags->alt_form && !is_zero && (conv == 'x' || conv == 'X'))
		prefix_len = 2;
	if (is_signed && (is_negative || flags->plus_sign || flags->space_sign))
		prefix_len = 1;
	total_len = len + precision_padding + prefix_len;
	if (flags->width > total_len)
		width_padding = flags->width - total_len;
	else
		width_padding = 0;
	if (!flags->left_align && (!flags->zero_pad || flags->precision != -1))
		while (width_padding-- > 0)
			chars_written += ft_putchar(' ');
	if (is_signed)
	{
		if (is_negative)
			chars_written += ft_putchar('-');
		else if (flags->plus_sign)
			chars_written += ft_putchar('+');
		else if (flags->space_sign)
			chars_written += ft_putchar(' ');
	}
	if (flags->alt_form && !is_zero && (conv == 'x' || conv == 'X'))
		chars_written += ft_putchar('0') + ft_putchar(conv);
	if (!flags->left_align && flags->zero_pad && flags->precision == -1)
		while (width_padding-- > 0)
			chars_written += ft_putchar('0');
	while (precision_padding-- > 0)
		chars_written += ft_putchar('0');
	if (len > 0)
		chars_written += ft_putstr(buffer, len);
	if (flags->left_align)
		while (width_padding-- > 0)
			chars_written += ft_putchar(' ');
	return (chars_written);
}

void	pointer_conversion(va_list args, t_flags *flags, int *chars_written)
{
	void	*ptr;
	char	buffer[18];
	int		len;
	int		width_padding;
	int		total_len;

	ptr = va_arg(args, void *);
	if (ptr == NULL)
	{
		len = 3;
		if (flags->width > len)
			width_padding = flags->width - len;
		else
			width_padding = 0;
		if (!flags->left_align)
			*chars_written += ft_print_padding(' ', width_padding);
		*chars_written += ft_putstr("0x0", len);
		if (flags->left_align)
			*chars_written += ft_print_padding(' ', width_padding);
		return ;
	}
	len = ft_convert_number((unsigned long)ptr, 16, buffer, 'x');
	total_len = len + 2;
	if (flags->width > total_len)
		width_padding = flags->width - total_len;
	else
		width_padding = 0;
	if (!flags->left_align)
		*chars_written += ft_print_padding(' ', width_padding);
	*chars_written += ft_putstr("0x", 2);
	*chars_written += ft_putstr(buffer, len);
	if (flags->left_align)
		*chars_written += ft_print_padding(' ', width_padding);
}
