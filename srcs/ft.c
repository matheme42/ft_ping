/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxence <maxence@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/28 14:25:22 by maxence           #+#    #+#             */
/*   Updated: 2021/05/28 18:10:34 by maxence          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void ft_bzero(void *ptr, int size)
{
    while (size--)
        ((char*)ptr)[size] = 0;
}

void	*ft_memset(void *b, int c, size_t len)
{
	size_t i;

	i = 0;
	while (i < len)
	{
		((unsigned char*)b)[i] = c;
		i++;
	}
	return (b);
}

int		ft_atoi(const char *str)
{
	int i;
	int neg;
	int result;

	neg = 1;
	result = 0;
	i = 0;
	while (str[i] && ((str[i] >= 9 && str[i] <= 13) || str[i] == 32))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			neg = -1;
		i++;
	}
	str = &str[i];
	i = -1;
	while (str[++i] >= '0' && str[i] <= '9')
		result = result * 10 + str[i] - '0';
	return (result * neg);
}

char	*ft_strchr(const char *s, int c)
{
	int i;

	i = 0;
	while (s[i] && s[i] != c)
		i++;
	if (s[i] == '\0' && c != '\0')
		return (NULL);
	return (&((char*)s)[i]);
}