#include "ft_malloc.h"

void	launch_realloc(void)
{
	char	*tmp;
	char	*tmp1;

	tmp = malloc(5000);
	ft_putendl("=========================== MALLOC ====================");
	show_alloc_mem();
	tmp = realloc(tmp, 4000);
	ft_putendl("=========================== REALLOC ====================");
	show_alloc_mem();
	tmp1 = malloc(4000);
	ft_putendl("=========================== MALLOC ====================");
	show_alloc_mem();
}

void	try_create_page()
{
	char	*str;
	char	*str2;
	char	*str3;

	str = malloc(900);
	ft_bzero(str, 900);
	show_alloc_mem();
	ft_putendl("=================================================");
	str2 = malloc(90000);
	ft_bzero(str2, 90000);
	show_alloc_mem();
	ft_putendl("=================================================");
	str3 = malloc(900000);
	ft_bzero(str3, 900000);
	show_alloc_mem();
}

void	create_many_small_page()
{
	char	*str[150];
	int		i;

	i = 0;
	while (i < 150)
		str[i++] = malloc(130000);
	show_alloc_mem();
}

int main(void)
{
	//try_create_page();
	create_many_small_page();
	//launch_realloc();
	return (0);
}
