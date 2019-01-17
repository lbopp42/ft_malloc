/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/12 13:01:18 by lbopp             #+#    #+#             */
/*   Updated: 2019/01/17 10:33:51 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"
#define DEBUG 0

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

void	print_mem_fd(void *p0, int fd)
{
    int i;
    uintptr_t p;

    p = (uintptr_t)p0;
    i = (sizeof(p) << 3) - 4;
    while (i >= 0)
    {
	ft_putchar_fd(hex_digit((p >> i) & 0xf), fd);
	i -= 4;
    }
}

void	show_alloc_mem(void)
{
    t_page	*page;
    t_meta	*origin;
    size_t	total;

    total = 0;
    page = g_zone[0].page;
    while (page)
    {
	ft_putstr("TINY : ");
	print_mem(page);
	ft_putchar('\n');
	origin = (t_meta*)((char*)page + align_size(sizeof(t_page*)));
	while (origin)
	{
	    print_mem((char*)origin + sizeof(t_meta));
	    ft_putstr(" - ");
	    print_mem((char*)origin + sizeof(t_meta) + origin->size);
	    ft_putstr(" : ");
	    ft_putnbr(origin->size);
	    ft_putendl(" octets");
	    ft_putstr("free : ");
	    ft_putnbr(origin->is_free);
	    ft_putendl("");
	    total += origin->size;
	    origin = origin->next;
	}
	page = page->next;
    }
    page = g_zone[1].page;
    while (page)
    {
	ft_putstr("SMALL : ");
	print_mem(page);
	ft_putchar('\n');
	origin = (t_meta*)((char*)page + align_size(sizeof(t_page*)));
	while (origin)
	{
	    print_mem((char*)origin + sizeof(t_meta));
	    ft_putstr(" - ");
	    print_mem((char*)origin + sizeof(t_meta) + origin->size);
	    ft_putstr(" : ");
	    ft_putnbr(origin->size);
	    ft_putendl(" octets");
	    ft_putstr("free : ");
	    ft_putnbr(origin->is_free);
	    ft_putendl("");
	    total += origin->size;
	    origin = origin->next;
	}
	page = page->next;
    }
    page = g_zone[2].page;
    while (page)
    {
	ft_putstr("LARGE : ");
	print_mem(page);
	ft_putchar('\n');
	origin = (t_meta*)((char*)page + align_size(sizeof(t_page*)));
	while (origin)
	{
	    print_mem((char*)origin + sizeof(t_meta));
	    ft_putstr(" - ");
	    print_mem((char*)origin + sizeof(t_meta) + origin->size);
	    ft_putstr(" : ");
	    ft_putnbr(origin->size);
	    ft_putendl(" octets");
	    ft_putstr("free : ");
	    ft_putnbr(origin->is_free);
	    ft_putendl("");
	    total += origin->size;
	    origin = origin->next;
	}
	page = page->next;
    }
    ft_putstr("Total : ");
    ft_putnbr(total);
    ft_putendl(" octects");
}

void	show_alloc_mem_fd(int fd)
{
    t_page	*page;
    t_meta	*origin;
    size_t	total;

    total = 0;
    page = g_zone[0].page;
    while (page)
    {
	ft_putstr_fd("TINY : ", fd);
	print_mem_fd(page, fd);
	ft_putchar_fd('\n', fd);
	origin = (t_meta*)((char*)page + align_size(sizeof(t_page*)));
	while (origin)
	{
	    print_mem_fd((char*)origin + sizeof(t_meta), fd);
	    ft_putstr_fd(" - ", fd);
	    print_mem_fd((char*)origin + sizeof(t_meta) + origin->size, fd);
	    ft_putstr_fd(" : ", fd);
	    ft_putnbr_fd(origin->size, fd);
	    ft_putendl_fd(" octets", fd);
	    ft_putstr_fd("free : ", fd);
	    ft_putnbr_fd(origin->is_free, fd);
	    ft_putendl_fd("", fd);
	    total += origin->size;
	    origin = origin->next;
	}
	page = page->next;
    }
    page = g_zone[1].page;
    while (page)
    {
	ft_putstr_fd("SMALL : ", fd);
	print_mem_fd(page, fd);
	ft_putchar_fd('\n', fd);
	origin = (t_meta*)((char*)page + align_size(sizeof(t_page*)));
	while (origin)
	{
	    print_mem_fd((char*)origin + sizeof(t_meta), fd);
	    ft_putstr_fd(" - ", fd);
	    print_mem_fd((char*)origin + sizeof(t_meta) + origin->size, fd);
	    ft_putstr_fd(" : ", fd);
	    ft_putnbr_fd(origin->size, fd);
	    ft_putendl_fd(" octets", fd);
	    ft_putstr_fd("free : ", fd);
	    ft_putnbr_fd(origin->is_free, fd);
	    ft_putendl_fd("", fd);
	    total += origin->size;
	    origin = origin->next;
	}
	page = page->next;
    }
    page = g_zone[2].page;
    while (page)
    {
	ft_putstr_fd("LARGE : ", fd);
	print_mem_fd(page, fd);
	ft_putchar_fd('\n', fd);
	origin = (t_meta*)((char*)page + align_size(sizeof(t_page*)));
	while (origin)
	{
	    print_mem_fd((char*)origin + sizeof(t_meta), fd);
	    ft_putstr_fd(" - ", fd);
	    print_mem_fd((char*)origin + sizeof(t_meta) + origin->size, fd);
	    ft_putstr_fd(" : ", fd);
	    ft_putnbr_fd(origin->size, fd);
	    ft_putendl_fd(" octets", fd);
	    ft_putstr_fd("free : ", fd);
	    ft_putnbr_fd(origin->is_free, fd);
	    ft_putendl_fd("", fd);
	    total += origin->size;
	    origin = origin->next;
	}
	page = page->next;
    }
    ft_putstr_fd("Total : ", fd);
    ft_putnbr_fd(total, fd);
    ft_putendl_fd(" octects", fd);
}

void	verif_data(void)
{
    t_page	*page;
    t_meta	*origin;
    t_meta	*prev;

    page = g_zone[0].page;
    while (page)
    {
	prev = NULL;
	origin = (t_meta*)((char*)page + align_size(sizeof(t_page*)));
	while (origin)
	{
	    if (origin->prev != prev)
	    {
		ft_putendl_fd("HERE", 3);
		print_mem_fd((char*)origin + sizeof(t_meta), 3);
		ft_putchar_fd('\n', 3);
		print_mem_fd((char*)origin->prev + sizeof(t_meta), 3);
		ft_putchar_fd('\n', 3);
		print_mem_fd((char*)prev + sizeof(t_meta), 3);
		ft_putchar_fd('\n', 3);
	    }
	    prev = origin;
	    origin = origin->next;
	}
	page = page->next;
    }
    page = g_zone[1].page;
    while (page)
    {
	prev = NULL;
	origin = (t_meta*)((char*)page + align_size(sizeof(t_page*)));
	while (origin)
	{
	    if (origin->prev != prev)
	    {
		ft_putendl_fd("HERE", 3);
		print_mem_fd((char*)origin + sizeof(t_meta), 3);
		ft_putchar_fd('\n', 3);
		print_mem_fd((char*)origin->prev + sizeof(t_meta), 3);
		ft_putchar_fd('\n', 3);
		print_mem_fd((char*)prev + sizeof(t_meta), 3);
		ft_putchar_fd('\n', 3);
	    }
	    prev = origin;
	    origin = origin->next;
	}
	page = page->next;
    }
    page = g_zone[2].page;
    while (page)
    {
	prev = NULL;
	origin = (t_meta*)((char*)page + align_size(sizeof(t_page*)));
	while (origin)
	{
	    if (origin->prev != prev)
	    {
		ft_putendl_fd("HERE", 3);
		print_mem_fd((char*)origin + sizeof(t_meta), 3);
		ft_putchar_fd('\n', 3);
		print_mem_fd((char*)origin->prev + sizeof(t_meta), 3);
		ft_putchar_fd('\n', 3);
		print_mem_fd((char*)prev + sizeof(t_meta), 3);
		ft_putchar_fd('\n', 3);
	    }
	    prev = origin;
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

void	init_data(t_meta **data, size_t page_size)
{
    (*data)->size = page_size;
    (*data)->is_free = 1;
    (*data)->prev = NULL;
    (*data)->next = NULL;
}

t_page	*create_page(size_t page_size)
{
    t_meta	*data;
    t_page	*new_page;

    new_page = (t_page*)mmap(0, round_page_size(page_size + sizeof(t_meta) + align_size(sizeof(t_page))), PROT_READ | PROT_WRITE,
	    MAP_ANON | MAP_PRIVATE, -1, 0);
    if (new_page == MAP_FAILED)
    {
	ft_putendl("FAIL MAP"); //TODO DEBUG
	exit(1);
    }
    new_page->next = NULL;
    data = (t_meta*)((char*)new_page + align_size(sizeof(t_page)));
    init_data(&data, round_page_size(page_size + sizeof(t_meta) + align_size(sizeof(t_page))) - align_size(sizeof(t_page)) - sizeof(t_meta));
    return (new_page);
}

void	*fill_place(t_meta *data, size_t size_wanted, size_t page_size)
{
    t_meta	*next_tmp;

    next_tmp = data->next;
    if ((data->size >= size_wanted + sizeof(t_meta) + ALIGN &&
		page_size == align_size(N) - sizeof(t_meta) - align_size(sizeof(t_page))) ||
	    (data->size >= size_wanted + sizeof(t_meta) + TINY + 1 &&
	     page_size == align_size(M) - sizeof(t_meta) - align_size(sizeof(t_page))))
    {
	data->next = (t_meta*)((char*)data + size_wanted + sizeof(t_meta));
	data->next->is_free = 1;
	data->next->size = data->size - size_wanted - sizeof(t_meta);
	data->next->prev = data;
	data->next->next = next_tmp;
	if (next_tmp)
	    data->next->next->prev = data->next;
	data->size = size_wanted;
    }
    data->is_free = 0;
    return ((char*)data + sizeof(t_meta));
}

void	*find_place(t_page **page, size_t size_wanted, size_t page_size)
{
    t_meta	*data;

    if (!*page)
	*page = create_page(page_size);
    data = (t_meta*)((char*)*page + align_size(sizeof(t_page)));
    while (data)
    {
	if (data->is_free && data->size >= size_wanted)
	    return (fill_place(data, size_wanted, page_size));
	data = data->next;
    }
    return (find_place(&(*page)->next, size_wanted, page_size));
}

void	*malloc(size_t size)
{
    t_page	*origin;
    t_meta	*data;

    if (size <= TINY)
	return (find_place(&g_zone[0].page, align_size(size),
		    align_size(N) - sizeof(t_meta) - align_size(sizeof(t_page))));
    if (size <= SMALL)
	return (find_place(&g_zone[1].page, align_size(size),
		    align_size(M) - sizeof(t_meta) - align_size(sizeof(t_page))));
    if (!g_zone[2].page)
    {
	g_zone[2].page = create_page(align_size(size));
	data = (t_meta*)((char*)g_zone[2].page + align_size(sizeof(t_page)));
    }
    else
    {
	origin = g_zone[2].page;
	while (g_zone[2].page->next)
	    g_zone[2].page = g_zone[2].page->next;
	g_zone[2].page->next = create_page(align_size(size));
	data = (t_meta*)((char*)g_zone[2].page->next + align_size(sizeof(t_page)));
	g_zone[2].page = origin;
    }
    data->is_free = 0;
    return ((char*)data + sizeof(t_meta));
}
