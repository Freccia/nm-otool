/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/21 18:04:46 by lfabbro           #+#    #+#             */
/*   Updated: 2018/01/29 18:26:45 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

static void		ft_otool_bis(void *ptr, char *name)
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

static int		ft_otool(char *name)
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
	ft_otool_bis(ptr, name);
	if (munmap(ptr, buf.st_size) < 0 || close(fd) < 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int				main(int ac, char **av)
{
	int			i;
	int			ret;
	int			fd;

	i = 1;
	ret = EXIT_SUCCESS;
	if (ac < 2)
	{
		if ((fd = open("a.out", O_RDONLY)) < 0)
			return (usage_nm(av[0]));
		close(fd);
		ft_otool("a.out");
	}
	else
	{
		while (i < ac)
		{
			if (ft_otool(av[i]) == EXIT_FAILURE)
				ret = EXIT_FAILURE;
			++i;
		}
	}
	return (ret);
}
