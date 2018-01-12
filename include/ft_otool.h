/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/21 18:31:20 by lfabbro           #+#    #+#             */
/*   Updated: 2018/01/12 19:07:42 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_OTOOL
# define FT_OTOOL

# include "libft.h"
# include "ft_common.h"

# include <sys/mman.h>
# include <mach-o/loader.h>
# include <mach-o/fat.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <errno.h>

# define ARCH_i386	" (architecture i386)"

void		otool_handle_32(void *ptr, char *name);
void		otool_handle_64(void *ptr, char *name);
void		otool_handle_fat(void *ptr, char *name);

#endif
