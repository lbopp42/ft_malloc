/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/17 10:47:29 by lbopp             #+#    #+#             */
/*   Updated: 2019/01/17 11:15:23 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

char	hex_digit(int v)
{
    if (v >= 0 && v < 10)
	return '0' + v;
    else
	return 'a' + v - 10;
}

void	print_mem(void *p0)
{
    int i;
    uintptr_t p;

    p = (uintptr_t)p0;
    i = (sizeof(p) << 3) - 4;
    while (i >= 0)
    {
	ft_putchar(hex_digit((p >> i) & 0xf));
	i -= 4;
    }
}

void	show_alloc_mem(void)
{
    t_page	*page;
    t_meta	*origin;
    size_t	total;
    int		i;

    total = 0;
    i = 0;
    while (i < 3)
    {
	page = g_zone[i].page;	
	while (page)
	{
	    if (i == 0)
		ft_putstr("TINY : ");
	    else if (i == 1)
		ft_putstr("SMALL : ");
	    else
		ft_putstr("LARGE : ");
	    print_mem(page);
	    ft_putchar('\n');
	    origin = (t_meta*)((char*)page + align_size(sizeof(t_page*)));
	    while (origin && !origin->is_free)
	    {
		print_mem((char*)origin + sizeof(t_meta));
		ft_putstr(" - ");
		print_mem((char*)origin + sizeof(t_meta) + origin->size);
		ft_putstr(" : ");
		ft_putnbr(origin->size);
		ft_putendl(" octets");
		total += origin->size;
		origin = origin->next;
	    }
	    page = page->next;
	}
	i++;
    }
    ft_putstr("Total : ");
    ft_putnbr(total);
    ft_putendl(" octects");
}

