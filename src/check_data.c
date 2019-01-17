/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/17 14:26:48 by lbopp             #+#    #+#             */
/*   Updated: 2019/01/17 14:26:51 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

int			is_valid_page(int type, void *ptr, t_page *begin_page)
{
	if ((type == 0 || type == 1) && (t_page*)ptr >= begin_page)
	{
		if (type == 0 && (char*)ptr <= (char*)begin_page + N)
			return (1);
		else if (type == 1 && (char*)ptr <= (char*)begin_page + M)
			return (1);
	}
	else if (type == 2 && (char*)ptr - align_size(sizeof(t_page))
			- sizeof(t_meta) == (char*)begin_page)
		return (1);
	return (0);
}

t_zone_id	find_zone_free(void *ptr)
{
	t_zone_id	info;
	t_page		*origin;
	int			type;

	type = -1;
	info.page = NULL;
	info.last = NULL;
	while (++type <= 2)
	{
		info.last = NULL;
		origin = g_zone[type].page;
		while (origin)
		{
			if (is_valid_page(type, ptr, origin))
			{
				info.type = type;
				info.page = origin;
				return (info);
			}
			info.last = origin;
			origin = origin->next;
		}
	}
	return (info);
}

int			check_data(t_page *page, t_meta *data)
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
