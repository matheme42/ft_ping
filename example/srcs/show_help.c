/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_help.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdaufin <jdaufin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/17 11:48:13 by jdaufin           #+#    #+#             */
/*   Updated: 2020/11/19 09:50:35 by jdaufin          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	show_help(void)
{
	printf("Usage: ft_ping [-hv] [-w deadline] [-W timeout] [-c count]");
	printf(" [-t time-to-live] destination\n");
}
