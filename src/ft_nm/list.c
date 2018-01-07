/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/06 18:23:17 by lfabbro           #+#    #+#             */
/*   Updated: 2018/01/07 16:56:42 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void	free_list(t_strtab *slist)
{
	t_strtab	*ptr;
	t_strtab	*tmp;

	ptr = slist;
	while (ptr)
	{
		tmp = ptr->next;
		free(ptr->strx);
		free(ptr);
		ptr = tmp;
	}
}

int		list_push(t_strtab **slist, struct nlist_64 symtab, char *strtab)
{
	t_strtab	*new;

	if (ft_strncmp(RADR, strtab + symtab.n_un.n_strx, ft_strlen(RADR)) == 0)
		return (EXIT_SUCCESS);
	if ((new = malloc(sizeof(t_strtab))) == NULL)
		return (EXIT_FAILURE);
	new->next = *slist;
	new->strx = ft_strdup(strtab + symtab.n_un.n_strx);
	new->type = symtab.n_type;
	new->sect = symtab.n_sect;
	new->desc = symtab.n_desc;
	new->value = symtab.n_value;
	*slist = new;
	return (EXIT_SUCCESS);
}

void	sorted_insert(t_strtab **slist, t_strtab *new)
{
	t_strtab	*ptr;

	if (*slist == NULL || ft_strcmp((*slist)->strx, new->strx) >= 0)
	{
		new->next = *slist;
		*slist = new;
	}
	else
	{
		ptr = *slist;
		while (ptr->next && ft_strcmp(ptr->next->strx, new->strx) < 0)
			ptr = ptr->next;
		new->next = ptr->next;
		ptr->next = new;
	}
}

void	insertion_sort(t_strtab **slist)
{
	t_strtab	*sorted;
	t_strtab	*ptr;
	t_strtab	*tmp;

	sorted = NULL;
	ptr = *slist;
	while (ptr)
	{
		tmp = ptr->next;
		sorted_insert(&sorted, ptr);
		ptr = tmp;
	}
	*slist = sorted;
}
