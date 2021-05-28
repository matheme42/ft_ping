/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   statistic.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxence <maxence@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/22 12:47:06 by maxence           #+#    #+#             */
/*   Updated: 2021/05/28 23:49:25 by maxence          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

t_stats *g_stat()
{
    static t_stats *stat = NULL;

    if (stat == NULL)
    {
        stat = malloc(sizeof(*stat));
        stat->domain_name = NULL;
        stat->packets_error = 0;
        stat->packets_recvd = 0;
        stat->packets_send = 0;
        stat->rtt_max = 0;
        stat->rtt_min = 0;
        stat->rtt_sq_total = 0;
        stat->rtt_total = 0;
        stat->start_time = 0;
    }
    return (stat);
}

void statistic()
{
    t_stats *stat = g_stat();

    int lostpacket =  100 - (((float)stat->packets_recvd / (stat->packets_send ? (float)stat->packets_send : (float)1)) * 100);
    int avg = stat->rtt_total / (stat->packets_recvd ? stat->packets_recvd : 1);
    suseconds_t mdev;

    mdev = stat->rtt_total / (stat->packets_recvd ? stat->packets_recvd : 1);
	stat->rtt_sq_total /= (stat->packets_recvd ? stat->packets_recvd : 1);
	mdev = sqrtl(stat->rtt_sq_total - mdev * mdev);

    dprintf(1, "--- %s ping statistics ---\n", stat->domain_name);
    if (stat->packets_error > 0)
    {
        dprintf(1, "%d packets transmitted, %d received, +%d errors, %d%% packet loss, time %ldms\n", stat->packets_send, stat->packets_recvd, stat->packets_error, lostpacket,  (get_time() - stat->start_time) / 1000);   
    }
    else
    {
        dprintf(1, "%d packets transmitted, %d received, %d%% packet loss, time %ldms\n", stat->packets_send, stat->packets_recvd, lostpacket,  (get_time() - stat->start_time) / 1000);   
    }

    if (stat->packets_recvd > 1)
        dprintf(1, "rtt min/avg/max/mdev = %ld.%ld/%ld.%ld/%ld.%ld/%ld.%ld ms\n", stat->rtt_min / 1000l, stat->rtt_min % 1000l, avg / 1000l, avg % 1000l, stat->rtt_max / 1000l, stat->rtt_max % 1000l, mdev / 1000l, mdev % 1000l);
    free(stat);
}