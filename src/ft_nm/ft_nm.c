/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/21 18:04:12 by lfabbro           #+#    #+#             */
/*   Updated: 2018/01/30 15:48:16 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static void		ft_nm_bis(void *ptr, char *name, t_options opt)
{
	uint32_t	magic_number;

	magic_number = *(uint32_t*)ptr;
	if (magic_number == MH_MAGIC_64 || magic_number == MH_CIGAM_64)
	{
		nm_handle_64(ptr, opt);
	}
	else if (magic_number == MH_MAGIC || magic_number == MH_CIGAM)
	{
		nm_handle_32(ptr, opt);
	}
	else if (magic_number == FAT_MAGIC_64 || magic_number == FAT_CIGAM_64)
	{
		nm_handle_fat(ptr, opt);
	}
	else if (magic_number == FAT_MAGIC || magic_number == FAT_CIGAM)
	{
		nm_handle_fat(ptr, opt);
	}
	else if (magic_number == FT_ARMAG)
		nm_handle_archive(ptr, opt, name);
	else
		error("The file was not recognized as a valid object file\n");
}

static int		ft_nm(char *name, int8_t multi_arg, t_options opt)
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
	if (multi_arg)
		ft_printf("\n%s:\n", name);
	ft_nm_bis(ptr, name, opt);
	if (munmap(ptr, buf.st_size) < 0 || close(fd) < 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int				main(int ac, char **av)
{
	int			i;
	int			multi_arg;
	int			ret;
	t_options	opt;

	if ((i = get_options(ac, av, &opt)) == -1)
		return (usage_nm(av[0]));
	ret = EXIT_SUCCESS;
	if (ac < 2)
		ft_nm("a.out", 0, opt);
	else
	{
		multi_arg = ac - i > 1 ? 1 : 0;
		while (i < ac)
		{
			if (ft_nm(av[i], multi_arg, opt) == EXIT_FAILURE)
				ret = EXIT_FAILURE;
			++i;
		}
	}
	return (ret);
}
