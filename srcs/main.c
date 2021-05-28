/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxence <maxence@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 17:05:41 by maxence           #+#    #+#             */
/*   Updated: 2021/05/29 00:05:28 by maxence          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "ft_ping.h"

option	opt;

static void interrupt(int value)
{
	statistic();
	exit(EXIT_SUCCESS);
}

static int ping(const char *hostname, struct sockaddr *clientaddr)
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
		if (opt['c'] && --opt['c'] == 0)
				interrupt(0);
		usleep(opt['i'] ? opt['i'] * 1000000 : 1000000);

		if (opt['w'] && (get_time() - g_stat()->start_time) / 1000000 >= opt['w'])
			interrupt(0);
	}
	return (EXIT_SUCCESS);
}

static void set_gstat(char *progname)
{
	g_stat()->domain_name = progname;
	g_stat()->start_time = get_time();
}

int main(int ac, char **av)
{
	struct	sockaddr	*destaddr;
	char				*progname;
	int					err;

	//accept only if you give an argument
	if (ac < 2)
		return (error(E_USAGE, av[0], NULL));
	ft_bzero(opt, sizeof(option));
	progname = av[0];
	av = get_option(ac - 1, &av[1], opt);

	if (opt['h'])
		usage(0);

	//transform the domain to an binary address
    if ((err = lookup_host(*av, &destaddr)))
		return (error(err, progname, *av));
	
	set_gstat(progname);
	
	// catch the signal
	signal(SIGINT, interrupt);

	// start the ping
	if ((err = ping(*av, destaddr)))
		return (error(err, progname, *av));

	return (EXIT_SUCCESS);
}

