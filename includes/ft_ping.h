/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxence <maxence@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/11 13:07:26 by matheme           #+#    #+#             */
/*   Updated: 2021/01/22 17:42:05 by maxence          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# ifndef FT_PING_H
# define FT_PING_H

// stdio.h allow the use of printf
# include <stdio.h>

// use for the define EXIT_FAILURE EXIT_SUCCESS
# include <stdlib.h>

// netdb.h allow the use of getaddrinfo and his define
# include <netdb.h>

// allow the use of setsocket
# include <sys/socket.h>

// use to add getpid()
# include <unistd.h>

// arpa/inet.h allow to use of inet_ntop
# include <arpa/inet.h>

// use to add the struct icmp
# include <netinet/ip.h>
# include <netinet/ip_icmp.h>
# include <netinet/in_systm.h>

// allow the use of gettimeoftheday
#include <sys/time.h>

// error.r add multiple define use for error
# include "error.h"

# include <string.h>
//allow to use of bzero

// allow the use of signal
#include <signal.h>

// allow the use of sqrt
#include <math.h>


# define IP_HDR_SIZE			20 // size of the ip header
# define ICMP_HDR_SIZE			ICMP_MINLEN // size of the header of the ICMP request
# define ICMP_PAYLOAD_SIZE		56 // size of the data of the ICMP request
# define PACKET_SIZE			(IP_HDR_SIZE + ICMP_HDR_SIZE + ICMP_PAYLOAD_SIZE) // Global packet size

typedef struct			s_stats
{
	char				*domain_name;
	suseconds_t			rtt_min;
	suseconds_t			rtt_max;
	suseconds_t			rtt_total;
	suseconds_t			rtt_sq_total;
	suseconds_t			start_time;
	uint16_t			packets_recvd;
	uint16_t			packets_send;
	uint16_t			packets_error;
}						t_stats;


// initialise and get the raw_socket
int             raw_socket();

// dnslookup
int             lookup_host(const char *host, struct sockaddr **sockaddr);

// send packet
int				send_packet(const int sockfd, struct sockaddr *dest, int seq, const char *domainname);

// receive packet
void            receive_packet(const int sockfd, struct sockaddr *destaddr);

// error
int             error(int err, char *prog_name, char *host_name);

// statistic
t_stats			*g_stat();
void			statistic();


//recupere le temps actuel
suseconds_t		get_time(void);

# endif