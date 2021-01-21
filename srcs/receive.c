/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   receive.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxence <maxence@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 12:08:48 by maxence           #+#    #+#             */
/*   Updated: 2021/01/21 15:03:32 by maxence          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "ft_ping.h"

static void preparemsghdr(struct msghdr	*callback)
{
    struct iovec    iov;
    struct sockaddr sin;
    char            control[64];
    char            databuff[84];

    bzero(&sin, sizeof(sin));
    bzero(callback, sizeof(*callback));
    bzero(control, sizeof(*control));
    bzero(databuff, sizeof(*databuff));

    callback->msg_control = control;
    callback->msg_controllen = sizeof(control);
    callback->msg_flags = 0;
    callback->msg_iov = &iov;
    callback->msg_iovlen = 1;
    callback->msg_name = &sin;
    callback->msg_namelen = sizeof(sin);
    callback->msg_iov->iov_base = databuff;
    callback->msg_iov->iov_len = sizeof(databuff);
}

static void fill_statistic(int time)
{
    if (time < g_ping_data.timestats.min_time || g_ping_data.timestats.min_time == 0)
        g_ping_data.timestats.min_time = time;
    if (time > g_ping_data.timestats.max_time || g_ping_data.timestats.max_time == 0)
        g_ping_data.timestats.max_time = time;

    g_ping_data.timestats.avg_time = ((g_ping_data.timestats.avg_time * g_ping_data.packet_success) + time) / (g_ping_data.packet_success + 1);

    g_ping_data.timestats.mdev_time += 0;
}

void checkpacket(void *packet)
{
    struct iphdr	*ip = packet;
	struct icmphdr	*icmp = packet;

    dprintf(1, "%d\n", icmp->type);
}

void receive_packet(const int sockfd, struct timeval sendtime, const char *hostname, int seq)
{
    struct timeval	receivetime;
    struct msghdr	callback;
    unsigned int    time;
    char            clientname[15];
    unsigned int    byte_receive;

    // prepare the callback
    preparemsghdr(&callback);

	// receive the packet
	byte_receive = recvmsg(sockfd, &callback, 0);

    checkpacket(callback.msg_iov);

	gettimeofday(&receivetime, NULL);
    inet_ntop (((struct sockaddr_in *)callback.msg_name)->sin_family, &((struct sockaddr_in *)callback.msg_name)->sin_addr, clientname, 15);


	time = ((receivetime.tv_sec -  sendtime.tv_sec) * 1000000);
    time += (receivetime.tv_usec - sendtime.tv_usec);

    fill_statistic(time);    
    g_ping_data.packet_success += 1;

//	64 bytes from localhost (127.0.0.1): icmp_seq=1 ttl=64 time=0.076 ms
	dprintf(1, "%d bytes from %s (%s): icmp_seq=%d ttl=64 time=%d.%d ms\n", byte_receive, hostname, clientname, seq, time / 1000, (time % 1000));  
}