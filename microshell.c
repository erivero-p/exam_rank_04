#include "microshell.h"

int	ft_error(char *str, char *ag)
{
	while (*str) // 2 es el fd para escribir en stderror
		write(2, str++, 1);
	if (ag)
	{
		while (*ag)
			write(2, ag++, 1);
		write(2, "\n", 1);
	}
	return (1);
}
int ft_cd(char **av, int i)
{
	if (i != 2)
		return (ft_error("error: cd: bad arguments\n", NULL));
	else if (chdir(av[1] == -1)) //av[1] pq debo revisar el puntero av + 1 (ya que av apunta a cd)
		return (ft_error("error: cd: cannot change directory to ", av[1]));
//		return (ft_error("error: cd: cannot change directory to "), ft_error(av[1]), ft_error("\n"));
	return (0);
}

int main(int ac, char **av, char **envp) 
{
	int i = 0;
	int ex_st = 0;

	if (ac > 1)
	{
		while (av[i] && av[++i])
		{
			av += i; //actualiza el puntero av, saltando lo ya ejecutado
			i = 0;
			while (av[i] && strcmp(av[i], "|") && strcmp(av[i], ";"))
				i++; //avanza hasta encontrar el siguiente delimitador
			if (!strcmp(*av, "cd")) // si av apunta a "cd"
				ex_st = ft_cd(av, i);
			else if (i) //si no, y i no es 0
				ex_st = ft_exec(av, envp, i);
		}
	}
	return (ex_st);
}
