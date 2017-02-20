/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singleton.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/20 16:42:25 by lbopp             #+#    #+#             */
/*   Updated: 2017/02/20 16:42:27 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

t_info	*singleton(t_info *data)
{
	static t_info	*info;

	if (data)
		info = data;
	return (info);
}

void	init_sigleton(t_lst_cir **lst_cir, int fd)
{
	t_info	*info;

	if (!(info = (t_info*)ft_memalloc(sizeof(t_info))))
		return ;
	info->fd = fd;
	info->lst_cir = lst_cir;
	info = singleton(info);
}
