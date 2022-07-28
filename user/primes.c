#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void process(int p[])
{
	int prime, n;
        //close p[1] before read it
        close(p[1]);
        //read number from parent proccess
	n = read(p[0], &prime, 4);
	// if p[0] is not closed, parent process
	// is still writing to the pipeline
	if (n > 0)
	{
		// print first number, which must be prime
		printf("prime %d\n", prime);
		// pipe for current process
		int p2[2];
		pipe(p2);
		// create child process
		int pid = fork();
		// current process
		if (pid > 0)
		{
			int i;
			// read number from parent process
			while(read(p[0], &i, 4) > 0)
			{
				// if the number is not divisible by prime
				// pass it to next process
				if (i % prime != 0)
				{
					write(p2[1], &i, 4);
				}
			}
			// when parent process no long write
			// close the pipe
			close(p2[1]);
			// wait until child process end
			wait(0);
		}
		else
		{
			process(p2);
		}
	}
}

int main(int argc, char *argv[])
{
	int p[2];
	pipe(p);
	int pid = fork();
	// parent process
	if (pid > 0)
	{
		// print 1st prime
		printf("prime 2\n");
		// iterate from 3 to 35
		for (int i = 3; i <= 35; i++)
		{
			// if i is divisible by 2, drop i
			// else pass i to the pipe
			if (i % 2 != 0)
			{
				write(p[1], &i, 4);
			}
		}
		// don't forget to close pipe
		close(p[1]);
		// wait until the entire pipeline terminate
		wait(0);
	}
	// child process
	else 
	{
		// recursivly call this function
		process(p);
	}
	exit(0);
}
