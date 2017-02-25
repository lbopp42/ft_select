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

int 		put_my_char(int c)
{
	t_info	*info;

	info = singleton(NULL);
	write(info->fd, &c, 1);
	return (1);
}

int			main(int ac, char *av[])
{
	t_lst_cir		*lst_cir;
	int				fd;
	int				len_max;
	t_info			*info;

	if (ac < 2)
		return (0);
	len_max = 0;
	lst_cir = NULL;
	fd = 0;
	fd = open("/dev/tty", O_RDWR);
	len_max = count_len_max(ac, av);
	fill_list_arg(ac, av, len_max, &lst_cir);
	init_sigleton(&lst_cir, fd);
	manage_signal();
	info = singleton(NULL);
	init_term();
	tputs(tgetstr("vi", NULL), 1, &put_my_char);
	tputs(tgetstr("ti", NULL), 1, &put_my_char);
	lst_cir->curseur = 1;
	draw_window(&lst_cir, info->fd);
	check_key(&lst_cir, info->fd);
	tputs(tgetstr("te", NULL), 1, &put_my_char);
	tputs(tgetstr("ve", NULL), 1, &put_my_char);
	return (0);
}
