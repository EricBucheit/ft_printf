/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   num.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebucheit <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/30 18:53:15 by ebucheit          #+#    #+#             */
/*   Updated: 2017/05/30 18:53:15 by ebucheit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int			get_size(uintmax_t num, int base)
{
	int len;

	len = 1;
	while (num /= base)
		len++;
	return (len);
}

void	spaces(int n, t_flags *flags, int c)
{
	int i;

	i = 0;
	if (c)
		c = '0';
	else
		c = ' ';
	if (n > 0)
	{
		while (i < n)
		{
			ft_putchar(c);
			i++;
		}
	}
}

void	number_print2(char *str, t_flags *flags)
{
	int size;

	size = ft_strlen(str);
	if (flags->neg)
	{
		ft_putstr(str);
		spaces(flags->width - size, flags, 0);
	}
	else
	{
		if (flags->zero)
			spaces(flags->width - size, flags, 1);
		else
			spaces(flags->width - size, flags, 0);
		ft_putstr(str);
	}
	ft_strdel(&str);
}

void		number_print(char *str, t_flags flags)
{
	int len;

	len = ft_strlen(str);
	if (flags.neg)
	{
		ft_putstr(str);
		prepend_width(flags, flags.width - len);
	}
	else
	{
		prepend_width(flags, flags.width - len);
		ft_putstr(str);
	}
}

void		num_to_s(uintmax_t number, t_flags flags, char before)
{
	int		len;
	char	*nbr;

	len = get_size(number, 10);
	if (flags.precision >= len && !(flags.zero_spacer = 0))
		len = flags.precision;
	if (before != 0 && !flags.zero_spacer)
		len++;
	else if (before == '-' && flags.zero_spacer && !flags.neg)
		len++;
	nbr = ft_strnew(len);
	nbr[len] = '\0';
	while (--len >= 0)
	{
		if (number != 0)
			nbr[len] = number % 10 + '0';
		else
			nbr[len] = '0';
		number /= 10;
	}
	if (before != 0 && flags.zero_spacer && flags.width - 1)
		ft_putchar(before);
	if (before != 0)
		nbr[0] = before;
	number_print2(nbr, flags);
}

intmax_t	promo_check(va_list ap, t_flags flags, char *fmt)
{
	intmax_t	res;

	if (*fmt == 'D' || flags.l)
		res = va_arg(ap, long);
	else if (flags.h)
		res = (short)va_arg(ap, int);
	else if (flags.hh)
		res = (signed char)va_arg(ap, int);
	else if (flags.ll)
		res = va_arg(ap, long long);
	else if (flags.z)
		res = va_arg(ap, ssize_t);
	else if (flags.j)
		res = va_arg(ap, intmax_t);
	else
		res = va_arg(ap, int);
	return (res);
}

void		num_parse(va_list ap, t_flags flags, char *fmt)
{
	intmax_t	number;
	char		before;

	number = promo_check(ap, flags, fmt);
	if (flags.plus_sign != 0)
		before = '+';
	else
		before = 0;
	if (flags.space && !flags.plus_sign)
		before = ' ';
	if (number < 0)
	{
		number *= -1;
		before = '-';
	}
	if (number == 0 && !flags.width && flags.precision)
		return ;
	if (number == 0 && flags.precision)
		ft_putchar(' ');
	num_to_s((uintmax_t)number, flags, before);
}
