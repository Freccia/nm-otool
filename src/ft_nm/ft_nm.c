/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/21 18:04:12 by lfabbro           #+#    #+#             */
/*   Updated: 2018/01/23 15:42:08 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

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

static void		ft_nm_bis(void *ptr, char *name)
{
	uint32_t	magic_number;

	magic_number = *(uint32_t*)ptr;
	if (magic_number == MH_MAGIC_64 || magic_number == MH_CIGAM_64)
	{
		nm_handle_64(ptr);
	}
	else if (magic_number == MH_MAGIC || magic_number == MH_CIGAM)
	{
		nm_handle_32(ptr);
	}
	else if (magic_number == FAT_MAGIC_64 || magic_number == FAT_CIGAM_64)
	{
		nm_handle_fat(ptr);
	}
	else if (magic_number == FAT_MAGIC || magic_number == FAT_CIGAM)
	{
		nm_handle_fat(ptr);
	}
	else if (magic_number == FT_ARMAG)
		nm_handle_archive(ptr, name);
	else
		error("The file was not recognized as a valid object file\n");
}

static int		ft_nm(char *name)
{
	int			fd;
	struct stat	buf;
	char		*ptr;

	if ((fd = open(name, O_RDONLY)) < 0)
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
	ft_nm_bis(ptr, name);
	if (munmap(ptr, buf.st_size) < 0)
		return (error("munmap failed"));
	if (close(fd) < 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int				main(int ac, char **av)
{
	int			i;
	int			ret;
	struct stat	buf;

	i = 1;
	ret = EXIT_SUCCESS;
	if (ac < 2)
	{
		if (stat("a.out", &buf) < 0)
			return (usage(av[0]));
		ft_nm("a.out");
	}
	else
	{
		while (i < ac)
		{
			if (ft_nm(av[i]) == EXIT_FAILURE)
				ret = EXIT_FAILURE;
			++i;
		}
	}
	return (ret);
}
