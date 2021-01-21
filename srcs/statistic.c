/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   statistic.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxence <maxence@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 15:28:38 by maxence           #+#    #+#             */
/*   Updated: 2020/12/18 18:46:41 by maxence          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "ft_ping.h"

void statistic(const char* host)
{
    unsigned int packet_lost;
    unsigned int total_time;
    int          average_time;

    packet_lost = 0;
    if (g_ping_data.packet_failure + g_ping_data.packet_success != 0)
        packet_lost = (double)g_ping_data.packet_failure / ((double)g_ping_data.packet_failure + (double)g_ping_data.packet_success) * 100;
    
    dprintf(1, "\n--- %s ping statistics ---\n", host);

    // // 2 packets transmitted, 2 received, 0% packet loss, time 1001ms
    dprintf(1, "%u packets transmitted, %u received, %u%% packet lost, time 0ms\n", g_ping_data.packet_success + g_ping_data.packet_failure, g_ping_data.packet_success, packet_lost);

    total_time = (g_ping_data.timestats.endtime.tv_sec - g_ping_data.timestats.start_time.tv_sec) * 1000000 + (g_ping_data.timestats.endtime.tv_usec - g_ping_data.timestats.start_time.tv_usec);
    total_time /= 1000;

    average_time = 0;

    //rtt min/avg/max/mdev = 116.040/117.522/119.005/1.521 ms
    dprintf(1, "rtt min/avg/max/mdev = %d.%.3d/%d.%.3d/%d.%.3d/%d.%.3d %ums\n", g_ping_data.timestats.min_time / 1000,  g_ping_data.timestats.min_time % 1000 , g_ping_data.timestats.max_time / 1000, g_ping_data.timestats.max_time % 1000, g_ping_data.timestats.avg_time / 1000, g_ping_data.timestats.avg_time % 1000, abs(g_ping_data.timestats.mdev_time) / 1000, abs(g_ping_data.timestats.mdev_time) % 1000, total_time);
}