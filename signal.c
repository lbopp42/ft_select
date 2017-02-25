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

static void	signalhandle(int signum)
{
	t_info	*info;

	(void)signum;
	info = singleton(NULL);
	draw_window(info->lst_cir, info->fd);
}

static void	signalstop(int signum)
{
	struct winsize	w;
	t_info			*info;
	char			c;

	(void)signum;
	info = singleton(NULL);
	c = info->init_term.c_cc[VSUSP];
	ioctl(0, TIOCSTI, &c);
	signal(SIGTSTP, SIG_DFL);
	default_term();
	tputs(tgetstr("te", NULL), 1, &put_my_char);
	tputs(tgetstr("ve", NULL), 1, &put_my_char);
}

static void	signalcont(int signum)
{
	t_info	*info;

	(void)signum;
	info = singleton(NULL);
	signal(SIGTSTP, &signalstop);
	tputs(tgetstr("vi", NULL), 1, &put_my_char);
	tputs(tgetstr("ti", NULL), 1, &put_my_char);
	init_term();
	draw_window(info->lst_cir, info->fd);
}

void		manage_signal(void)
{
	signal(SIGWINCH, &signalhandle);
	signal(SIGTSTP, &signalstop);
	signal(SIGCONT, &signalcont);
}