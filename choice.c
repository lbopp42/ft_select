/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   choice.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/20 16:50:28 by lbopp             #+#    #+#             */
/*   Updated: 2017/02/20 16:51:18 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

static void	treat_choice(t_lst_cir **lst_cir,
	t_lst_cir *tmp, int fd, char buffer[])
{
	if (buffer[0] == 27 && buffer[1] == 91 &&
			(buffer[2] == 66 || buffer[2] == 65))
	{
		tmp->curseur = 0;
		tmp->prev->curseur = (buffer[2] == 66) ? 1 : 0;
		tmp->next->curseur = (buffer[2] == 65) ? 1 : 0;
	}
	else if (buffer[0] == 32)
	{
		tmp->curseur = 0;
		tmp->select = tmp->select ? 0 : 1;
		tmp->prev->curseur = 1;
	}
	else if (buffer[0] == 127 || (buffer[0] == 27
				&& buffer[1] == 91 && buffer[2] == 51 && buffer[3] == 126))
	{
		if (tmp == *lst_cir)
			*lst_cir = (*lst_cir)->prev;
		if (tmp->prev == tmp)
		{
			tmp = del_one_lst_cir(tmp);
			exit_term(*lst_cir, fd, buffer);
		}
		tmp = del_one_lst_cir(tmp);
	}
}

void		check_key(t_lst_cir **lst_cir, int fd)
{
	char		buffer[4];
	t_lst_cir	*tmp;

	while (42)
	{
		ft_bzero(buffer, 4);
		read(0, buffer, 4);
		tmp = *lst_cir;
		while (tmp->curseur != 1)
			tmp = tmp->prev;
		(buffer[0] == 4 || buffer[0] == '\n' || (buffer[0] == 27 && !buffer[1]))
		? exit_term(*lst_cir, fd, buffer) : 0;
		treat_choice(lst_cir, tmp, fd, buffer);
		draw_window(lst_cir, fd);
	}
}
