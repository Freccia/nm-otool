/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/06 18:23:17 by lfabbro           #+#    #+#             */
/*   Updated: 2018/01/23 16:36:25 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void		free_list(t_strtab *slist)
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

static int	list_skip(struct nlist_64 *symtab, char *strtab)
{
	if (ft_strncmp(RADR, strtab + symtab->n_un.n_strx, ft_strlen(RADR)) == 0)
		return (1);
	if (symtab->n_type & N_STAB)
		return (1);
	return (0);
}

int			list_push(t_strtab **slist, struct nlist_64 *symtab, char *strtab)
{
	t_strtab	*new;

	if (list_skip(symtab, strtab))
		return (EXIT_SUCCESS);
	if ((new = malloc(sizeof(**slist))) == NULL)
		return (EXIT_FAILURE);
	new->next = *slist;
	if (strtab + symtab->n_un.n_strx != NULL)
		new->strx = ft_strdup(strtab + symtab->n_un.n_strx);
	else
		new->strx = NULL;
	new->type = symtab->n_type;
	new->sect = symtab->n_sect;
	new->desc = symtab->n_desc;
	new->value = (uint64_t)symtab->n_value;
	*slist = new;
	return (EXIT_SUCCESS);
}

static void	sorted_insert_by_number(t_strtab **slist, t_strtab *new)
{
	t_strtab	*ptr;

	ft_printf("NUMBER\n");
	if (*slist == NULL || new->value >= (*slist)->value)
	{
		new->next = *slist;
		*slist = new;
	}
	else
	{
		ptr = *slist;
		while (ptr->next && new->value < (*slist)->value)
			ptr = ptr->next;
		new->next = ptr->next;
		ptr->next = new;
	}
}

static void	sorted_insert_by_name(t_strtab **slist, t_strtab *new)
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

void		insertion_sort(t_strtab **slist, int options)
{
	t_strtab	*sorted;
	t_strtab	*ptr;
	t_strtab	*tmp;

	sorted = NULL;
	ptr = *slist;
	while (ptr)
	{
		tmp = ptr->next;
		if (options & SORT_BY_NUM)
			sorted_insert_by_number(&sorted, ptr);
		else
			sorted_insert_by_name(&sorted, ptr);
		ptr = tmp;
	}
	*slist = sorted;
}
