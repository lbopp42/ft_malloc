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

int main(void)
{
	launch_realloc();
	return (0);
}
