

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <locale.h>
#include <dirent.h>
#include <dlfcn.h>
#include <ctype.h>

#define MAXSTRING 50
#define MAXARRAY 5

typedef double (*function)(int argc, ...);

typedef struct
{
	char name[MAXSTRING];
	function math_func;
} operation;

operation op[5];
int v = 0;

void* init(char* path)
{
	void* handle;
	DIR* dir = opendir(path);
	struct dirent* e;// = readdir(dir);
	
	while ((e = readdir(dir)) != NULL)
	{
		if(strstr(e->d_name, "so") != NULL)
		{
			char libpath[100];
			memset(libpath, '\0', 100);
			snprintf(libpath, 100, "%s/%s", path, e->d_name);
			handle = dlopen(libpath, RTLD_LAZY);
			if(handle != NULL)
			{
				char* d = e->d_name;
				d[strlen(d) - 3] = 0;
				strcpy(op[v].name, d);
				op[v++].math_func = dlsym(handle, d);
			}
			
		}
	}
	return handle;
}

int main(int argc, char* argv[])
{
	if (argc != 2) return -1;

	void* handle = init(argv[1]);

	setlocale(LC_ALL, "Rus");
	while (1)
	{
		printf("Выберите операцию:\n");
		for(int i = 0; i < v; ++i)
		{
			printf("%d. %s\n", i + 1, op[i].name);
		}
		int state;
		printf("%d. Выход\nВыбор: ", v+1);
		scanf("%d", &state);
		if (state == v+1) return 0;
		if (state > v+1 || !isdigit(state)) 
		{
			getchar();
		}

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
	dlclose(handle);
	return 0;
}