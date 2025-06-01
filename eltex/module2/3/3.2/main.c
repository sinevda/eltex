
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

uint32_t GetIP(char[]);
uint32_t GenIP();
int comp(uint32_t, uint32_t, uint32_t);

int main(int argc, char* argv[])
{
	if (argc != 4) return -1;
	printf("Gate: %s, Mask: %s\n", argv[1], argv[2]);
	srand(time(NULL));

	uint32_t gwip = GetIP(argv[1]);
	uint32_t mask = GetIP(argv[2]);
	int N = atoi(argv[3]);
	int count = 0;
	for (int i = 0; i < N; ++i)
	{
		if (comp(gwip, mask, GenIP())) ++count;
	}

	printf("Accepted: %d (%.2f%%). Denied: %d. Total: %d.", count, ((float)count / N) * 100, N - count, N);
	getchar();

	return 0;
}

uint32_t GetIP(char string[])
{
	uint32_t ip = 0;
	char* p = strtok(string, ".");

	while (p != NULL)
	{
		ip <<= 8;
		ip += (int)atoi(p);

		p = strtok(NULL, ".");
	}

	return ip;
}

uint32_t GenIP()
{
	uint32_t ip = 0;

	for (int i = 0; i < 4; ++i)
	{
		ip <<= 8;
		ip += rand() % 255;
	}

	return ip;
}

int comp(uint32_t gwip, uint32_t mask, uint32_t ip)
{
	return (gwip & mask) == (mask & ip);
}