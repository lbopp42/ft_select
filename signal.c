/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/20 16:52:47 by lbopp             #+#    #+#             */
/*   Updated: 2017/02/20 16:53:13 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

void	signalhandle(int signum)
{
	t_info	*info;

	(void)signum;
	info = singleton(NULL);
	draw_window(info->lst_cir, info->fd);
}
