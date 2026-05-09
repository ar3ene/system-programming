#include <iostream>
#include <random>
#include <memory>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/select.h>

#define WAIT_TIME_1 1
#define WAIT_TIME_2 4
#define STR_SIZE 32
#define STR_NUM 5


using namespace std;

void child(int id, int *fd) {
	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<unsigned int>dist(1, STR_SIZE);

	close(fd[0]);

	int wait_time = (id==1 ? WAIT_TIME_1 : WAIT_TIME_2);

	for (int t=0; t< STR_NUM; ++t) {
		int dim = dist(mt);
		unique_ptr<char[]> str(new char[dim+1]);


		for (int i=0; i<dim; ++i)
			str[i]='a'+dist(mt)%26;

		str[dim] = '\0';

		cout << "Child " << id << " writes " << str.get() << " (" << dim << ")" << endl;

		write(fd[1], &dim, sizeof(int));
		write(fd[1], str.get(), dim);

		this_thread::sleep_for(chrono::seconds(wait_time));
	}
	close(fd[1]);
	return;
}

int main(void) {
	int fd1[2], fd2[2];
	pipe(fd1);
	pipe(fd2);

	pid_t ret = fork();

	if (!ret) { //first child
		child(1, fd1);
	}
	else {
		ret = fork();
		if (!ret) { //second child
			child(2, fd2);
		}
		else
		{
			close(fd1[1]);
			close(fd2[1]);
                        fd_set rset;
                        int maxfd = ((fd1[0]>fd2[0]) ? fd1[0] : fd2[0])+1;

                        int t=0;
			while (t<2*STR_NUM) {
                                FD_ZERO (&rset);
                                FD_SET(fd1[0], &rset);
                                FD_SET(fd2[0], &rset);
                                select (maxfd, &rset, NULL, NULL, NULL);
                                if (FD_ISSET(fd1[0], &rset)) {
				int n;
				read (fd1[0], &n, sizeof(int));
				unique_ptr<char[]> str(new char[n+1]);
                                read (fd1[0], str.get(), n);
                                str[n] = '\0';
                                cout << "Parent reads from child 1: " << str.get() << " (" << n << ")" << endl;
                                ++t;
                                } else if (FD_ISSET(fd2[0], &rset)) {
                                int n;
				read (fd2[0], &n, sizeof(int));
				unique_ptr<char[]> str(new char[n+1]);
                                read (fd2[0], str.get(), n);
                                str[n] = '\0';
                                cout << "Parent reads from child 2: " << str.get() << " (" << n << ")" << endl;
                                ++t;
                                }
			}
			wait (NULL);
			wait (NULL); 
		}
	}
	return 0;

}
