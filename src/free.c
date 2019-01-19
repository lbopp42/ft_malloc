/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/17 10:01:34 by lbopp             #+#    #+#             */
/*   Updated: 2019/01/19 10:46:28 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

void		free_defrag(t_meta **data)
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

void		try_remove_page(t_zone_id ret, t_meta *data)
{
	if (ret.type == 2)
	{
		if (ret.last)
			ret.last->next = ret.page->next;
		else
			g_zone[ret.type].page = ret.page->next;
		munmap(ret.page, data->size + sizeof(t_meta) +
				align_size(sizeof(t_page)));
	}
	else if (((ret.type == 0 && data->size == N - sizeof(t_meta) -
		align_size(sizeof(t_page))) || (ret.type == 1 && data->size == M -
		sizeof(t_meta) - align_size(sizeof(t_page)))) && (ret.last ||
		ret.page->next))
	{
		if (ret.last)
			ret.last->next = ret.page->next;
		else
			g_zone[ret.type].page = ret.page->next;
		munmap(ret.page, data->size + sizeof(t_meta) +
				align_size(sizeof(t_page)));
	}
}

void		ft_free(void *ptr)
{
	t_meta		*data;
	t_zone_id	ret;

	if (!ptr)
		return ;
	if (!(ret = find_zone_free(ptr)).page)
		return ;
	data = (t_meta*)((char*)ptr - sizeof(t_meta));
	if (!check_data(ret.page, data))
		return ;
	data->is_free = 1;
	free_defrag(&data);
	try_remove_page(ret, data);
}

void		free(void *ptr)
{
	pthread_mutex_lock(&g_mutex_stock);
	ft_free(ptr);
	pthread_mutex_unlock(&g_mutex_stock);
}
