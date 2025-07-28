/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_printf.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 22:46:04 by jesuserr          #+#    #+#             */
/*   Updated: 2025/07/28 13:36:52 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Taking as reference the original ft_printf, this version is designed to
// work with a buffer to speed up printing operations. It accumulates output
// in a static buffer and flushes it to stdout when full, reducing the number
// of system calls. Supports basic format specifiers (%s, %d, %%). Manual flush
// implemented also to allow flush per user request.

#include "ft_ls.h"

static char		g_printf_buffer[BUFFER_SIZE_PRINTF];
static uint16_t	g_buffer_pos = 0;

static void	add_char_to_buffer(char c)
{
	if (g_buffer_pos >= BUFFER_SIZE_PRINTF - 1)
	{
		write(STDOUT_FILENO, g_printf_buffer, g_buffer_pos);
		g_buffer_pos = 0;
	}
	g_printf_buffer[g_buffer_pos++] = c;
}

static void	ft_putstr(char *s)
{
	if (!s)
		s = "(null)";
	while (*s)
		add_char_to_buffer(*s++);
}

static void	ft_put_number(uint64_t n)
{
	if (n >= 10)
	{
		ft_put_number(n / 10);
		ft_put_number(n % 10);
	}
	else
	{
		n = n + 48;
		add_char_to_buffer((char)n);
	}
}

void	ft_printf_flush(void)
{
	if (g_buffer_pos > 0)
	{
		write(STDOUT_FILENO, g_printf_buffer, g_buffer_pos);
		g_buffer_pos = 0;
	}
}

void	ft_printf(char const *hold, ...)
{
	va_list			args;

	va_start (args, hold);
	while (*hold)
	{
		if (*hold != '%')
			add_char_to_buffer(*hold++);
		else
		{
			hold++;
			if ((*hold) == '%')
				add_char_to_buffer('%');
			else if (*hold == 's' || (*hold == ' ' && *(hold + 1) == 's'))
				ft_putstr(va_arg (args, char *));
			else if (*hold == 'd')
				ft_put_number(va_arg (args, uint64_t));
			hold++;
		}
	}
	va_end (args);
}
