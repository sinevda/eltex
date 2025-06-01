#include <stdio.h>
#include <stdarg.h>
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