/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxence <maxence@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 17:05:41 by maxence           #+#    #+#             */
/*   Updated: 2021/01/21 14:53:18 by maxence          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "ft_ping.h"

void interrupt(int value)
{
	gettimeofday(&g_ping_data.timestats.endtime, NULL);

	// print the statistic
	statistic(g_ping_data.hostname);
	exit(EXIT_SUCCESS);
}

int ping(const char *hostname, struct sockaddr *clientaddr)
{
	static int		seq = 0;
	struct timeval	*sendtime;

	while (++seq)
	{
		if(!(sendtime = send_packet(raw_socket(), clientaddr, hostname, seq)))
			return (E_SEND_SOCKET);

		receive_packet(raw_socket(), *sendtime, hostname, seq);
		usleep(1000000);
	}
	
	return (EXIT_SUCCESS);
}

int main(int ac, char **av)
{
	struct	sockaddr *destaddr;
	int		err;

	//accept only if you give an argument
	if (ac < 2)
		return (error(E_USAGE, av[0], NULL));

	//transform the domain to an binary address
    if ((err = lookup_host(av[1], &destaddr)))
		return (error(err, av[0], av[1]));

	bzero(&g_ping_data, sizeof(g_ping_data));
	g_ping_data.hostname = av[1];
	gettimeofday(&g_ping_data.timestats.start_time, NULL);
	
	// catch the signal
	signal(SIGINT, interrupt);

	// start the ping
	if ((err = ping(av[1], destaddr)))
		return (error(err, av[0], av[1]));

	return (EXIT_SUCCESS);
}

