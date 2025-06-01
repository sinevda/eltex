#include <stdio.h>
#include <stdarg.h>
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