//Составить программу определения такого максимального подотрезка среди элементов
//заданного целочисленного массива, элементы которого увеличиваются по порядку на 1.
//Подотрезок массива - это часть ("кусок") массива, который включает в себя все элементы
//исходного массива от некоторого начального индекса до некоторого другого конечного
//индекса без изменения порядка следования элементов. Элементы подотрезка функция
//должна вернуть через указатель-параметр, а ее длину — через возвращаемый результат.
//Программа должна уметь обрабатывать ошибки во входных данных (отрицательное количество
//элементов и др.). В случае возникновения ошибки нужно вывести в поток стандартного вывода
//сообщение "[error]" (без кавычек) и завершить выполнение программы.

#include <stdlib.h>
#include <stdio.h>

size_t	get_input(int ** const res)
{
	size_t	len = 0;
	int		*arr = NULL;
	int		buf;
	size_t	i = 0;
	int		ret;

	if (!res)
	{
		return 0;
	}
	ret = scanf("%zu", &len);
	if (!ret || ret == EOF)
	{
		return 0;
	}
	if (len <= 0)
	{
		return 0;
	}
	arr = (int*)malloc(sizeof(int) * len);
	if (!arr)
	{
		return 0;
	}
	while (i < len)
	{
		ret = scanf("%d", &buf);
		if (!ret || ret == EOF)
		{
			free(arr);
			return 0;
		}
		arr[i++] = buf;
	}
	*res = arr;
	return len;
}

size_t	get_max_seq(const int * const arr, const size_t arr_len, int ** const res)
{
	size_t	max_low_idx = 0;
	size_t	max_len = 1;
	size_t	low = 0;
	size_t	high = 1;
	size_t	len = 1;
	size_t	i = 0;

	if (!arr || !res)
	{
		return 0;
	}
	while (high < arr_len)
	{
		if (arr[high - 1] + 1 == arr[high])
		{
			++len;
			++high;
		}
		else
		{
			if (len > max_len)
			{
				max_len = len;
				max_low_idx = low;
			}
			low = high;
			high = low + 1;
			len = 1;
		}
	}
	if (len > max_len)
	{
		max_len = len;
		max_low_idx = low;
	}
	if (max_len == 1)
	{
		return 1;
	}
	*res = (int*)malloc(sizeof(int) * max_len);
	if (*res == NULL)
	{
		return 0;
	}
	while (i < max_len)
	{
		(*res)[i] = arr[max_low_idx + i];
		++i;
	}
	return max_len;
}

void	print_result(const int * const res, const size_t res_len)
{
	size_t	i = 0;

	if (!res_len)
	{
		printf("%s", "[error]");
		return ;
	}
	if (res_len == 1)
	{
		printf("%d", 0);
		return ;
	}
	printf("%zu\n", res_len);
	while (i < res_len)
	{
		printf("%d", res[i]);
		if (i != res_len - 1)
			printf(" ");
		++i;
	}
}

void	delete(int *res, int *arr)
{
	if (res)
		free(res);
	if (arr)
		free(arr);
}

int		main()
{
	int		*arr = NULL;
	size_t	arr_size = get_input(&arr);
	size_t	res_size = 0;
	int		*res = NULL;

	if (!arr_size)
	{
		printf("%s", "[error]");
		return 0;
	}
	res_size = get_max_seq(arr, arr_size, &res);
	print_result(res, res_size);
	delete(res, arr);
	return 0;
}
