/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 10:50:31 by lbopp             #+#    #+#             */
/*   Updated: 2019/01/22 10:40:29 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MALLOC_H
# define FT_MALLOC_H
# include "libft.h"
# include <sys/mman.h>
# include <pthread.h>

# define TINY	992
# define SMALL	130048
# define N		2097152
# define M		16777216
# define ALIGN	16

typedef struct		s_meta
{
	size_t			size;
	int				is_free;
	struct s_meta	*next;
	struct s_meta	*prev;
}					t_meta;

typedef struct		s_page
{
	struct s_page	*next;
}					t_page;

typedef struct		s_zone
{
	t_page			*page;
}					t_zone;

typedef struct		s_zone_id
{
	t_page			*page;
	t_page			*last;
	int				type;
}					t_zone_id;

t_zone					g_zone[3];
extern pthread_mutex_t	g_mutex_stock;

/*
**  FREE
*/
void				free(void *ptr);
void				ft_free(void *ptr);
void				try_remove_page(t_zone_id ret, t_meta *data);
void				free_defrag(t_meta **data);

/*
**	CHECK_DATA
*/
int					is_valid_page(int type, void *ptr, t_page *begin_page);
t_zone_id			find_zone_free(void *ptr);
int					check_data(t_page *page, t_meta *data);

/*
**  MALLOC
*/
void				*malloc(size_t size);
void				*find_place(t_page **page, size_t size_wanted,
							size_t page_size);
void				*fill_place(t_meta *data, size_t size_wanted,
							size_t page_size);
void				*malloc_large(size_t size);

/*
**	CREATE_PAGE
*/
t_page				*create_page(size_t page_size);
void				init_data(t_meta **data, size_t page_size);

/*
**  CALLOC
*/
void				*calloc(size_t count, size_t size);

/*
**  REALLOC
*/
void				*realloc(void *ptr, size_t size);
void				*reallocf(void *ptr, size_t size);
void				free_without_check(void *ptr, t_zone_id ret);

/*
**  SHOW_ALLOC_MEM
*/
void				print_type(int type);
void				show_alloc_mem(void);
void				print_mem(void *p0);
char				hex_digit(int v);
void				print_each_data(const t_page *page, size_t *total);

/*
**  ALIGN_DATA
*/
size_t				align_size(size_t init_size);
size_t				round_page_size(size_t init_size);

#endif
