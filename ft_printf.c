#include "ft_printf.h"

int ft_putchar(char c)
{
    return (write(1, &c, 1));
}

int ft_putstr(char *str, int len)
{
    return (write(1, str, len));
}

void ft_reverse_str(char *str, int len)
{
    int i;
    int j;
    char tmp;

    i = 0;
    j = len - 1;
    while (i < j)
    {
        tmp = str[i];
        str[i] = str[j];
        str[j] = tmp;
        i++;
        j--;
    }
}

int ft_number_len(unsigned long long n, int base)
{
    int len = 0;
    if (n == 0) return 1;
    while (n)
    {
        n /= base;
        len++;
    }
    return len;
}

int ft_print_hex_prefix(t_flags *flags, char conv, int is_zero)
{
    int chars_written;

    chars_written = 0;
    if (flags->alt_form && !is_zero && (conv == 'x' || conv == 'X'))
    {
        ft_putchar('0');
        ft_putchar(conv);
        chars_written = 2;
    }
    return (chars_written);
}

int ft_print_sign(int is_signed, int is_negative, t_flags *flags)
{
    int chars_written;

    chars_written = 0;
    if (is_signed)
    {
        if (is_negative)
        {
            ft_putchar('-');
            chars_written++;
        }
        else if (flags->plus_sign)
        {
            ft_putchar('+');
            chars_written++;
        }
        else if (flags->space_sign)
        {
            ft_putchar(' ');
            chars_written++;
        }
    }
    return (chars_written);
}

int ft_convert_and_print(unsigned long long n, int base, int is_signed, 
                          int is_negative, t_flags *flags, char conv)
{
    char    converted[64];
    char    *digits;
    int     len;
    int     chars_written = 0;
    int     is_zero = (n == 0);

    digits = "0123456789abcdef";
    len = 0;

    // Convert to string
    if (is_zero)
        converted[len++] = '0';
    else
    {
        while (n)
        {
            converted[len++] = digits[n % base];
            n /= base;
        }
    }

    // Uppercase for hex
    if (conv == 'X')
    {
        for (int i = 0; i < len; i++)
            if (converted[i] >= 'a' && converted[i] <= 'f')
                converted[i] = converted[i] - 'a' + 'A';
    }

    // Reverse the string
    ft_reverse_str(converted, len);

    // Precision padding for numbers
    int precision_padding = 0;
    if (flags->precision > len)
        precision_padding = flags->precision - len;

    // Total padding calculation
    int total_padding = 0;
    int total_len = len + precision_padding;
    
    // Add space for prefix and sign
    if (flags->alt_form && !is_zero && (conv == 'x' || conv == 'X'))
        total_len += 2;
    if (is_signed && (is_negative || flags->plus_sign || flags->space_sign))
        total_len++;

    if (flags->width > total_len)
        total_padding = flags->width - total_len;

    // Handle right-aligned padding (without left alignment)
    if (!flags->left_align && total_padding > 0)
    {
        // Decide padding character (space or zero)
        char pad_char = (flags->zero_pad && flags->precision == -1) ? '0' : ' ';
        
        while (total_padding--)
        {
            ft_putchar(pad_char);
            chars_written++;
        }
    }

    // Print sign and prefix
    chars_written += ft_print_sign(is_signed, is_negative, flags);
    chars_written += ft_print_hex_prefix(flags, conv, is_zero);

    // Precision padding for numbers
    while (precision_padding--)
    {
        ft_putchar('0');
        chars_written++;
    }

    // Print converted number
    for (int i = 0; i < len; i++)
    {
        ft_putchar(converted[i]);
        chars_written++;
    }

    // Handle left-aligned padding
    if (flags->left_align && total_padding > 0)
    {
        while (total_padding--)
        {
            ft_putchar(' ');
            chars_written++;
        }
    }

    return (chars_written);
}

void intialize_flags(t_flags *flags)
{
    flags->left_align = 0;
    flags->zero_pad = 0;
    flags->width = 0;
    flags->precision = -1;
    flags->plus_sign = 0;
    flags->space_sign = 0;
    flags->alt_form = 0;
}

void parse_width(const char **format, t_flags *flags)
{
    while (**format >= '0' && **format <= '9')
    {
        flags->width = flags->width * 10 + (**format - '0');
        (*format)++;
    }
}

void parse_precision(const char **format, t_flags *flags)
{
    if (**format == '.')
    {
        (*format)++;
        flags->precision = 0;
        while (**format >= '0' && **format <= '9')
        {
            flags->precision = flags->precision * 10 + (**format - '0');
            (*format)++;
        }
    }
}

int ft_parse_flags(const char **format, t_flags *flags)
{
    intialize_flags(flags);

    // Parse flag characters
    while (**format == '-' || **format == '0' || **format == '+' || 
           **format == ' ' || **format == '#')
    {
        if (**format == '-')
            flags->left_align = 1;
        else if (**format == '0')
            flags->zero_pad = 1;
        else if (**format == '+')
            flags->plus_sign = 1;
        else if (**format == ' ')
            flags->space_sign = 1;
        else if (**format == '#')
            flags->alt_form = 1;
        (*format)++;
    }
    parse_width(format, flags);
    parse_precision(format, flags);

    return (0);
}

int ft_printf(const char *format, ...)
{
    va_list args;
    int     chars_written;
    t_flags flags;

    va_start(args, format);
    chars_written = 0;

    while (*format)
    {
        if (*format == '%')
        {
            format++;
            ft_parse_flags(&format, &flags);

            // Character conversion
            if (*format == 'c')
            {
                char c = (char)va_arg(args, int);
                int width_padding = (flags.width > 1) ? flags.width - 1 : 0;

                // Handle right-aligned padding (spaces before the character)
                if (!flags.left_align && width_padding > 0) {
                    while (width_padding--) {
                        ft_putchar(' ');
                        chars_written++;
                    }
                }

                // Print the character
                ft_putchar(c);
                chars_written++;

                // Handle left-aligned padding (spaces after the character)
                if (flags.left_align && width_padding > 0)
                {
                    while (width_padding--)
                    {
                        ft_putchar(' ');
                        chars_written++;
                    }
                }
            }
            // Integer conversions
            else if (*format == 'd' || *format == 'i')
            {
                long long num = va_arg(args, int);
                int is_negative = 0;
                if (num < 0)
                {
                    is_negative = 1;
                    num = -num;
                }
                chars_written += ft_convert_and_print(num, 10, 1, is_negative, &flags, *format);
            }
            // Unsigned conversions
            else if (*format == 'u')
                chars_written += ft_convert_and_print(va_arg(args, unsigned int), 10, 0, 0, &flags, *format);
            // Hex conversions
            else if (*format == 'x' || *format == 'X')
                chars_written += ft_convert_and_print(va_arg(args, unsigned int), 16, 0, 0, &flags, *format);
            // Pointer conversion
            else if (*format == 'p')
            {
                void *ptr = va_arg(args, void*);
                ft_putstr("0x", 2);
                chars_written += 2;
                chars_written += ft_convert_and_print((unsigned long)ptr, 16, 0, 0, &flags, 'x');
            }
            else if (*format == '%')
            {
                int padding = flags.width > 1 ? flags.width - 1 : 0;
                if (!flags.left_align)
                    while (padding--) ft_putchar(' ');
                ft_putchar('%');
                if (flags.left_align)
                    while (padding--) ft_putchar(' ');
                chars_written += (flags.width > 1) ? flags.width : 1;
            }
            // String conversion
            else if (*format == 's')
            {
                char *str = va_arg(args, char *);
                if (!str) str = "(null)";
                int len = 0;
                while (str[len] && (flags.precision == -1 || len < flags.precision))
                    len++;
                int padding = flags.width > len ? flags.width - len : 0;
                if (!flags.left_align)
                    while (padding--) ft_putchar(' ');
                ft_putstr(str, len);
                if (flags.left_align)
                    while (padding--) ft_putchar(' ');
                chars_written += (flags.width > len) ? flags.width : len;
            }
        }
        else
        {
            ft_putchar(*format);
            chars_written++;
        }
        format++;
    }

    va_end(args);
    return (chars_written);
}
