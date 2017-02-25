/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/20 16:45:25 by lbopp             #+#    #+#             */
/*   Updated: 2017/02/20 16:45:27 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

void	exit_term(t_lst_cir *lst_cir, int fd, char buffer[])
{
	ft_putstr_fd(tgetstr("te", NULL), fd);
	ft_putstr_fd(tgetstr("ve", NULL), fd);
	default_term();
	close(fd);
	if (buffer[0] == '\n')
		print_select_lst_cir(lst_cir);
	exit(0);
}

void	init_term(void)
{
	char			*term;
	struct termios	attr;
	t_info			*info;

	info = singleton(NULL);
	tcgetattr(STDIN_FILENO, &info->init_term);
	tcgetattr(STDIN_FILENO, &attr);
	singleton(info);
	attr.c_lflag &= ~(ECHO | ICANON);
	attr.c_cc[VMIN] = 1;
	attr.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSADRAIN, &attr);
	if (!(term = getenv("TERM")))
	{
		ft_putstr_fd("ft_select: environment not found\n", 2);
		default_term();
		exit(0);
	}
	else
		tgetent(NULL, term);
}

void	default_term(void)
{
	t_info	*info;

	info = singleton(NULL);
	tcsetattr(STDIN_FILENO, TCSADRAIN, &info->init_term);
	tgetent(NULL, getenv("TERM"));
}
