/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/20 16:33:50 by lbopp             #+#    #+#             */
/*   Updated: 2017/02/20 16:33:51 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

static void		move_to(int x, int y, int fd)
{
	char	*s;

	s = ft_strjoin("\033[", ft_itoa(y));
	s = ft_stradd(s, ";");
	s = ft_stradd(s, ft_itoa(x));
	s = ft_stradd(s, "H");
	ft_putstr_fd(s, fd);
	free(s);
}

static void		draw_first_elem(t_lst_cir **lst_cir, int fd, int *x, int *y)
{
	struct winsize	w;

	ioctl(fd, TIOCGWINSZ, &w);
	if ((*lst_cir)->select == 1)
		ft_putstr_fd("\033[7m", fd);
	if ((*lst_cir)->curseur == 1)
		ft_putstr_fd("\033[4m", fd);
	ft_putstr_fd((*lst_cir)->content, fd);
	if (*y == w.ws_row)
	{
		*y = 0;
		*x += (*lst_cir)->size_max + 1;
	}
	move_to(*x, *y + 1, fd);
	*y += 1;
	ft_putstr_fd("\033[0m", fd);
}

static void		draw_other_elem(t_lst_cir **lst_cir, int fd, int *x, int *y)
{
	t_lst_cir		*tmp;
	struct winsize	w;

	ioctl(fd, TIOCGWINSZ, &w);
	tmp = (*lst_cir)->prev;
	while (tmp != *lst_cir)
	{
		if (tmp->select == 1)
			ft_putstr_fd("\033[7m", fd);
		if (tmp->curseur == 1)
			ft_putstr_fd("\033[4m", fd);
		ft_putstr_fd(tmp->content, fd);
		ft_putstr_fd("\033[0m", fd);
		if (*y == w.ws_row)
		{
			*y = 0;
			*x += tmp->size_max + 1;
		}
		move_to(*x, *y + 1, fd);
		*y += 1;
		tmp = tmp->prev;
	}
}

void			draw_window(t_lst_cir **lst_cir, int fd)
{
	int	x;
	int	y;

	x = 1;
	y = 1;
	ft_putstr_fd(tgetstr("cl", NULL), fd);
	draw_first_elem(lst_cir, fd, &x, &y);
	draw_other_elem(lst_cir, fd, &x, &y);
}
