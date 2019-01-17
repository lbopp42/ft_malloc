/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/17 10:47:29 by lbopp             #+#    #+#             */
/*   Updated: 2019/01/17 14:57:35 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

char	hex_digit(int v)
{
	if (v >= 0 && v < 10)
		return ('0' + v);
	else
		return ('a' + v - 10);
}

void	print_mem(void *p0)
{
	int			i;
	uintptr_t	p;

	p = (uintptr_t)p0;
	i = (sizeof(p) << 3) - 4;
	while (i >= 0)
	{
		ft_putchar(hex_digit((p >> i) & 0xf));
		i -= 4;
	}
}

void	print_each_data(const t_page *page, size_t *total)
{
	t_meta	*data;

	data = (t_meta*)((char*)page + align_size(sizeof(t_page*)));
	while (data && !data->is_free)
	{
		print_mem((char*)data + sizeof(t_meta));
		ft_putstr(" - ");
		print_mem((char*)data + sizeof(t_meta) + data->size);
		ft_putstr(" : ");
		ft_putnbr(data->size);
		ft_putendl(" octets");
		*total += data->size;
		data = data->next;
	}
}

void	print_type(int type)
{
	if (type == 0)
		ft_putstr("TINY : ");
	else if (type == 1)
		ft_putstr("SMALL : ");
	else
		ft_putstr("LARGE : ");
}

void	show_alloc_mem(void)
{
	t_page	*page;
	size_t	total;
	int		type;

	total = 0;
	type = -1;
	while (++type < 3)
	{
		page = g_zone[type].page;
		while (page)
		{
			print_type(type);
			print_mem(page);
			ft_putchar('\n');
			print_each_data(page, &total);
			page = page->next;
		}
	}
	ft_putstr("Total : ");
	ft_putnbr(total);
	ft_putendl(" octects");
}
