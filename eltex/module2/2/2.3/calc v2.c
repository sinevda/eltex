#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <stdarg.h>
#include <locale.h>

#define MAXSTRING 50
#define MAXARRAY 5

typedef double (*function)(int argc, ...);

typedef struct
{
	char name[MAXSTRING];
	function math_func;
} operation;

double summ(int argc, ...)
{
	if (argc < 2)
	{
		printf("Мало аргументов");
		return 0;
	}

	va_list l;
	va_start(l, argc);


	double res = va_arg(l, double);
	for (int i = 1; i < argc; ++i)
	{
		res += va_arg(l, double);
	}

	va_end(l);
	return res;
}

double diff(int argc, ...)
{
	if (argc < 2)
	{
		printf("Мало аргументов");
		return 0;
	}

	va_list l;
	va_start(l, argc);


	double res = va_arg(l, double);
	for (int i = 0; i < argc; ++i)
	{
		res -= va_arg(l, double);
	}

	va_end(l);
	return res;
}

double divis(int argc, ...)
{
	if (argc < 2)
	{
		printf("Мало аргументов");
		return 0;
	}

	va_list l;
	va_start(l, argc);


	double res = va_arg(l, double);
	for (int i = 0; i < argc; ++i)
	{
		int temp = va_arg(l, double);
		if (temp != 0) res /= temp;
	}

	va_end(l);
	return res;
}

double multipl(int argc, ...)
{
	if (argc < 2)
	{
		printf("Мало аргументов");
		return 0;
	}

	va_list l;
	va_start(l, argc);


	double res = va_arg(l, double);
	for (int i = 0; i < argc; ++i)
	{
		res *= va_arg(l, double);
	}

	va_end(l);
	return res;
}

int main()
{
	setlocale(LC_ALL, "Rus");

	operation op[] = {
		{"Сложение", summ},
		{"Вычитание", diff},
		{"Умножение", multipl},
		{"Деление", divis}
	};



	while (1)
	{
		int state;
		printf("Выберите операцию:\n1. Сумма\n2. Разность\n3. Умножение\n4. Деление\n5. Выход\nВыбор: ");
		scanf("%d", &state);
		if (state == 5) return 0;

		double a;
		double arr[MAXARRAY];

		int n = 0;
		printf("Сколько чисел?(не более %d): ", MAXARRAY);
		scanf("%d", &n);


		printf("Введите числа: ");

		for (int i = 0; i < n; ++i)
		{
			scanf("%lf", &a);
			arr[i] = a;
		}

		switch (n)
		{
		case 1:
			printf("Результат: %lf\n\n",
				op[state - 1].math_func(n, arr[0]));
			break;

		case 2:
			printf("Результат: %lf\n\n",
				op[state - 1].math_func(n, arr[0], arr[1]));
			break;

		case 3:
			printf("Результат: %lf\n\n",
				op[state - 1].math_func(n, arr[0], arr[1], arr[2]));
			break;

		case 4:
			printf("Результат: %lf\n\n",
				op[state - 1].math_func(n, arr[0], arr[1], arr[2], arr[3]));
			break;

		case 5:
			printf("Результат: %lf\n\n",
				op[state - 1].math_func(n, arr[0], arr[1], arr[2], arr[3], arr[4]));
			break;

		default:
			break;
		}


	}
	return 0;
}