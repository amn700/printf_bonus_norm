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
int ft_print_number(unsigned long long n, int base, int is_signed, int is_negative, 
                    t_flags *flags, char conv)
{
    char    buffer[64];
    int     len;
    int     chars_written = 0;
    int     is_zero = (n == 0);

    // Convert number to string
    len = ft_convert_number(n, base, buffer, conv);

    // Handle precision 0 for zero value
    if (is_zero && flags->precision == 0)
        len = 0;

    // Calculate padding
    int precision_padding = (flags->precision > len) ? flags->precision - len : 0;
    
    // Calculate total length including prefix/sign
    int prefix_len = 0;
    if (flags->alt_form && !is_zero && (conv == 'x' || conv == 'X'))
        prefix_len = 2;
    if (is_signed && (is_negative || flags->plus_sign || flags->space_sign))
        prefix_len = 1;

    int total_len = len + precision_padding + prefix_len;
    int width_padding = (flags->width > total_len) ? flags->width - total_len : 0;

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

void	pointer_conversion(va_list args, t_flags *flags, int *chars_written)
{
	unsigned long ptr;

	ptr = (unsigned long)va_arg(args, void *);
	*chars_written += ft_putstr("0x", 2);
	*chars_written += ft_print_number(ptr, 16, 0, 0, flags, 'x');
}

void handle_conversion(va_list args, const char **format, t_flags *flags, int *chars_written)
{
    char    specifier = **format;
    long    num;

    if (specifier == 'd' || specifier == 'i')
    {
        num = va_arg(args, int);
        if (num < 0)
            *chars_written += ft_print_number(-(unsigned long long)num, 10, 1, 1, flags, specifier);
        else
            *chars_written += ft_print_number(num, 10, 1, 0, flags, specifier);
    }
    else if (specifier == 'u')
        *chars_written += ft_print_number(va_arg(args, unsigned int), 10, 0, 0, flags, specifier);
    else if (specifier == 'x' || specifier == 'X')
        *chars_written += ft_print_number(va_arg(args, unsigned int), 16, 0, 0, flags, specifier);
    else if (specifier == 'p')
        pointer_conversion(args, flags, chars_written);
    else if (specifier == 'c')
        character_conversion(args, flags, chars_written);
    else if (specifier == 's')
        string_conversion(args, flags, chars_written);
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
