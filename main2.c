#include <string.h>
#include <stdio.h>
#include <stdlib.h>
void	print_first_section(char **av)
{
	int i;
	char *str = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	printf("/* XPM */\n");
	printf("static char *_");
	i = 0;
	while (i < 30)
	{
		printf("%c", str[(int) ((rand() / ((double) RAND_MAX + 1)) * strlen(str))]);
		i++;
	}
	printf("[] = {\n");
	printf("/* columns rows colors chars-per-pixel */\n");
	printf("\"%s %s %s %s \",\n", av[1], av[2], av[3], av[4]);
	printf("\"  c #7A7A7A\",\n");
	printf("\". c #000000\",\n");
	printf("/* pixels */\n");
}

int main(int ac, char **av)
{
	int i;
	int j;
	int b;

	b = 2;
	i = 0;
	j = 0;
	print_first_section(av);
	while (j < atoi(av[2]))
	{
		printf("\"");
		i = 0;
		while (i < atoi(av[1]))
		{
			if (i == 0 || i > atoi(av[1]) - b  || j == 0 || j > atoi(av[2]) - b)
				printf(".");
			else
				printf(" ");
			i++;
		}
		if (j != atoi(av[2]) - 1)
			printf("\",\n");
		j++;
	}
	printf("\"\n};");
}