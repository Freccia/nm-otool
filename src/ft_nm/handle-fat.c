/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle-fat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/04 17:43:49 by lfabbro           #+#    #+#             */
/*   Updated: 2018/01/08 00:24:42 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int			handle_fat(void *ptr)
{
	struct fat_header		*header;
	struct fat_arch			*arch_ptr;
	uint32_t				i;

	i = 0;
	header = (struct fat_header*)ptr;
	if (should_swap_bytes_fat(header->magic))
		swap_fat_header((struct fat_header*)ptr);
	while (i < header->nfat_arch)
	{
		arch_ptr = (struct fat_arch*)((char*)header + sizeof(*header));
		ft_printf("cputype: %x\n", arch_ptr->cputype);
		ft_printf("nfat: %x\n", header->nfat_arch);
		++i;
	}
	return (EXIT_SUCCESS);
}
