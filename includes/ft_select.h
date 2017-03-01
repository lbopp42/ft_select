/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/15 12:30:13 by lbopp             #+#    #+#             */
/*   Updated: 2017/03/01 13:45:40 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SELECT_H
# define FT_SELECT_H

# include <stdlib.h>
# include <signal.h>
# include <stdio.h>
# include <string.h>
# include <errno.h>
# include <term.h>
# include "libft.h"
# include <fcntl.h>
# include <sys/ioctl.h>

typedef struct	s_lst_cir
{
	int					select;
	int					curseur;
	int					size_max;
	char				*content;
	struct s_lst_cir	*next;
	struct s_lst_cir	*prev;
}				t_lst_cir;

typedef struct	s_info
{
	t_lst_cir		**lst_cir;
	int				fd;
	struct termios	init_term;
}				t_info;

void			add_next_lst_cir(t_lst_cir **cour, t_lst_cir *new, int len_max);
void			check_key(t_lst_cir **lst_cir, int fd);
t_lst_cir		*create_lst_cir(char *content, int len_max);
void			default_term(void);
t_lst_cir		*del_one_lst_cir(t_lst_cir *list_cir);
void			draw_window(t_lst_cir **lst_cir, int fd);
void			exit_term(t_lst_cir *lst_cir, int fd, char buffer[]);
void			init_sigleton(t_lst_cir **lst_cir, int fd);
void			init_term(void);
void			print_select_lst_cir(t_lst_cir *list_cir);
int 			put_my_char(int c);
void			manage_signal(void);
t_info			*singleton(t_info *data);

#endif
