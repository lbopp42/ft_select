/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/20 16:52:47 by lbopp             #+#    #+#             */
/*   Updated: 2017/03/01 13:46:30 by lbopp            ###   ########.fr       */
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

static void	leave_clean(int signum)
{
	t_info	*info;

	(void)signum;
	info = singleton(NULL);
	tputs(tgetstr("te", NULL), 1, &put_my_char);
	tputs(tgetstr("ve", NULL), 1, &put_my_char);
	close(info->fd);
	exit(0);
}

void		manage_signal(void)
{
	int			i;
	const int	sign_array[25] = {SIGABRT, SIGALRM, SIGFPE, SIGHUP, SIGILL,
		SIGINT, SIGKILL, SIGPIPE, SIGQUIT, SIGSEGV, SIGTERM, SIGUSR1, SIGUSR2,
		SIGCHLD, SIGSTOP, SIGTTIN, SIGTTOU, SIGBUS, SIGPROF, SIGSYS,
		SIGTRAP, SIGURG, SIGVTALRM, SIGXCPU, SIGXFSZ};

	i = 0;
	while (i < 25)
	{
		signal(sign_array[i], &leave_clean);
		i++;
	}
	signal(SIGWINCH, &signalhandle);
	signal(SIGTSTP, &signalstop);
	signal(SIGCONT, &signalcont);
}
