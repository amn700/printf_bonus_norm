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
    int len;

    len = 0;
    while (n)
    {
        n /= base;
        len++;
    }
    return (len ? len : 1);
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
    int     chars_written;
    int     is_zero;

    digits = "0123456789abcdef";
    len = 0;
    chars_written = 0;
    is_zero = (n == 0);

    // Convert to string
    if (is_zero)
    {
        converted[len++] = '0';
    }
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
        int i = 0;
        while (i < len)
        {
            if (converted[i] >= 'a' && converted[i] <= 'f')
                converted[i] = converted[i] - 'a' + 'A';
            i++;
        }
    }

    // Reverse the string
    ft_reverse_str(converted, len);

    // Padding logic for width
    int padding = 0;
    if (flags->width > len + chars_written)
        padding = flags->width - len - chars_written;

    if (!flags->left_align && flags->zero_pad)
    {
        // Zero padding
        while (padding--)
        {
            ft_putchar('0');
            chars_written++;
        }
    }

    // Print prefix, sign, and number
    chars_written += ft_print_hex_prefix(flags, conv, is_zero);
    chars_written += ft_print_sign(is_signed, is_negative, flags);

    // Print the number
    int i = 0;
    while (i < len)
    {
        ft_putchar(converted[i]);
        chars_written++;
        i++;
    }

    // After printing, handle left alignment if needed
    if (flags->left_align)
    {
        while (padding--)
        {
            ft_putchar(' ');
            chars_written++;
        }
    }

    return (chars_written);
}

int ft_parse_flags(const char **format, t_flags *flags)
{
    // Reset flags
    flags->left_align = 0;
    flags->zero_pad = 0;
    flags->width = 0;
    flags->precision = -1;
    flags->plus_sign = 0;
    flags->space_sign = 0;
    flags->alt_form = 0;

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

    // Parse width
    while (**format >= '0' && **format <= '9')
    {
        flags->width = flags->width * 10 + (**format - '0');
        (*format)++;
    }

    // Parse precision
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
                ft_putchar(c);
                chars_written++;
            }
            // String conversion
            else if (*format == 's')
            {
                char *s = va_arg(args, char*);
                int len = 0;
                while (s[len] && (flags.precision == -1 || len < flags.precision))
                    len++;
                ft_putstr(s, len);
                chars_written += len;
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
            // Percent sign
            else if (*format == '%')
            {
                ft_putchar('%');
                chars_written++;
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
