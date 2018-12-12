/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 10:50:31 by lbopp             #+#    #+#             */
/*   Updated: 2018/11/11 13:14:35 by lbopp            ###   ########.fr       */
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

typedef struct	s_meta
{
	size_t			size;
	int				is_free;
	struct s_meta	*next;
	struct s_meta	*prev;
}				t_meta;

typedef struct	s_page
{
	struct s_page	*next;
}				t_page;

typedef struct	s_zone
{
	t_page			*page;
}				t_zone;

typedef struct	s_zone_id
{
	t_page			*page;
	t_page			*last;
	int				type;
}				t_zone_id;

t_zone			g_zone[3];
pthread_mutex_t g_mutex_stock;
void			free(void *ptr);
void			*malloc(size_t size);
void			*realloc(void *ptr, size_t size);
void			*calloc(size_t n, size_t size);
size_t			align_size(size_t size);
void			show_alloc_mem(void);

#endif
