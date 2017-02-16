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

void		add_next_lst_cir(t_lst_cir **cour, t_lst_cir *new)
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