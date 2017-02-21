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
	default_term();
	ft_putstr_fd(tgetstr("te", NULL), fd);
	ft_putstr_fd(tgetstr("ve", NULL), fd);
	close(fd);
	if (buffer[0] == '\n')
		print_select_lst_cir(lst_cir);
	exit(0);
}

void	init_term(void)
{
	struct termios	attr;
	char			*term;

	tcgetattr(STDIN_FILENO, &attr);
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
	struct termios	attr;

	tcgetattr(STDIN_FILENO, &attr);
	attr.c_lflag |= (ECHO | ICANON);
	tcsetattr(STDIN_FILENO, TCSADRAIN, &attr);
	tgetent(NULL, getenv("TERM"));
}
