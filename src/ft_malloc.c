/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/12 13:01:18 by lbopp             #+#    #+#             */
/*   Updated: 2018/12/13 17:33:08 by lbopp            ###   ########.fr       */
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

	page = g_zone[0].page;
	while (page)
	{
		ft_putstr("TINY : ");
		print_mem(page);
		ft_putchar('\n');
		origin = (t_meta*)((char*)page + align_size(sizeof(t_page*)));
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
	page = g_zone[1].page;
	while (page)
	{
		ft_putstr("SMALL : ");
		print_mem(page);
		ft_putchar('\n');
		origin = (t_meta*)((char*)page + align_size(sizeof(t_page*)));
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
	page = g_zone[2].page;
	while (page)
	{
		ft_putstr("LARGE : ");
		print_mem(page);
		ft_putchar('\n');
		origin = (t_meta*)((char*)page + align_size(sizeof(t_page*)));
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
		ft_putendl("NEXT");
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

	//ft_putstr("Creation de page de taille = ");
	//ft_putnbr(round_page_size(page_size + sizeof(t_meta) + align_size(sizeof(t_page))));
	//ft_putendl("");
	new_page = (t_page*)mmap(0, round_page_size(page_size + sizeof(t_meta) + align_size(sizeof(t_page))), PROT_READ | PROT_WRITE,
			MAP_ANON | MAP_PRIVATE, -1, 0);
	if (new_page == MAP_FAILED)
	{
		ft_putendl("FAIL MAP");
		exit(1);
	}
	new_page->next = next_page;
	data = (t_meta*)((char*)new_page + align_size(sizeof(t_page)));
	init_data(data, round_page_size(page_size + sizeof(t_meta) + align_size(sizeof(t_page))) - align_size(sizeof(t_page)) - sizeof(t_meta));
	//ft_bzero((char*)data + sizeof(t_meta), data->size);
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
		data->size = size_wanted;
	}
	data->is_free = 0;
	return ((char*)data + sizeof(t_meta));
}

void	*find_place(t_page *page, size_t size_wanted, size_t page_size)
{
	t_meta	*data;

	if (!page)
		page = create_page(page_size, NULL);
	data = (t_meta*)((char*)page + align_size(sizeof(t_page)));
	while (data)
	{
		if (data->is_free && data->size >= size_wanted)
			return (fill_place(data, size_wanted, page_size));
		data = data->next;
	}
	return (find_place(page->next, size_wanted, page_size));
}

void	*malloc(size_t size)
{
	t_page	*origin;
	t_meta	*data;

	//ft_putendl("MALLOC = ");
	//ft_putnbr(size);
	//ft_putendl("");
	if (size <= TINY)
	{
		if (!g_zone[0].page)
			g_zone[0].page = create_page(align_size(N) - sizeof(t_meta) - align_size(sizeof(t_page)), NULL);
		return (find_place(g_zone[0].page, align_size(size), align_size(N) - sizeof(t_meta) - align_size(sizeof(t_page))));
	}
	if (size <= SMALL)
	{
		if (!g_zone[1].page)
			g_zone[1].page = create_page(align_size(M) - sizeof(t_meta) - align_size(sizeof(t_page)), NULL);
		return (find_place(g_zone[1].page, align_size(size), align_size(M) - sizeof(t_meta) - align_size(sizeof(t_page))));
	}
	if (!g_zone[2].page)
	{
		g_zone[2].page = create_page(align_size(size), NULL);
		data = (t_meta*)((char*)g_zone[2].page + align_size(sizeof(t_page)));
	}
	else
	{
		origin = g_zone[2].page;
		while (g_zone[2].page->next)
			g_zone[2].page = g_zone[2].page->next;
		g_zone[2].page->next = create_page(align_size(size), NULL);
		data = (t_meta*)((char*)g_zone[2].page->next + align_size(sizeof(t_page)));
		g_zone[2].page = origin;
	}
	data->is_free = 0;
	//ft_putendl("=============================== MALLOC IN ==================================");
	//show_alloc_mem();
	//ft_putendl("========================================================================");
	//sleep(2);
	return ((char*)data + sizeof(t_meta));
}

void	basic_free(t_meta *data)
{
	data->is_free = 1;
}

void	free_defrag(t_meta *data)
{
	if (data->next && data->next->is_free == 1)
	{
		data->size += data->next->size + sizeof(t_meta);
		if (data->next->next)
			data->next->next->prev = data;
		data->next = data->next->next;
	}
	if (data->prev && data->prev->is_free == 1)
	{
		data->prev->size += data->size + sizeof(t_meta);
		if (data->next)
			data->next->prev = data->prev;
		data->prev->next = data->next;
		*data = *data->prev;
	}
}

t_zone_id	find_zone_free(void *ptr)
{
	t_page	*origin;
	t_zone_id	ret;
	int		i;		//TODO TEST	

	origin = g_zone[0].page;
	ret.last = NULL;
	i = 0;
	while (origin)
	{
		if (origin <= (t_page*)ptr && (char*)origin + N >= (char*)ptr)
		{
			ret.page = origin;
			ret.type = 0;
			return (ret);
		}
		i++;
		ret.last = origin;
		origin = origin->next;
	}
	origin = g_zone[1].page;
	ret.last = NULL;
	i = 0;
	while (origin)
	{
		if (origin <= (t_page*)ptr && (char*)origin + N >= (char*)ptr)
		{
			ret.page = origin;
			ret.type = 1;
			return (ret);
		}
		i++;
		ret.last = origin;
		origin = origin->next;
	}
	origin = g_zone[2].page;
	ret.last = NULL;
	i = 0;
	while (origin)
	{
		if ((char*)ptr - align_size(sizeof(t_page*)) - sizeof(t_meta) == (char*)origin)
		{
			ret.page = origin;
			ret.type = 2;
			return (ret);
		}
		i++;
		ret.last = origin;
		origin = origin->next;
	}
	ret.page = NULL;
	return (ret);
}

void	try_remove_page(t_zone_id ret, t_meta *data)
{
	if (ret.type == 2)
	{
		if (ret.last)
			ret.last->next = ret.page->next;
		else
			g_zone[ret.type].page = ret.page->next;
		munmap(ret.page, data->size + sizeof(t_meta) + align_size(sizeof(t_page*)));
	}
	else if (((data->size == N - sizeof(t_meta) && ret.type == 0)
			|| (data->size == M - sizeof(t_meta) && ret.type == 1)) && (ret.last || ret.page->next))
	{
		if (ret.last)
			ret.last->next = ret.page->next;
		else
			g_zone[ret.type].page = ret.page->next;
		munmap(ret.page, data->size + sizeof(t_meta) + align_size(sizeof(t_page*)));
	}
}

int		check_data(t_page *page, t_meta *data)
{
	t_meta	*valid_data;

	valid_data = (t_meta*)((char*)page + align_size(sizeof(t_page*)));
	while (valid_data && valid_data != data)
		valid_data = valid_data->next;
	if (valid_data && valid_data == data)
		return (1);
	return (0);
}

void	free(void *ptr)
{
	(void)ptr;
	t_meta		*data;
	t_zone_id	ret;

	if (ptr == NULL)
		return;
	if (!(ret = find_zone_free(ptr)).page)
		return;
	data = (t_meta*)((char*)ptr - sizeof(t_meta));
	if (!check_data(ret.page, data))
		return;
	free_defrag(data);
	basic_free(data);
	try_remove_page(ret, data);
}

void	*realloc(void *ptr, size_t size)
{
	(void)ptr;
	//free(ptr);
	return (malloc(size));
}

int		main(void)
{
	char	*str[1000000];
	char	*str1[10000];
	char	*str2[10000];

	int i = 0;
	while (i < 10000)
	{
		//str[i] = malloc(1000);
		//ft_bzero(str[i], 1000);
		//str1[i] = malloc(100000);
		//ft_bzero(str1[i], 100000);
		//str[i] = malloc(500);
		//ft_bzero(str[i], 500);
		str1[i] = malloc(2000);
		ft_bzero(str1[i], 2000);
		//str2[i] = malloc(1000000);
		//ft_bzero(str2[i], 1000000);
		i++;
	}
	i = 0;
	while (i < 10000)
	{
		//free(str[i]);
		free(str1[i]);
		//free(str2[i]);
		i++;
	}
		ft_putendl("============================================================================");
		ft_putendl("APRES FREE");
		show_alloc_mem();
		ft_putendl("----------------------------------------------------------------------------");
		sleep(2);
	return (1);
}
