#include <iostream>
#include <random>
#include <memory>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <sys/wait.h>

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
			for (int t=0; t<STR_NUM; ++t) {
				int n;
				read (fd1[0], &n, sizeof(int));
				unique_ptr<char[]> str(new char[n+1]);
                                read (fd1[0], str.get(), n);
                                str[n] = '\0';
                                cout << "Parent reads from child 1: " << str.get() << " (" << n << ")" << endl;
				read (fd2[0], &n, sizeof(int));
				str.reset(new char[n+1]);
                                read (fd2[0], str.get(), n);
                                str[n] = '\0';
                                cout << "Parent reads from child 2: " << str.get() << " (" << n << ")" << endl;

			}
			wait (NULL);
			wait (NULL); 
		}
	}
	return 0;

}
