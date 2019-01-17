/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   align_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/17 12:55:22 by lbopp             #+#    #+#             */
/*   Updated: 2019/01/17 12:56:38 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

size_t	align_size(size_t init_size)
{
    if (init_size < ALIGN)
	return (ALIGN);
    else if (init_size % ALIGN == 0)
	return (init_size);
    else
	return (init_size + ALIGN - init_size % ALIGN);
}

size_t	round_page_size(size_t init_size)
{
    size_t	os_page_size;

    os_page_size = (size_t)getpagesize();
    if (init_size < os_page_size)
	return (os_page_size);
    else if (init_size % os_page_size == 0)
	return (init_size);
    else
	return (init_size + os_page_size - init_size % os_page_size);
}
