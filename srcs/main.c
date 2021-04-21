/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxence <maxence@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 17:05:41 by maxence           #+#    #+#             */
/*   Updated: 2021/01/22 17:40:14 by maxence          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "ft_ping.h"


void interrupt(int value)
{
	statistic();
	exit(EXIT_SUCCESS);
}

int ping(const char *hostname, struct sockaddr *clientaddr)
{
	static int		seq = 0;
	struct timeval	*sendtime;

	if (raw_socket() < 0) {
		return (raw_socket());
	}

	while (++seq)
	{
		if (!send_packet(raw_socket(), clientaddr, seq, hostname))
			receive_packet(raw_socket(), clientaddr);
		usleep(1000000);
	}
	
	return (EXIT_SUCCESS);
}

void set_gstat(char *progname)
{
	g_stat()->domain_name = progname;
	g_stat()->start_time = get_time();
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
	
	set_gstat(av[1]);
	
	// catch the signal
	signal(SIGINT, interrupt);

	// start the ping
	if ((err = ping(av[1], destaddr)))
		return (error(err, av[0], av[1]));

	return (EXIT_SUCCESS);
}

