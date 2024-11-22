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

int ft_printf(const char *format, ...);
#endif