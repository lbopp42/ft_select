/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/15 12:30:13 by lbopp             #+#    #+#             */
/*   Updated: 2017/02/15 12:30:15 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SELECT_H
# define FT_SELECT_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <term.h>
#include "minimal.h"
#include "libft.h"
#include <fcntl.h>
#include <sys/ioctl.h>

typedef struct  s_lst_cir
{
    int                 select;
    int                 curseur;
    int					size_max;
    char                *content;
    struct s_lst_cir    *next;
    struct s_lst_cir    *prev;
}               t_lst_cir;

void		add_next_lst_cir(t_lst_cir **cour, t_lst_cir *new, int len_max);
t_lst_cir	*create_lst_cir(char *content, int len_max);
void		ft_putlstcir_fd(t_lst_cir *lst_cir, int fd);
t_lst_cir	*del_one_lst_cir(t_lst_cir *list_cir);
int			test(int c);
void		print_select_lst_cir(t_lst_cir *list_cir);

#endif