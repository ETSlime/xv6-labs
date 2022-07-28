#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[])
{
	int pid;
	int fds[2];
	int fds2[2];
	char buf[100];
	char buf2[100];
	// create two pipes, fds and fds2
	// fds[0] is read from fds and fds[1] is write to fds
	pipe(fds);
	pipe(fds2);
	// create child process
	pid = fork();
	// child process
	if (pid == 0)
	{
		// important: close fds[1] before read fds[0]
		// because there is only one fds that can be active
		// the same for fds2
		close(fds[1]);
		close(fds2[0]);
		// read ping from fds
		read(fds[0], buf, sizeof(buf));
		// print receive ping
		printf("%d: received ping\n", getpid());
		// send pong(a byte) to fds2
		write(fds2[1], "b", 1);
		// close fds2[1] after write
		close(fds2[1]);

	}
	// parent process
	else
	{
		close(fds[0]);
		close(fds2[1]);
		// send ping(a byte) to fds
		write(fds[1], "a", 1);
		// read pong from fds2
		read(fds2[0], buf2, sizeof(buf2));
		// print receive pong
		printf("%d: received pong\n", getpid());
		close(fds[1]);
	}
	exit(0);
}

