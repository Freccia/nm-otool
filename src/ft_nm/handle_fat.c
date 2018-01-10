/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_fat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/04 17:43:49 by lfabbro           #+#    #+#             */
/*   Updated: 2018/01/10 21:52:30 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static void		put_not_handled(cpu_type_t cputype)
{
	char	cpu[15];

	ft_bzero(&cpu, 15);
	if (cputype == CPU_TYPE_POWERPC || cputype == CPU_TYPE_POWERPC64)
		ft_strcpy(cpu, "PPC || PPC64");
	else if (cputype == CPU_TYPE_ARM || cputype == CPU_TYPE_ARM64)
		ft_strcpy(cpu, "ARM || ARM64");
	else if (cputype == CPU_TYPE_SPARC)
		ft_strcpy(cpu, "SPARC");
	else if (cputype == CPU_TYPE_I860)
		ft_strcpy(cpu, "I860");
	else if (cputype == CPU_TYPE_HPPA)
		ft_strcpy(cpu, "HPPA");
	else if (cputype == CPU_TYPE_MC680x0)
		ft_strcpy(cpu, "MC680x0");
	else if (cputype == CPU_TYPE_MC98000)
		ft_strcpy(cpu, "MC98000");
	else if (cputype == CPU_TYPE_MC88000)
		ft_strcpy(cpu, "MC88000");
	else
		ft_strcpy(cpu, "Unknown");
	ft_printf("CPU architecture %s not handled.\n\n", cpu);
}

static int		has_64(void *ptr, struct fat_header *header,
		struct fat_arch *arch_ptr)
{
	uint32_t	i;

	i = 0;
	while (i < header->nfat_arch)
	{
		if (arch_ptr->cputype == CPU_TYPE_X86_64)
		{
			handle_64((void *)ptr + arch_ptr->offset);
			return (1);
		}
		arch_ptr = (struct fat_arch *)((char *)arch_ptr + sizeof(*arch_ptr));
		++i;
	}
	return (0);
}

int				handle_fat(void *ptr)
{
	struct fat_header		*header;
	struct fat_arch			*arch_ptr;
	uint32_t				i;

	i = 0;
	header = (struct fat_header *)ptr;
	arch_ptr = (struct fat_arch *)((char *)header + sizeof(*header));
	if (should_swap_bytes_fat(header->magic))
		swap_fat(header, arch_ptr);
	if (has_64(ptr, header, arch_ptr) == 1)
		return (EXIT_SUCCESS);
	else
	{
		while (i < header->nfat_arch)
		{
			if (arch_ptr->cputype == CPU_TYPE_I386)
				handle_32((void *)ptr + arch_ptr->offset);
			else
				put_not_handled(arch_ptr->cputype);
			arch_ptr = (struct fat_arch *)((char *)arch_ptr + sizeof(*arch_ptr));
			++i;
		}
	}
	return (EXIT_SUCCESS);
}
