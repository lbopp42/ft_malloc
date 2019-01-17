/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/17 10:01:34 by lbopp             #+#    #+#             */
/*   Updated: 2019/01/17 10:05:34 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

void	    free_defrag(t_meta **data)
{
    if ((*data)->next && (*data)->next->is_free == 1)
    {
	(*data)->size += (*data)->next->size + sizeof(t_meta);
	if ((*data)->next->next)
	    (*data)->next->next->prev = *data;
	(*data)->next = (*data)->next->next;
    }
    if ((*data)->prev && (*data)->prev->is_free == 1)
    {
	(*data)->prev->size += (*data)->size + sizeof(t_meta);
	if ((*data)->next)
		(*data)->next->prev = (*data)->prev;
	(*data)->prev->next = (*data)->next;
	(*data) = (*data)->prev;
    }
}

t_zone_id   find_zone_free(void *ptr)
{
    t_zone_id	info;
    t_page		*origin;
    int			type;

    type = 0;
    info.page = NULL;
    info.last = NULL;
    while (type <= 2)
    {
	info.last = NULL;
	origin = g_zone[type].page;
	while (origin)
	{
	    if (((t_page*)ptr >= origin && ((type == 0 && (char*)ptr <= (char*)origin + N) ||
			    (type == 1 && (char*)ptr <= (char*)origin + M))) ||
		    (type == 2 && (char*)ptr - align_size(sizeof(t_page)) - sizeof(t_meta) == (char*)origin))
	    {
		info.type = type;
		info.page = origin;
		return (info);
	    }
	    info.last = origin;
	    origin = origin->next;
	}
	type++;
    }
    return (info);
}

void	    try_remove_page(t_zone_id ret, t_meta *data)
{
    if (ret.type == 2)
    {
	if (ret.last)
	    ret.last->next = ret.page->next;
	else
	    g_zone[ret.type].page = ret.page->next;
	munmap(ret.page, data->size + sizeof(t_meta) + align_size(sizeof(t_page)));
    }
    else if (((ret.type == 0 && data->size == N - sizeof(t_meta) - align_size(sizeof(t_page)))
		|| (ret.type == 1 && data->size == M - sizeof(t_meta) - align_size(sizeof(t_page)))) && (ret.last || ret.page->next))
    {
	if (ret.last)
	    ret.last->next = ret.page->next;
	else
	    g_zone[ret.type].page = ret.page->next;
	munmap(ret.page, data->size + sizeof(t_meta) + align_size(sizeof(t_page)));
    }
}

int	    check_data(t_page *page, t_meta *data)
{
    t_meta	*valid_data;

    valid_data = (t_meta*)((char*)page + align_size(sizeof(t_page)));
    while (valid_data)
    {
	if (valid_data && valid_data == data)
	    return (1);
	valid_data = valid_data->next;
    }
    return (0);
}

void	    free(void *ptr)
{
    t_meta		*data;
    t_zone_id	ret;

    if (!ptr)
	return;
    if (!(ret = find_zone_free(ptr)).page)
	return;
    data = (t_meta*)((char*)ptr - sizeof(t_meta));
    if (!check_data(ret.page, data))
	return;
    data->is_free = 1;
    free_defrag(&data);
    try_remove_page(ret, data);
}
