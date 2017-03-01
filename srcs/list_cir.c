/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_cir.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/20 16:32:52 by lbopp             #+#    #+#             */
/*   Updated: 2017/02/20 16:32:56 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

void		add_next_lst_cir(t_lst_cir **cour, t_lst_cir *new, int len_max)
{
	if (*cour == NULL)
		*cour = new;
	else
	{
		(*cour)->next->prev = new;
		new->next = (*cour)->next;
		new->prev = *cour;
		new->select = 0;
		new->curseur = 0;
		new->size_max = len_max;
		(*cour)->next = new;
	}
}

t_lst_cir	*create_lst_cir(char *content, int len_max)
{
	t_lst_cir	*new;

	if (!(new = (t_lst_cir*)ft_memalloc(sizeof(t_lst_cir))))
		return (NULL);
	new->next = new;
	new->prev = new;
	new->select = 0;
	new->curseur = 0;
	new->size_max = len_max;
	new->content = ft_strdup(content);
	return (new);
}

t_lst_cir	*del_one_lst_cir(t_lst_cir *list_cir)
{
	if (list_cir == NULL)
		return (NULL);
	list_cir->next->prev = list_cir->prev;
	list_cir->prev->next = list_cir->next;
	list_cir->prev->curseur = 1;
	free(list_cir->content);
	list_cir->content = NULL;
	free(list_cir);
	list_cir = NULL;
	return (list_cir);
}

void		print_select_lst_cir(t_lst_cir *list_cir)
{
	t_lst_cir	*tmp;
	static int	i = 0;

	if (list_cir->select)
	{
		ft_putstr(list_cir->content);
		i++;
	}
	tmp = list_cir->prev;
	while (tmp != list_cir)
	{
		if (tmp->select)
		{
			if (i != 0)
				write(1, " ", 1);
			ft_putstr(tmp->content);
			i++;
		}
		tmp = tmp->prev;
	}
}
