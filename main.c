// Разработайте программу-калькулятор, умеющую вычислять арифметические
// выражения над множествами натуральных чисел. Входные данные 
// (выражение с описанием множеств и производимых над ними операций) 
// подаются на стандартный поток ввода программы, результат вычислений
// должен подаваться на стандартный поток вывода. 
// Поддерживаемые операции: 'U' - объединение, '^' - пересечение, 
// '\' - разность множеств, '()' - задание приоритета вычислений.
// Множества задаются в виде [ el1, el2, ... ], где элементами являются обычные натуральные числа.
// Множества могут быть пустыми.
// Между операциями, множествами и элементами множеств может быть любое количество пробелов.
// При вычислениях должны учитываться приоритеты операций 
// (в том числе, заданные вручную при помощи круглых скобочек).

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define INIT_SIZE 128
#define MEM_MULT 3

char	*g_expr = NULL;

typedef struct	set_s {
	int	*set;
	int	len;
}				set_t;

int	cmp(const void *x1, const void *x2)
{
	return (*(int*)x1 - *(int*)x2);
}

int	search(set_t *set, int d)
{
	size_t	left = 0;
	size_t	right = set->len;

	while (left < right)
	{
		int mid = (left + right) / 2;
		if (set->set[mid] == d)
			return (1);
		if (set->set[mid] < d)
			left = mid + 1;
		else
			right = mid;
	}
	return (0);
}

int	free_set(set_t *n)
{
	if (n->set)
		free(n->set);
	free(n);
	return (1);
}

set_t	*set_union(set_t *set1, set_t *set2)
{
	set_t *res = (set_t*)malloc(sizeof(set_t));
	if (!res)
	{
		free_set(set1);
		free_set(set2);
		return (0);
	}
	res->len = 0;
	res->set = NULL;
	if (!set1->len && !set2->len)
	{
		free_set(set1);
		free_set(set2);
		return (res);
	}
	if (!set1->len)
	{
		qsort(set2->set, set2->len, sizeof(int), cmp);
		free_set(res);
		free_set(set1);
		return (set2);
	}
	if (!set2->len)
	{
		qsort(set1->set,set1->len, sizeof(int), cmp);
		free_set(res);
		free_set(set2);
		return (set1);
	}
	qsort(set2->set, set2->len, sizeof(int), cmp);
	qsort(set1->set, set1->len, sizeof(int), cmp);
	int *buf = (int*)malloc(sizeof(int) * (set1->len + set2->len));
	if (!buf)
	{
		free_set(set1);
		free_set(set2);
		free_set(res);
		return (0);
	}
	int prev;
	int i = 0;
	int j = 0;
	int m = 0;
	if (set1->set[i] < set2->set[j])
	{
		buf[m] = set1->set[i];
		i++;
	}
	else
	{
		buf[m] = set2->set[j];
		j++;
	}
	m++;
	prev = buf[0];
	while (i < set1->len && j < set2->len)
	{
		if (set1->set[i] < set2->set[j])
		{
			if (set1->set[i] == prev)
			{
				i++;
				continue;
			}
			buf[m] = set1->set[i];
			prev = buf[m];
			i++;
		}
		else
		{
			if (set2->set[j] == prev)
			{
				j++;
				continue;
			}
			buf[m] = set2->set[j];
			prev = buf[m];
			j++;
		}
		m++;
	}
	while (i < set1->len)
	{
		if (set1->set[i] == prev)
		{
			i++;
			continue;
		}
		buf[m] = set1->set[i];
		prev = buf[m];
		i++;
		m++;
	}
	while (j < set2->len)
	{
		if (set2->set[j] == prev)
		{
			j++;
			continue;
		}
		buf[m] = set2->set[j];
		prev = buf[m];
		j++;
		m++;
	}
	res->set = buf;
	res->len = m;
	free_set(set1);
	free_set(set2);
	return (res);
}

set_t *set_intersect(set_t *set1, set_t *set2)
{
	set_t *res = (set_t*)malloc(sizeof(set_t));
	if (!res)
	{
		free_set(set1);
		free_set(set2);
		return (0);
	}
	res->len = 0;
	res->set = NULL;
	if (!set1->len || !set2->len)
	{
		free_set(set1);
		free_set(set2);
		return (res);
	}
	qsort(set1->set, set1->len, sizeof(int), cmp);
	qsort(set2->set, set2->len, sizeof(int), cmp);
	int *buf = (int*)malloc(sizeof(int) * (set1->len + set2->len));
	if (!buf)
	{
		free_set(set1);
		free_set(set2);
		free_set(res);
		return (0);
	}
	int len = 0;
	int i = 0;
	while (i < set1->len)
	{
		if (search(set2, set1->set[i]))
			buf[len++] = set1->set[i++];
		else
			++i;
	}
	res->set = buf;
	res->len = len;
	free_set(set1);
	free_set(set2);
	return (res);
}

set_t	*set_div(set_t *set1, set_t *set2)
{
	set_t *res = (set_t*)malloc(sizeof(set_t));
	if (!res)
	{
		free_set(set1);
		free_set(set2);
		return (0);
	}
	res->len = 0;
	res->set = NULL;
	if (!set1->len && !set2->len)
	{
		free_set(set1);
		free_set(set2);
		free_set(res);
		return (res);
	}
	if (!set1->len)
	{
		free_set(set2);
		return (res);
	}
	if (!set2->len)
	{
		qsort(set1->set, set1->len, sizeof(int), cmp);
		free_set(set1);
		free_set(res);
		return (set1);
	}
	qsort(set1->set, set1->len, sizeof(int), cmp);
	qsort(set2->set, set2->len, sizeof(int), cmp);
	int *buf = (int*)malloc(sizeof(int) * (set1->len + set2->len));
	if (!buf)
	{
		free_set(set1);
		free_set(set2);
		free_set(res);
		return (0);
	}
	int len = 0;
	int i = 0;
	while (i < set1->len)
	{
		if (!search(set2, set1->set[i]))
			buf[len++] = set1->set[i++];
		else
			++i;
	}
	res->set = buf;
	res->len = len;
	free_set(set1);
	free_set(set2);
	return (res);
}

int	check_set(set_t *set)
{
	int	prev;
	int	i = 1;
	if (!set->len)
		return (1);
	prev = set->set[0];
	while (i < set->len)
	{
		if (set->set[i] == prev)
			return (0);
		prev = set->set[i++];
	}
	return (1);
}

int	parse_set(set_t **res)
{
	int	size = 0;
	int	i = 0;
	int	*set;
	int	digit;

	while (g_expr[++i] != ']')
	{
		if (!isdigit(g_expr[i]) && g_expr[i] != ',')
			return (0);
		if (g_expr[i] == ',' && !isdigit(g_expr[i + 1]))
			return (0);
		if (g_expr[i] == ',' && g_expr[i - 1] == '[')
			return (0);
	}
	*res = (set_t*)malloc(sizeof(set_t));
	if (!*res)
		return (0);
	i = 0;
	if (g_expr[i + 1] == ']')
	{
		g_expr += 2;
		(*res)->len = 0;
		(*res)->set = NULL;
		return (1);
	}
	while (g_expr[i] != ']')
		if (g_expr[i++] == ',')
			size++;
	set = (int*)malloc(sizeof(int) * (size + 1));
	if (!set)
	{
		free_set(*res);
		return (0);
	}
	i = 0;
	size = 0;
	while (g_expr[i] != ']')
	{
		if (g_expr[i] == '[' || g_expr[i] == ',')
		{
			++i;
			digit = atoi(g_expr + i);
			set[size++] = digit;
		}
		++i;
	}
	g_expr += (i + 1);
	(*res)->len = size;
	(*res)->set = set;
	if (!check_set(*res))
	{
		free_set(*res);
		return (0);
	}
	return (size);
}

int	get_expr(char **arr)
{
	int		size = INIT_SIZE;
	char	*buf = NULL;
	char	*tmp = NULL;
	int		i = 0;
	char	sym;

	buf = (char*)malloc(sizeof(char) * size);
	if (!buf)
		return (0);
	while (scanf("%c", &sym) != EOF)
	{
		if (sym != ' ')
		{
			buf[i] = sym;
			++i;
		}
		if (i == size)
		{
			size *= MEM_MULT;
			tmp = (char*)realloc(buf, size);
			if (!tmp)
			{
				free(buf);
				return (0);
			}
			else
				buf = tmp;
		}
	}
	buf[i] = '\0';
	*arr = buf;
	return (1);
}

set_t	*evalExpr();
set_t	*getSum();
set_t	*getFactor();
set_t	*getSet();

set_t *evalExpr()
{
	set_t *res = getSum();
	if (!res)
		return (0);
	while (*g_expr == 'U' || *g_expr == '\\')
		if (*g_expr == 'U')
		{
			g_expr++;
			set_t *sum = getSum();
			if (!sum)
			{
				free_set(res);
				return (0);
			}
			res = set_union(res, sum);
			if (!res)
				return (0);
		}
		else
		{
			g_expr++;
			set_t *sum = getSum();
			if (!sum)
			{
				free_set(res);
				return (0);
			}
			res = set_div(res, sum);
			if (!res)
				return (0);
		}
	return (res);
}

set_t	*getSum()
{
	set_t *res = getFactor();
	if (!res)
		return (0);
	while (*g_expr == '^')
	{
		g_expr++;
		set_t *set = getFactor();
		if (!set)
		{
			free_set(res);
			return (0);
		}
		res = set_intersect(res, set);
		if (!res)
			return (0);
	}
	return (res);
}

set_t	*getFactor()
{
	if (*g_expr == '(')
	{
		g_expr++;
		set_t *res = evalExpr();
		if (!res)
			return (0);
		if (*g_expr != ')')
		{
			free_set(res);
			return (0);
		}
		g_expr++;
		return (res);
	}
	else
	{
		set_t *set = getSet();
		if (!set)
			return (0);
		return (set);
	}
}

set_t	*getSet()
{
	set_t *res = NULL;

	if (!parse_set(&res))
		return (0);
	return (res);
}

void	print_result(set_t *r, char *begin)
{
	int	i = 0;
	int	size;

	if (r == NULL)
	{
		free(begin);
		printf("[error]");
		return ;
	}
	if (*g_expr != '\0')
	{
		printf("[error]");
		free_set(r);
		free(begin);
		return ;
	}
	size = r->len;
	if (!r->len)
	{
		printf("[]");
		free(begin);
		free_set(r);
		return ;
	}
	printf("[");
	while (i < size)
	{
		if (i == size - 1)
		{
			printf("%d]", r->set[i++]);
			continue;
		}
		printf("%d,", r->set[i++]);
	}
	free_set(r);
	free(begin);
}

int	main()
{
	if (!get_expr(&g_expr))
	{
		printf("[error]");
		return (0);
	}
	char *begin = g_expr;
	set_t *r = evalExpr();
	print_result(r, begin);
	return (0);
}
