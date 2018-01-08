/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle-fat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/04 17:43:49 by lfabbro           #+#    #+#             */
/*   Updated: 2018/01/08 14:27:34 by lfabbro          ###   ########.fr       */
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
	arch_ptr = (struct fat_arch*)((char*)header + sizeof(*header));
	if (should_swap_bytes_fat(header->magic))
	{
		swap_fat_header(header);
		swap_fat_arch(arch_ptr, header->nfat_arch);
	}
	while (i < header->nfat_arch)
	{
		if (arch_ptr->cputype == CPU_TYPE_X86_64)
			handle_64((void*)ptr + arch_ptr->offset);
		else if (arch_ptr->cputype == CPU_TYPE_I386)
			handle_32((void*)ptr + arch_ptr->offset);
		else
			ft_printf("CpuType %x not handled.\n", arch_ptr->cputype);
		arch_ptr = (struct fat_arch*)((char*)arch_ptr + sizeof(*arch_ptr));
		++i;
	}
	return (EXIT_SUCCESS);
}
