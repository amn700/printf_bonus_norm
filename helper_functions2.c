#include "ft_printf.h"

void	ft_reverse_str(char *str, int len)
{
	int i = 0, j = len - 1;
	while (i < j) {
		char tmp = str[i];
		str[i++] = str[j];
		str[j--] = tmp;
	}
}
