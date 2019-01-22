/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/12 13:01:18 by lbopp             #+#    #+#             */
/*   Updated: 2019/01/22 10:42:22 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

pthread_mutex_t	g_mutex_stock = PTHREAD_MUTEX_INITIALIZER;

void	*fill_place(t_meta *data, size_t size_wanted, size_t page_size)
{
	t_meta	*next_tmp;
	size_t	tiny_page;
	size_t	small_page;

	tiny_page = align_size(N) - sizeof(t_meta) - align_size(sizeof(t_page));
	small_page = align_size(M) - sizeof(t_meta) - align_size(sizeof(t_page));
	next_tmp = data->next;
	if ((data->size >= size_wanted + sizeof(t_meta) + ALIGN &&
		page_size == tiny_page) ||
		(data->size >= size_wanted + sizeof(t_meta) + TINY + 1 &&
		page_size == small_page))
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
		if (!(*page = create_page(page_size)))
			return (NULL);
	data = (t_meta*)((char*)*page + align_size(sizeof(t_page)));
	while (data)
	{
		if (data->is_free && data->size >= size_wanted)
			return (fill_place(data, size_wanted, page_size));
		data = data->next;
	}
	return (find_place(&(*page)->next, size_wanted, page_size));
}

void	*malloc_large(size_t size)
{
	t_meta	*data;
	t_page	*origin;

	if (!g_zone[2].page)
	{
		if ((g_zone[2].page = create_page(align_size(size))) == NULL)
			return (NULL);
		data = (t_meta*)((char*)g_zone[2].page + align_size(sizeof(t_page)));
	}
	else
	{
		origin = g_zone[2].page;
		while (g_zone[2].page->next)
			g_zone[2].page = g_zone[2].page->next;
		g_zone[2].page->next = create_page(align_size(size));
		data = (t_meta*)((char*)g_zone[2].page->next +
				align_size(sizeof(t_page)));
		g_zone[2].page = origin;
	}
	data->is_free = 0;
	return ((char*)data + sizeof(t_meta));
}

void	*malloc(size_t size)
{
	size_t	tiny_page;
	size_t	small_page;
	void	*ptr;

	tiny_page = align_size(N) - sizeof(t_meta) - align_size(sizeof(t_page));
	small_page = align_size(M) - sizeof(t_meta) - align_size(sizeof(t_page));
	pthread_mutex_lock(&g_mutex_stock);
	if (size <= TINY)
		ptr = find_place(&g_zone[0].page, align_size(size), tiny_page);
	else if (size <= SMALL)
		ptr = find_place(&g_zone[1].page, align_size(size), small_page);
	else
		ptr = malloc_large(size);
	pthread_mutex_unlock(&g_mutex_stock);
	return (ptr);
}
