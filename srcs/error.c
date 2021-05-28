/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxence <maxence@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 15:30:37 by maxence           #+#    #+#             */
/*   Updated: 2021/05/29 00:13:16 by maxence          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "ft_ping.h"

int error(int err, char *prog_name, char *host_name)
{

	switch (err)
	{
		case E_USAGE:
			usage(0);
		break ;
		case E_UNKNOWN_HOST:
			dprintf(2, "%s: unknown host %s\n", prog_name, host_name);
		break ;
		case E_SOCKET:
			perror(prog_name);
		break ;
		case E_SEND_SOCKET:
			dprintf(2, "cannot send the packet");
		break ;
	}
	return (EXIT_FAILURE);
}

void usage(char c)
{
	if (c)
		dprintf(1, "ft_ping: option invalide -- '%c'\n", c);
	dprintf(2, "Usage: ft_ping [-hvDaq] [-t ttl] [-c count] [-i interval] [-w deadline] [-W timeout] destination\n");
	exit(EXIT_FAILURE);
}