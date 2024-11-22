#include "ft_printf.h"

// Convert a number to a string (supports bases up to 16)
int	ft_convert_number(unsigned long long n, int base, char *buffer, char conv)
{
	char *digits = (conv == 'X') ? "0123456789ABCDEF" : "0123456789abcdef";
	int len = 0;

	if (n == 0)
		buffer[len++] = '0';
	while (n > 0) {
		buffer[len++] = digits[n % base];
		n /= base;
	}
	ft_reverse_str(buffer, len);
	return (len);
}
// Print signed numbers (handles d and i conversions)
int	ft_print_signed_number(int n, t_flags *flags) {
	char	buffer[64];
	int		is_negative = (n < 0);
	int		len;
	int		chars_written = 0;

	// Convert number to string (using absolute value)
	len = ft_convert_number(is_negative ? -(long)n : n, 10, buffer, 'd');

	// Adjust length for precision
	int precision_padding = (flags->precision > len) ? flags->precision - len : 0;
	int total_len = len + precision_padding;

	// Include space for sign (+, -, or ' ')
	if (is_negative || flags->plus_sign || flags->space_sign)
		total_len++;

	// Adjust for width
	int width_padding = (flags->width > total_len) ? flags->width - total_len : 0;

	// Right-aligned padding (spaces)
	if (!flags->left_align && width_padding > 0)
		chars_written += ft_print_padding((flags->zero_pad && flags->precision == -1) ? '0' : ' ', width_padding);

	// Print sign
	if (is_negative)
		chars_written += ft_putchar('-');
	else if (flags->plus_sign)
		chars_written += ft_putchar('+');
	else if (flags->space_sign)
		chars_written += ft_putchar(' ');

	// Precision padding (zeros)
	chars_written += ft_print_padding('0', precision_padding);

	// Print number
	chars_written += ft_putstr(buffer, len);

	// Left-aligned padding (spaces)
	if (flags->left_align && width_padding > 0)
		chars_written += ft_print_padding(' ', width_padding);

	return (chars_written);
}

// Handler for d and i conversions
void	signed_conversion(va_list args, t_flags *flags, int *chars_written) {
	int n = va_arg(args, int);
	*chars_written += ft_print_signed_number(n, flags);
}

// Print numeric conversions (handles flags, padding, precision, etc.)
int	ft_print_number(unsigned long long n, int base, int is_signed, int is_negative, t_flags *flags, char conv) {
	char	buffer[64];
	int		len = ft_convert_number(n, base, buffer, conv);
	int		is_zero = (n == 0);
	int		chars_written = 0;

	// Adjust length for precision
	int precision_padding = (flags->precision > len) ? flags->precision - len : 0;
	int total_len = len + precision_padding;

	// Add space for prefix and sign
	if (flags->alt_form && !is_zero && (conv == 'x' || conv == 'X'))
		total_len += 2;
	if (is_signed && (is_negative || flags->plus_sign || flags->space_sign))
		total_len++;

	// Adjust for width
	int width_padding = (flags->width > total_len) ? flags->width - total_len : 0;

	// Right-aligned padding
	if (!flags->left_align && width_padding > 0)
		chars_written += ft_print_padding((flags->zero_pad && flags->precision == -1) ? '0' : ' ', width_padding);

	// Print sign
	if (is_signed) {
		if (is_negative)
			chars_written += ft_putchar('-');
		else if (flags->plus_sign)
			chars_written += ft_putchar('+');
		else if (flags->space_sign)
			chars_written += ft_putchar(' ');
	}

	// Print prefix for hexadecimal
	if (flags->alt_form && !is_zero && (conv == 'x' || conv == 'X'))
		chars_written += ft_putstr((conv == 'x') ? "0x" : "0X", 2);

	// Precision padding
	chars_written += ft_print_padding('0', precision_padding);

	// Print number
	chars_written += ft_putstr(buffer, len);

	// Left-aligned padding
	if (flags->left_align && width_padding > 0)
		chars_written += ft_print_padding(' ', width_padding);

	return (chars_written);
}

void	pointer_conversion(va_list args, t_flags *flags, int *chars_written)
{
	unsigned long ptr;

	ptr = (unsigned long)va_arg(args, void *);
	*chars_written += ft_putstr("0x", 2);
	*chars_written += ft_print_number(ptr, 16, 0, 0, flags, 'x');
}

void	handle_conversion(va_list args, const char **format, t_flags *flags, int *chars_written)
{
	char	specifier = **format;
	int		n;

	if (specifier == 'c')
		character_conversion(args, flags, chars_written);
	else if (specifier == 's')
		string_conversion(args, flags, chars_written);
	else if (specifier == 'd' || specifier == 'i')
		signed_conversion(args, flags, chars_written);
	else if (specifier == 'u')
		*chars_written += ft_print_number(va_arg(args, unsigned int), 10, 0, 0, flags, specifier);
	else if (specifier == 'x' || specifier == 'X')
		*chars_written += ft_print_number(va_arg(args, unsigned int), 16, 0, 0, flags, specifier);
	else if (specifier == 'p')
		pointer_conversion(args, flags, chars_written);
	else if (specifier == '%')
		*chars_written += ft_putchar('%');
}

int	ft_printf(const char *format, ...)
{
	va_list args;
	t_flags flags;
	int chars_written;

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
