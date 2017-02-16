/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_cir.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                              	                  +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/15 12:18:23 by lbopp             #+#    #+#             */
/*   Updated: 2017/02/15 12:18:26 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

void	add_next_lst_cir(t_lst_cir **cour, t_lst_cir *new)
{
	if (*cour == NULL)
		*cour = new;
	else
	{
		(*cour)->next->prev = new;
		new->next = (*cour)->next;
		new->prev = *cour;
		(*cour)->next = new;
	}
}

t_lst_cir	*create_lst_cir(char *content)
{
	t_lst_cir	*new;

	if (!(new = (t_lst_cir*)ft_memalloc(sizeof(t_lst_cir))))
		return (NULL);
	new->next = new;
	new->prev = new;
	new->select = 0;
	new->curseur = 0;
	new->content = ft_strdup(content);
	return (new);
}

void	ft_putlstcir_fd(t_lst_cir *lst_cir, int fd)
{
	t_lst_cir	*tmp;

	if (lst_cir == NULL)
		ft_putstr_fd("List is empty", 2);
	else
	{
		/*if (lst_cir->curseur == 1)
			printf("\033[4m\n");
		if (lst_cir->select == 1)
			printf("\033[7m\n");*/
		ft_putstr_fd(lst_cir->content, fd);
		tmp = lst_cir->prev;
		while (tmp != lst_cir)
		{
			tputs(tgetstr("cr", NULL), 1, &test);
			/*if (tmp->curseur == 1)
				printf("\033[4m\n");
			if (lst_cir->select == 1)
				printf("\033[7m\n");*/
			//tputs(tgetstr("ll", NULL), 1, &test);
			ft_putstr_fd(tmp->content, fd);
			//printf("\033[0m\n");
			tmp = tmp->prev;
		}
	}
}