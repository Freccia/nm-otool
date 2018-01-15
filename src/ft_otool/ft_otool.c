/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/21 18:04:46 by lfabbro           #+#    #+#             */
/*   Updated: 2018/01/15 18:54:01 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

int				error(char *str)
{
	ft_printfd(2, "%s\n", str);
	return (EXIT_FAILURE);
}

int				usage(char *name)
{
	ft_printfd(2, "Usage: %s /path/to/binary\n", name);
	return (EXIT_FAILURE);
}

static void		ft_otool(void *ptr, char *name)
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
	else if (magic_number == FT_ARMAG)
		otool_handle_archive(ptr, name);
	else
		error("The file was not recognized as a valid object file\n");
}

int				main(int ac, char **av)
{
	int			fd;
	char		*ptr;
	struct stat	buf;

	if (ac < 2 && (fd = open("a.out", O_RDONLY)) < 0)
		return (usage(av[0]));
	else if (ac >= 2 && (fd = open(av[1], O_RDONLY)) < 0)
	{
		if (errno == EACCES)
			return (error("Permission denied"));
		return (error("fd opening failed"));
	}
	if (fstat(fd, &buf) < 0)
		return (error("fstat failed"));
	if (buf.st_mode & S_IFDIR)
		return (error("Is a directory"));
	if ((ptr = mmap(0, buf.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0))
			== MAP_FAILED)
		return (error("mmap failed"));
	if (ft_parse_binary(ptr, buf.st_size))
		return (error("The file was not recognized as a valid object file\n"));
	ft_otool(ptr, ac >= 2 ? av[1] : "a.out");
	if (munmap(ptr, buf.st_size) < 0)
		return (error("munmap failed"));
	return (EXIT_SUCCESS);
}
