/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dnslookup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxence <maxence@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 10:50:47 by maxence           #+#    #+#             */
/*   Updated: 2020/12/18 17:51:00 by maxence          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "ft_ping.h"

//                      localhost       0x 0a 0f 0t 0x ..   
int lookup_host (const char *host, struct sockaddr **sockaddr)
{
	struct	addrinfo hints;
	struct	addrinfo *res;
	void	*ptr;


	bzero(&hints, sizeof(hints));

	hints.ai_socktype = SOCK_DGRAM; // send UDP packet
	hints.ai_family = AF_INET; // return only IPV4 addr

	if (!getaddrinfo (host, NULL, &hints, &res))
	{
		*sockaddr = res->ai_addr;
		return (EXIT_SUCCESS);
	}
	return (E_UNKNOWN_HOST);
}