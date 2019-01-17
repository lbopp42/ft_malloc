/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/12 13:01:18 by lbopp             #+#    #+#             */
/*   Updated: 2019/01/17 12:56:43 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"
#define DEBUG 0

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
