
#include <unistd.h>

void	ft_bzero(char **str, unsigned long size)
{
	unsigned long i = 0;

	while (i < size)
	{
		*str[i] = 0;
	}
}

int main(void)
{
	int		a;
	char	str[20];

//	unsigned int x = 1;
//	printf ("%d", (int) (((char *)&x)[0]));
	ft_bzero(&str);
	str[0] = "q";
	str[1] = "w";
	str[2] = "e";
	str[3] = "r";
	str[5] = "t";
	str[6] = "y";
	str[7] = "\n";
	write(1, str, 20);
	return (0);
}
