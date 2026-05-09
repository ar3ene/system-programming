#include <iostream>
#include <random>
#include <memory>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

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
                        int val = fcntl(fd1[0], F_GETFL, 0);
                        val |= O_NONBLOCK;
                        fcntl(fd1[0], F_SETFL, val);
                        val = fcntl(fd2[0], F_GETFL, 0);
                        val |= O_NONBLOCK;
                        fcntl(fd2[0], F_SETFL, val);

                        int t=0;
			while (t<2*STR_NUM) {
				int n1, n2, nR;
				nR = read (fd1[0], &n1, sizeof(int));
                                if (nR>0) {
				unique_ptr<char[]> str1(new char[n1+1]);
                                read (fd1[0], str1.get(), n1);
                                str1[n1] = '\0';
                                cout << "Parent reads from child 1: " << str1.get() << " (" << n1 << ")" << endl;
                                ++t;
                                }
				nR = read (fd2[0], &n2, sizeof(int));
                                if (nR>0) {
				unique_ptr<char[]> str2(new char[n2+1]);
                                read (fd2[0], str2.get(), n2);
                                str2[n2] = '\0';
                                cout << "Parent reads from child 2: " << str2.get() << " (" << n2 << ")" << endl;
                                ++t;
                                }
                                this_thread::sleep_for(chrono::seconds(1));
			}
			wait (NULL);
			wait (NULL); 
		}
	}
	return 0;

}
