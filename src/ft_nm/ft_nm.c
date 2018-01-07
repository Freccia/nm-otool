/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/21 18:04:12 by lfabbro           #+#    #+#             */
/*   Updated: 2018/01/08 00:26:09 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int			error(char *str)
{	
	ft_printfd(2, "%s\n", str);
	return (EXIT_FAILURE);
}

int			usage(char *name)
{
	ft_printfd(2, "Usage: %s /path/to/exec\n", name);
	return (EXIT_FAILURE);
}

int			should_swap_bytes_fat(uint32_t magic)
{
	return (magic == FAT_CIGAM || magic == FAT_CIGAM_64);
}

int			should_swap_bytes(uint32_t magic)
{
	return (magic == MH_CIGAM || magic == MH_CIGAM_64);
}

// TODO Should swap bytes?  --> swap all mmapped memory CIGAM
//TODO handle fat 64
void		ft_nm(char *ptr)
{
	uint32_t	magic_number;

	magic_number = *(uint32_t*)ptr;
	if (magic_number == MH_MAGIC_64 || magic_number == MH_CIGAM_64)
	{
		handle_64(ptr);
	}
	else if (magic_number == MH_MAGIC || magic_number == MH_CIGAM)
	{
		handle_32(ptr);
	}
	else if (magic_number == FAT_MAGIC_64 || magic_number == FAT_CIGAM_64)
	{
		handle_fat(ptr);
	}
	else if (magic_number == FAT_MAGIC || magic_number == FAT_CIGAM)
	{
		handle_fat(ptr);
	}
}

int			main(int ac, char **av)
{
	int			fd;
	char		*ptr;
	struct stat	buf;

	// TODO parse options
	if (ac < 2 && (fd = open("a.out", O_RDONLY)) < 0)
		return (usage(av[0]));
	else if (ac >= 2 && (fd = open(av[1], O_RDONLY)) < 0)
		// TODO missing file || noperm ???
		return (error("fd opening failed"));
	if (fstat(fd, &buf) < 0)
		return (error("fstat failed"));
	if ((ptr = mmap(0, buf.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		return (error("mmap failed"));
	ft_nm(ptr);
	if (munmap(ptr, buf.st_size) < 0)
		return (error("munmap failed"));
	return (EXIT_SUCCESS);
}
