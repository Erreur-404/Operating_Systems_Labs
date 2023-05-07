#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

const char *FIFO_PATH = "/tmp/tp2_fifo";

int main() {
    if (fork() == 0) {
        mkfifo(FIFO_PATH, 0600);
        if (fork() == 0) {
            // P1
            int outputFd = open(FIFO_PATH, O_WRONLY);
            dup2(outputFd, 1);
            close(outputFd);
            execlp("rev", "rev", "In.txt", NULL);
            _exit(1);
        }
        int inputFd = open(FIFO_PATH, O_RDONLY);
        dup2(inputFd, 0);
        int outFile = open("Outn.txt", O_WRONLY | O_CREAT | O_TRUNC, 0660);
        dup2(outFile, 1);
        close(outFile);
        close(inputFd);
        execlp("rev", "rev", NULL);
        _exit(1);
    }
    wait(NULL);
    if (fork() == 0) {
        // P3
        execlp("diff", "diff", "In.txt", "Outn.txt", "-s", NULL);
        _exit(1);
    }
    wait(NULL);
    return 0;
}