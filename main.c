/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/20 17:02:39 by lbopp             #+#    #+#             */
/*   Updated: 2017/02/20 17:09:38 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

static int	count_len_max(int ac, char **av)
{
	int i;
	int len_max;

	i = 1;
	len_max = 0;
	while (i < ac)
	{
		if (len_max < ft_strlen(av[i]))
			len_max = ft_strlen(av[i]);
		i++;
	}
	return (len_max);
}

static void	fill_list_arg(int ac, char **av,
		int len_max, t_lst_cir **lst_cir)
{
	int			i;
	t_lst_cir	*new;

	i = 1;
	new = NULL;
	while (i < ac)
	{
		new = create_lst_cir(av[i], len_max);
		add_next_lst_cir(lst_cir, new, len_max);
		i++;
	}
}

int			main(int ac, char *av[])
{
	t_lst_cir		*lst_cir;
	int				fd;
	int				len_max;

	if (ac < 2)
		return (0);
	len_max = 0;
	lst_cir = NULL;
	fd = 0;
	fd = open("/dev/tty", O_RDWR);
	init_term();
	len_max = count_len_max(ac, av);
	fill_list_arg(ac, av, len_max, &lst_cir);
	ft_putstr_fd(tgetstr("ti", NULL), fd);
	ft_putstr_fd(tgetstr("vi", NULL), fd);
	lst_cir->curseur = 1;
	init_sigleton(&lst_cir, fd);
	signal(SIGWINCH, &signalhandle);
	draw_window(&lst_cir, fd);
	check_key(&lst_cir, fd);
	ft_putstr_fd(tgetstr("te", NULL), fd);
	ft_putstr_fd(tgetstr("ve", NULL), fd);
	return (0);
}
