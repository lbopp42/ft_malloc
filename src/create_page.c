/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_page.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/17 16:46:36 by lbopp             #+#    #+#             */
/*   Updated: 2019/01/17 16:52:02 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

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

	new_page = (t_page*)mmap(0, round_page_size(page_size + sizeof(t_meta) +
				align_size(sizeof(t_page))), PROT_READ | PROT_WRITE,
		MAP_ANON | MAP_PRIVATE, -1, 0);
	new_page->next = NULL;
	data = (t_meta*)((char*)new_page + align_size(sizeof(t_page)));
	init_data(&data, round_page_size(page_size + sizeof(t_meta) +
				align_size(sizeof(t_page))) - align_size(sizeof(t_page)) -
			sizeof(t_meta));
	return (new_page);
}
