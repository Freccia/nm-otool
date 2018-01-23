/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_fat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/04 17:43:49 by lfabbro           #+#    #+#             */
/*   Updated: 2018/01/23 15:13:22 by lfabbro          ###   ########.fr       */
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

static int		has_64(void *ptr, char *name, struct fat_header *header,
		struct fat_arch *arch_ptr)
{
	struct mach_header	*mach_header;
	uint32_t			i;

	i = 0;
	while (i < header->nfat_arch)
	{
		if (arch_ptr->cputype == CPU_TYPE_X86_64)
		{
			mach_header = (void *)ptr + arch_ptr->offset;
			if (mach_header->magic == FT_ARMAG)
				nm_handle_archive(mach_header, name);
			else
				nm_handle_64(mach_header);
			return (1);
		}
		arch_ptr = (struct fat_arch *)((char *)arch_ptr + sizeof(*arch_ptr));
		++i;
	}
	return (0);
}

int				nm_handle_fat(void *ptr, char *name)
{
	struct fat_header		*header;
	struct fat_arch			*arch_ptr;
	uint32_t				i;

	i = 0;
	header = (struct fat_header *)ptr;
	arch_ptr = (struct fat_arch *)((char *)header + sizeof(*header));
	if (should_swap_bytes_fat(header->magic))
		swap_fat(header, arch_ptr);
	if (has_64(ptr, name, header, arch_ptr) == 1)
		return (EXIT_SUCCESS);
	else
	{
		while (i < header->nfat_arch)
		{
			if (arch_ptr->cputype == CPU_TYPE_I386)
				nm_handle_32((void *)ptr + arch_ptr->offset);
			else
				put_not_handled(arch_ptr->cputype);
			arch_ptr = (struct fat_arch *)((char*)arch_ptr + sizeof(*arch_ptr));
			++i;
		}
	}
	return (EXIT_SUCCESS);
}
