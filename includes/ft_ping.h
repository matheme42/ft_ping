/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxence <maxence@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/11 13:07:26 by matheme           #+#    #+#             */
/*   Updated: 2021/01/21 14:54:06 by maxence          ###   ########lyon.fr   */
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

// ping packet structure 
struct ping_pkt 
{ 
    struct icmphdr hdr; 
    char msg[56 - sizeof(struct icmphdr)]; 
};


struct time_ping_data
{
    struct timeval  start_time;
    struct timeval  endtime;
    int             min_time;
    int             max_time;
    int             avg_time;
    long long       mdev_time;
};

// @todo work in progress
typedef struct  s_ping_data
{
    char        *hostname;
    unsigned int packet_success;
    unsigned int packet_failure;
    struct time_ping_data timestats;
}               t_ping_data;

t_ping_data g_ping_data;

// initialise and get the raw_socket
int             raw_socket();

// dnslookup
int             lookup_host(const char *host, struct sockaddr **sockaddr);

// send packet
struct timeval  *send_packet(int sockfd, struct sockaddr* clientaddr, const char *hostname, int seq);

// receive packet
void            receive_packet(const int sockfd, struct timeval sendtime, const char *hostname, int seq);

// error
int             error(int err, char *prog_name, char *host_name);

// statistic
void            statistic(const char *host);

# endif