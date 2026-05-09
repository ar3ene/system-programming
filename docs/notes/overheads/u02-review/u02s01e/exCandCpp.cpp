#include <string>
#include <vector>
#include <iostream>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int main (int argc, char ** argv) {
    /*
     * C++
     */
  
    vector<string> v;

    cout << "Vector example (C++):" << endl;    
    string s2 = "one";
    v.push_back(s2);
    s2 = "two";
    v.push_back(s2);
    s2 = "three";
    v.push_back(s2);
    for (long unsigned int i = 0; i < v.size(); ++i) {
      cout << "  " << v[i] << '\n';
    }

    /*
     * C
     */

    int l, fd[2];
    char s[100];

    // no buffering
    setbuf (stdout, 0);
    fprintf (stdout, "Pipe example (C):\n");
    
    // Create pipe
    pipe (fd);

    if (fork()!=0) {
        while (1) {
            fprintf (stdout, "  Father Write (string, 'end' to stop): ");
            scanf ("%s", s);
            l = strlen (s) + 1;
            write (fd[1], &l, sizeof(int));
            write (fd[1], s, l*sizeof(char));
            if (strcmp (s, "end")==0) break;
	    sleep (1);
        }
        wait ((int *) 0);
    } else {
        while (1) {
            read (fd[0], &l, sizeof(int));
            read (fd[0], s, l*sizeof(char));
            fprintf (stdout, "  Child Read: %s\n", s);
            if (strcmp (s, "end")==0) break;
        }
        exit (0);
    }

    return (0);
}
