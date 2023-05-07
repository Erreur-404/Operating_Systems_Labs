#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main() {
    if (fork() == 0) {
        // P2
        int fd[2];
        pipe(fd);
        if (fork() == 0) {
            // P1
            dup2(fd[1], 1);
            close(fd[0]);
            close(fd[1]);
            execlp("rev", "rev", "In.txt", NULL);
            _exit(1);
        }
        int outFile = open("Out.txt", O_WRONLY | O_CREAT | O_TRUNC, 0660);
        dup2(outFile, 1);
        dup2(fd[0], 0);
        close(fd[1]);
        close(fd[0]);
        close(outFile);
        execlp("rev", "rev", NULL);
        _exit(1);
    }
    wait(NULL);
    if (fork() == 0) {
        // P3
        execlp("diff", "diff", "In.txt", "Out.txt", "-s", NULL);
        _exit(1);
    }
    wait(NULL);
    return 0;
}
