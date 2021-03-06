/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_fat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/12 18:47:21 by lfabbro           #+#    #+#             */
/*   Updated: 2018/01/15 19:00:28 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

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
	uint32_t	i;

	i = 0;
	while (i < header->nfat_arch)
	{
		if (arch_ptr->cputype == CPU_TYPE_X86_64)
		{
			otool_handle_64((void *)ptr + arch_ptr->offset, name);
			return (1);
		}
		arch_ptr = (struct fat_arch *)((char *)arch_ptr + sizeof(*arch_ptr));
		++i;
	}
	return (0);
}

static int		has_others(void *ptr, char *name,
		struct fat_header *header, struct fat_arch *arch_ptr)
{
	char					*name_arch;
	uint32_t				i;

	name_arch = NULL;
	i = 0;
	while (i < header->nfat_arch)
	{
		if (arch_ptr->cputype == CPU_TYPE_I386)
		{
			if ((name_arch =
				calloc(1, ft_strlen(name) + ft_strlen(ARCH_I386) + 1)) == NULL)
				return (1);
			name_arch = ft_strncpy(name_arch, name, ft_strlen(name));
			name_arch = ft_strcat(name_arch, ARCH_I386);
			otool_handle_32((void *)ptr + arch_ptr->offset, name_arch);
			free(name_arch);
		}
		else
			put_not_handled(arch_ptr->cputype);
		arch_ptr = (struct fat_arch *)((char *)arch_ptr + sizeof(*arch_ptr));
		++i;
	}
	return (0);
}

void			otool_handle_fat(void *ptr, char *name)
{
	struct fat_header		*header;
	struct fat_arch			*arch_ptr;

	header = (struct fat_header *)ptr;
	arch_ptr = (struct fat_arch *)((char *)header + sizeof(*header));
	if (should_swap_bytes_fat(header->magic))
		swap_fat(header, arch_ptr);
	if (has_64(ptr, name, header, arch_ptr) == 1)
		return ;
	else
		has_others(ptr, name, header, arch_ptr);
}
