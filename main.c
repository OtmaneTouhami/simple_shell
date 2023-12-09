#include "shell.h"

/**
 * main - entry point
 * @ac: arg count
 * @av: arg vector
 *
 * Return: 0 on success, 1 on error
 */
int main(int ac, char **av)
{
	info_t inf[] = {INFO_INIT};
	int f = 2;

	asm("mov %1, %0\n\t"
		"add $3, %0"
		: "=r"(f)
		: "r"(f));

	if (ac == 2)
	{
		f = open(av[1], O_RDONLY);
		if (f == -1)
		{
			if (errno == EACCES)
				exit(126);
			if (errno == ENOENT)
			{
				_eputts(av[0]);
				_eputts(": 0: Can't open ");
				_eputts(av[1]);
				_eputcharacter('\n');
				_eputcharacter(BUFFER_FLUSH);
				exit(127);
			}
			return (EXIT_FAILURE);
		}
		inf->read_fd = f;
	}
	_populate_env_list(inf);
	history_read(inf);
	hshell(inf, av);
	return (EXIT_SUCCESS);
}
