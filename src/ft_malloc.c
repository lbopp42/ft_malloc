/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/12 13:01:18 by lbopp             #+#    #+#             */
/*   Updated: 2018/12/12 17:30:38 by lbopp            ###   ########.fr       */
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

char hex_digit(int v) {
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

	//page = g_zone[0].page;
	//while (page)
	//{
	//	ft_putstr("TINY : ");
	//	print_mem(page);
	//	ft_putchar('\n');
	//	origin = (t_meta*)((char*)page + align_size(sizeof(t_page*)));
	//	while (origin)
	//	{
	//		print_mem(origin);
	//		ft_putstr(" - ");
	//		print_mem((char*)origin + origin->size);
	//		ft_putstr(" : ");
	//		ft_putnbr(origin->size);
	//		ft_putendl(" octets");
	//		ft_putstr("free : ");
	//		ft_putnbr(origin->is_free);
	//		ft_putendl("");
	//		origin = origin->next;
	//	}
	//	page = page->next;
	//}
	//page = g_zone[1].page;
	//while (page)
	//{
	//	ft_putstr("SMALL : ");
	//	print_mem(page);
	//	ft_putchar('\n');
	//	origin = (t_meta*)((char*)page + align_size(sizeof(t_page*)));
	//	while (origin)
	//	{
	//		print_mem(origin);
	//		ft_putstr(" - ");
	//		print_mem((char*)origin + origin->size);
	//		ft_putstr(" : ");
	//		ft_putnbr(origin->size);
	//		ft_putendl(" octets");
	//		ft_putstr("free : ");
	//		ft_putnbr(origin->is_free);
	//		ft_putendl("");
	//		origin = origin->next;
	//	}
	//	page = page->next;
	//}
	page = g_zone[2].page;
	while (page)
	{
		ft_putstr("LARGE : ");
		print_mem(page);
		ft_putchar('\n');
		origin = (t_meta*)(page + 1);
		while (origin)
		{
			print_mem(origin);
			ft_putstr(" - ");
			print_mem((char*)origin + origin->size);
			ft_putstr(" : ");
			ft_putnbr(origin->size);
			ft_putendl(" octets");
			ft_putstr("free : ");
			ft_putnbr(origin->is_free);
			ft_putendl("");
			origin = origin->next;
		}
		page = page->next;
	}
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

void	init_data(t_meta *data, size_t page_size)
{
	data->size = page_size;
	data->is_free = 1;
	data->prev = NULL;
	data->next = NULL;
}

t_page	*create_page(size_t page_size, t_page *next_page)
{
	t_meta	*data;
	t_page	*new_page;

	new_page = (t_page*)mmap(0, round_page_size(page_size + sizeof(t_meta) + sizeof(t_page)), PROT_READ | PROT_WRITE,
			MAP_ANON | MAP_PRIVATE, -1, 0);
	new_page->next = next_page;
	data = (t_meta*)(new_page + 1);
	init_data(data, round_page_size(page_size + sizeof(t_meta) + sizeof(t_page)) - sizeof(t_meta));
	return (new_page);
}

//void	*find_place(t_page *page, size_t size_wanted)
//{
//	t_meta	*data;
//
//	if (!page)
//		page = create_page(size_wanted, NULL);
//	data = (t_meta*)(page + 1);
//	while (data)
//	{
//		if (data->is_free && data->size >= size_wanted) // Pour les larges
//		{
//			data->is_free = 0;
//			data->size = size_wanted + sizeof(t_meta);
//			return ((char*)data + sizeof(t_meta));
//		}
//		else
//			ft_putendl("ON ENTRE PAS");
//		data = data->next;
//	}
//	return (find_place(page->next, size_wanted));
//}

void	*malloc(size_t size)
{
	t_page	*origin;
	t_meta	*data;

	ft_putendl("MALLOC = ");
	ft_putnbr(size);
	ft_putendl("");
	origin = g_zone[2].page;
	if (g_zone[2].page == NULL)
		g_zone[2].page = create_page(align_size(size), NULL);
	else
	{
		while (g_zone[2].page->next)
			g_zone[2].page = g_zone[2].page->next;
		g_zone[2].page->next = create_page(align_size(size), NULL);
	}
	data = (t_meta*)(g_zone[2].page + 1);
	data->is_free = 0;
	ft_putendl("===================  BEFORE  ==========================================");
	show_alloc_mem();
	ft_putendl("========================================================================");
	sleep(2);
	g_zone[2].page = origin;
	return (data + sizeof(t_meta));
}

void	free(void *ptr)
{
	(void)ptr;
	return ;
}

void	*realloc(void *ptr, size_t size)
{
	//ft_putendl("On use REALLOC");
	(void)ptr;
	return (malloc(size));
}
