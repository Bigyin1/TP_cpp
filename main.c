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

typedef struct	set_s {
	int	*set;
	int	len;
}				set_t;

void	free_set(set_t *n)
{
	if (!n)
	{
		return;
	}
	if (n->set)
	{
		free(n->set);
	}
	free(n);
}

void	delete_sets(set_t *p1, set_t *p2)
{
	if (p1)
	{
		free_set(p1);
	}
	if (p2)
	{
		free_set(p2);
	}
}

int	cmp(const void *x1, const void *x2)
{
	return (*(int*)x1 - *(int*)x2);
}

int	search(set_t *set, int d)
{
	if (!set)
	{
		return -1;
	}
	size_t	left = 0;
	size_t	right = set->len;

	while (left < right)
	{
		size_t mid = (left + right) / 2;
		if (set->set[mid] == d)
			return 1;
		if (set->set[mid] < d)
			left = mid + 1;
		else
			right = mid;
	}
	return 0;
}

set_t	*copy_set(set_t *src, set_t *dist)
{
	if (!src || !dist)
	{
		return 0;
	}
	free(dist->set);
	int i = 0;
	dist->len = src->len;
	dist->set = (int*)malloc(sizeof(int) * dist->len);
	if (!dist->set)
		return 0;
	while (i < dist->len)
	{
		dist->set[i] = src->set[i];
		++i;
	}
	return dist;
}

set_t	*set_union(set_t *set1, set_t *set2)
{
	if (!set1 || !set2)
	{
		return 0;
	}
	set_t *res = (set_t*)malloc(sizeof(set_t));
	if (!res)
	{
		return 0;
	}
	res->len = 0;
	res->set = NULL;
	if (!set1->len && !set2->len)
	{
		return res;
	}
	if (!set1->len)
	{
		qsort(set2->set, set2->len, sizeof(int), cmp);
		set_t *buf = copy_set(set2, res);
		if (!buf)
		{
			free_set(res);
			return 0;
		}
		return buf;
	}
	if (!set2->len)
	{
		qsort(set1->set,set1->len, sizeof(int), cmp);
		set_t *buf = copy_set(set1, res);
		if (!buf)
		{
			free_set(res);
			return 0;
		}
		return buf;
	}
	qsort(set2->set, set2->len, sizeof(int), cmp);
	qsort(set1->set, set1->len, sizeof(int), cmp);
	int *buf = (int*)malloc(sizeof(int) * (set1->len + set2->len));
	if (!buf)
	{
		free_set(res);
		return 0;
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
				m--;
			}
			else
			{
				buf[m] = set1->set[i];
				prev = buf[m];
				i++;
			}
		}
		else
		{
			if (set2->set[j] == prev)
			{
				j++;
				m--;
			}
			else
			{
				buf[m] = set2->set[j];
				prev = buf[m];
				j++;
			}
		}
		m++;
	}
	while (i < set1->len)
	{
		if (set1->set[i] == prev)
		{
			i++;
		}
		else
		{
			buf[m] = set1->set[i];
			prev = buf[m];
			i++;
			m++;
		}
	}
	while (j < set2->len)
	{
		if (set2->set[j] == prev)
		{
			j++;
		}
		else
		{
			buf[m] = set2->set[j];
			prev = buf[m];
			j++;
			m++;
		}
	}
	res->set = buf;
	res->len = m;
	return (res);
}

set_t *set_intersect(set_t *set1, set_t *set2)
{
	if (!set1 || !set2)
	{
		return 0;
	}
	set_t *res = (set_t*)malloc(sizeof(set_t));
	if (!res)
	{
		return 0;
	}
	res->len = 0;
	res->set = NULL;
	if (!set1->len || !set2->len)
	{
		return res;
	}
	qsort(set1->set, set1->len, sizeof(int), cmp);
	qsort(set2->set, set2->len, sizeof(int), cmp);
	int *buf = (int*)calloc((set1->len + set2->len), sizeof(int));
	if (!buf)
	{
		free_set(res);
		return 0;
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
	return res;
}

set_t	*set_div(set_t *set1, set_t *set2)
{
	if (!set1 || !set2)
	{
		return 0;
	}
	set_t *res = (set_t*)malloc(sizeof(set_t));
	if (!res)
	{
		return 0;
	}
	res->len = 0;
	res->set = NULL;
	if (!set1->len && !set2->len)
	{
		return res;
	}
	if (!set1->len)
	{
		return res;
	}
	if (!set2->len)
	{
		qsort(set1->set, set1->len, sizeof(int), cmp);
		set_t *buf = copy_set(set1, res);
		if (!buf)
		{
			free_set(res);
			return 0;
		}
		return buf;
	}
	qsort(set1->set, set1->len, sizeof(int), cmp);
	qsort(set2->set, set2->len, sizeof(int), cmp);
	int *buf = (int*)calloc((set1->len + set2->len), sizeof(int));
	if (!buf)
	{
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
	return res;
}

int	check_doublicates_in_set(set_t *set)
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

int	check_set_symbols(char *expr)
{
	int i = 0;

	while (expr[++i] != ']')
	{
		if (!isdigit(expr[i]) && expr[i] != ',')
		{
			return 0;
		}
		if (expr[i] == ',' && !isdigit(expr[i + 1]))
		{
			return 0;
		}
		if (expr[i] == ',' && expr[i - 1] == '[')
		{
			return 0;
		}
	}
	return 1;
}

int	parse_set(set_t **res, char **expr)
{
	if (!res || !expr)
	{
		return 0;
	}
	if (!check_set_symbols(*expr))
	{
		return 0;
	}
	int	size = 0;
	int	i = 0;
	int	*set;
	int	digit;

	*res = (set_t*)malloc(sizeof(set_t));
	if (!*res)
	{
		return 0;
	}
	if ((*expr)[i + 1] == ']')
	{
		*expr += 2;
		(*res)->len = 0;
		(*res)->set = NULL;
		return 1;
	}
	while ((*expr)[i] != ']')
		if ((*expr)[i++] == ',')
			size++;
	set = (int*)malloc(sizeof(int) * (size + 1));
	if (!set)
	{
		free_set(*res);
		return 0;
	}
	i = 0;
	size = 0;
	while ((*expr)[i] != ']')
	{
		if ((*expr)[i] == '[' || (*expr)[i] == ',')
		{
			++i;
			digit = atoi(*expr + i);
			set[size++] = digit;
		}
		++i;
	}
	*expr += (i + 1);
	(*res)->len = size;
	(*res)->set = set;
	if (!check_doublicates_in_set(*res))
	{
		free_set(*res);
		return 0;
	}
	return size;
}

int	get_expr(char **arr)
{
	if (!arr)
	{
		return 0;
	}
	int		size = INIT_SIZE;
	char	*buf = NULL;
	char	*tmp = NULL;
	int		i = 0;
	char	sym;

	buf = (char*)malloc(sizeof(char) * size);
	if (!buf)
		return 0;
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
				return 0;
			}
			else
				buf = tmp;
		}
	}
	buf[i] = '\0';
	*arr = buf;
	return 1;
}

set_t	*evalExpr(char **expr);
set_t	*getSum(char **expr);
set_t	*getFactor(char **expr);
set_t	*getSet(char **expr);

set_t *evalExpr(char **expr)
{
	if (!expr)
	{
		return 0;
	}
	set_t *res = getSum(expr);
	if (!res)
	{
		return 0;
	}
	while (**expr == 'U' || **expr == '\\')
		if (**expr == 'U')
		{
			(*expr)++;
			set_t *sum = getSum(expr);
			if (!sum)
			{
				free_set(res);
				return 0;
			}
			set_t *buf = set_union(res, sum);
			delete_sets(res, sum);
			if (!buf)
			{
				return 0;
			}
			res = buf;
		}
		else
		{
			(*expr)++;
			set_t *sum = getSum(expr);
			if (!sum)
			{
				free_set(res);
				return 0;
			}
			set_t *buf = set_div(res, sum);
			delete_sets(res, sum);
			if (!buf)
			{
				return 0;
			}
			res = buf;
		}
	return res;
}

set_t	*getSum(char **expr)
{
	if (!expr)
	{
		return 0;
	}
	set_t *res = getFactor(expr);
	if (!res)
	{
		return 0;
	}
	while (**expr == '^')
	{
		(*expr)++;
		set_t *set = getFactor(expr);
		if (!set)
		{
			free_set(res);
			return 0;
		}
		set_t *buf = set_intersect(res, set);
		delete_sets(res, set);
		if (!buf)
		{
			return 0;
		}
		res = buf;
	}
	return res;
}

set_t	*getFactor(char **expr)
{
	if (!expr)
	{
		return 0;
	}
	if (**expr == '(')
	{
		(*expr)++;
		set_t *res = evalExpr(expr);
		if (!res)
		{
			return 0;
		}
		if (**expr != ')')
		{
			free_set(res);
			return 0;
		}
		(*expr)++;
		return (res);
	}
	else
	{
		set_t *set = getSet(expr);
		if (!set)
		{
			return 0;
		}
		return (set);
	}
}

set_t	*getSet(char **expr)
{
	set_t *res = NULL;

	if (!parse_set(&res, expr))
	{
		return 0;
	}
	return (res);
}

void	print_result(set_t *r, char *begin, char *expr)
{
	int	i = 0;
	int	size;

	if (!begin)
	{
		if (r)
			free(r);
		printf("[error]");
		return ;
	}
	if (!r)
	{
		free(begin);
		printf("[error]");
		return ;
	}
	if (*expr != '\0')
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
		
		printf("%d", r->set[i]);
		if (i != size - 1)
		{
			printf(",");
		}
		++i;
	}
	printf("]");
	free_set(r);
	free(begin);
}

int	main()
{
	char *expr = NULL;
	if (!get_expr(&expr))
	{
		printf("[error]");
		return (0);
	}
	char *begin = expr;
	set_t *r = evalExpr(&expr);
	print_result(r, begin, expr);
	return (0);
}
