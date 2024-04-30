#include "microshell.h"

int	ft_error(char *error, char *arg)
{
	while (*error)
		write(2, error++, 1);
// 2 is the fd to write in stderror
	if (arg)
	{
		while (*arg)
			write(2, arg++, 1);
	}
	write(2, "\n", 1);
	return (1);
}

int ft_cd(char **av, int i)
{
	if (i != 2)
		return (ft_error("error: cd: bad arguments", NULL));
	else if (chdir(av[1]) == -1) //av[1] cause av[0] points to "cd"
		return (ft_error("error: cd: cannot change directory to ", av[1]));
	return (0);
}
int	ft_pipe(int	*fd, int rd)
{//fd is an int that determines if redirection is to pipe (1) or to stdout (0)
	if (dup2(fd[rd], rd) == -1)
		return (1);
	if (close(fd[0]) == -1)
		return (1); 
	if (close(fd[1]) == -1)
		return (1);
	return (0);
}
int ft_exec(char **argv, char **envp, int i, int pip)
{
	int fd[2];
	int status;

	if (pip && pipe(fd) == -1) // if there's a pipe and pipe() fails
		return (ft_error("error: fatal", NULL));
	int pid = fork();
	if (!pid) // fork pid is 0
	{
		argv[i] = 0; // nullterminates after comand
		if (pip && ft_pipe(fd, 1)) // if there's a pipe and redir faiils
			return ft_error("error: fatal", NULL);
		execve(*argv, argv, envp); //if execve works, it will end the fork
		return ft_error("error: cannot execute ", *argv); //if not, we must print the error
	}
	waitpid(pid, &status, 0); //we wait the fork to end
	if (pip && ft_pipe(fd, 0))
		return ft_error("error: fatal", NULL);
	return WIFEXITED(status) && WEXITSTATUS(status);
}


int main(int ac, char **av, char **envp) 
{
	int i = 0;
	int ex_st = 0;

	if (ac > 1)
	{
		while (av[i] && av[++i])
		{
			int pip = 0;
			av += i; //updates av, skipping what its already executed
			i = 0;
			while (av[i] && strcmp(av[i], "|") && strcmp(av[i], ";"))
				i++; //go ahead until the next delimiter
			if (!strcmp(*av, "cd")) // if av points to cd
				ex_st = ft_cd(av, i);
			else if (i)
			{
				if (av[i] && !strcmp(av[i], "|"))
					pip = 1;
				ex_st = ft_exec(av, envp, i, pip);
			}
		}
	}
	return (ex_st);
}