#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	pid_t pid, ppid;

	pid = getpid();
	printf("pid is %d\n", pid);

	ppid = getppid();
	printf("ppid is %d\n", ppid);
	while(1);

	return 0;
}
