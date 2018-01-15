/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_archive.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/15 17:28:28 by lfabbro           #+#    #+#             */
/*   Updated: 2018/01/15 18:59:54 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

static void		ft_otool_archive(void *ptr, char *name)
{
	uint32_t	magic_number;

	magic_number = *(uint32_t *)ptr;
	if (magic_number == MH_MAGIC_64 || magic_number == MH_CIGAM_64)
	{
		otool_handle_64(ptr, name);
	}
	else if (magic_number == MH_MAGIC || magic_number == MH_CIGAM)
	{
		otool_handle_32(ptr, name);
	}
	else if (magic_number == FAT_MAGIC_64 || magic_number == FAT_CIGAM_64 ||
			magic_number == FAT_MAGIC || magic_number == FAT_CIGAM)
	{
		otool_handle_fat(ptr, name);
	}
	else
		error("The file was not recognized as a valid object file\n");
}

static char		*object_name(char *name, char *obj_name)
{
	char	*str;
	int		len;

	len = ft_strlen(name) + ft_strlen(obj_name) + 3;
	if ((str = malloc(sizeof(*str) + len)) == NULL)
		return (NULL);
	str = ft_strcpy(str, name);
	str = ft_strcat(str, "(");
	str = ft_strcat(str, obj_name);
	str = ft_strcat(str, ")");
	return (str);
}

static void		otool_handle_archive_bis(void *ptr, char *name, t_archive ar,
		t_quad q)
{
	char			*obj_name;

	obj_name = NULL;
	while (q.tot < ar.symtab_size)
	{
		if (q.tmp != ar.symtab[q.i])
		{
			ar.ar_obj = (struct ar_hdr *)(ptr + ar.symtab[q.i]);
			q.off = get_object_offset(ar.ar_obj);
			obj_name = (char *)((void *)ar.ar_obj + ARCHIVE_HEADER_SIZE);
			if ((obj_name = object_name(name, obj_name)) == NULL)
				return ;
			ft_otool_archive(((void *)ar.ar_obj + q.off), obj_name);
			q.tmp = ar.symtab[q.i];
			free(obj_name);
		}
		q.tot += sizeof(q.i) * 2;
		q.i += 2;
	}
}

void			otool_handle_archive(void *ptr, char *name)
{
	t_archive		ar;
	t_quad			q;

	ar.ar = (struct ar_hdr *)((uint8_t *)ptr + ARMAG_LEN);
	ar.symtab_offset = get_object_offset(ar.ar);
	ar.symtab_size = *(uint32_t *)(ptr + ar.symtab_offset + ARMAG_LEN);
	ar.symtab = (uint32_t *)(ptr + ar.symtab_offset + ARMAG_LEN + sizeof(int));
	q.tot = 0;
	q.i = 1;
	ft_printf("Archive : %s\n", name);
	otool_handle_archive_bis(ptr, name, ar, q);
}
