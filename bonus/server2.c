// burada sigaction kullanarak durumu yönettim
#include "minitalk.h"
#include <unistd.h>
void	ft_handle(int sig)
{
	static int	bit = 128;
	static int	c = 0;

	if (sig == SIGUSR1)
		c = c + bit;
	bit = bit / 2;
	if (bit == 0)
	{
		write(1, &c, 1);
		bit = 128;
		c = 0;
	}
}

int	main(void)
{
	struct sigaction sa;
	
	sa.sa_handler =ft_handle;
	sa.sa_flags = 0;
	write(1, "PID:", 4);
	ft_putnbr(getpid());
	write(1, "\n", 1);
	sigaction(SIGUSR1,&sa,NULL);
	sigaction(SIGUSR2,&sa,NULL);
	while (1)
		pause();
}
