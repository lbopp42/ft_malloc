/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/17 10:28:17 by lbopp             #+#    #+#             */
/*   Updated: 2019/01/22 10:37:24 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

void	free_without_check(void *ptr, t_zone_id ret)
{
	t_meta	*data;

	data = (t_meta*)((char*)ptr - sizeof(t_meta));
	data->is_free = 1;
	free_defrag(&data);
	try_remove_page(ret, data);
}

void	*realloc(void *ptr, size_t size)
{
	void		*new;
	t_meta		*data;
	t_zone_id	ret;

	if (!ptr)
		return (malloc(size));
	if (size == 0 && ptr)
	{
		free(ptr);
		return (malloc(ALIGN));
	}
	if (!(ret = find_zone_free(ptr)).page)
		return (NULL);
	data = (t_meta*)((char*)ptr - sizeof(t_meta));
	if (!check_data(ret.page, data))
		return (NULL);
	if (data->size >= align_size(size))
		return (ptr);
	if ((new = malloc(size)) == NULL)
		return (NULL);
	ft_memcpy(new, ptr, data->size);
	pthread_mutex_lock(&g_mutex_stock);
	free_without_check(ptr, ret);
	pthread_mutex_unlock(&g_mutex_stock);
	return (new);
}

void	*reallocf(void *ptr, size_t size)
{
	void		*new;
	t_meta		*data;
	t_zone_id	ret;

	if (!ptr)
		return (malloc(size));
	if (size == 0 && ptr)
	{
		free(ptr);
		return (malloc(ALIGN));
	}
	if (!(ret = find_zone_free(ptr)).page)
		return (NULL);
	data = (t_meta*)((char*)ptr - sizeof(t_meta));
	if (!check_data(ret.page, data))
		return (NULL);
	if (data->size >= align_size(size))
		return (ptr);
	new = malloc(size);
	if (new)
		ft_memcpy(new, ptr, data->size);
	pthread_mutex_lock(&g_mutex_stock);
	free_without_check(ptr, ret);
	pthread_mutex_unlock(&g_mutex_stock);
	return (new);
}
