#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <stdarg.h>
#include <locale.h>

#define MAXSTRING 50

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
	for (int i = 0; i < argc; ++i)
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
		res /= va_arg(l, double);
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

	while (1)
	{
		int state;
		printf("Выберите операцию:\n1. Сумма\n2. Разность\n3. Умножение\n4. Деление\n5. Выход\nВыбор: ");
		scanf("%d", &state);
		if (state == 5) return 0;

		double a, b;
		printf("Введите два числа через пробел: ");
		scanf("%lf %lf", &a, &b);

		switch (state)
		{
		case 1:
			printf("Результат: %lf\n\n", summ(2, a, b));
			break;
		case 2:
			printf("Результат: %lf\n\n", diff(2, a, b));
			break;
		case 3:
			printf("Результат: %lf\n\n", multipl(2, a, b));
			break;
		case 4:
			printf("Результат: %lf\n\n", divis(2, a, b));
			break;
		case 5:
			return 0;
		default:
			break;
		}
	}

	return 0;
}