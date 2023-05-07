#include "libprocesslab/libprocesslab.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

void question1()
{
    int status;
    int nChilds = 0;
    registerProc(getpid(), getppid(), 0, 0);
    if (fork() == 0) {
        registerProc(getpid(), getppid(), 1, 1);

        if (fork() == 0) {
            registerProc(getpid(), getppid(), 2, 1);
            exit(1);
        }
        if (fork() == 0) {
            registerProc(getpid(), getppid(), 2, 2);
            exit(1);
        }
        while (wait(&status) != -1) {
            nChilds += WEXITSTATUS(status);
        }
        exit(++nChilds);
    }
    if (fork() == 0) {
        registerProc(getpid(), getppid(), 1, 2);

        if (fork() == 0) {
            registerProc(getpid(), getppid(), 2, 3);
            exit(1);
        }
        while (wait(&status) != -1) {
            nChilds += WEXITSTATUS(status);
        }
        exit(++nChilds);
    }
    if (fork() == 0) {
        registerProc(getpid(), getppid(), 1, 3);

        if (fork() == 0) {
            registerProc(getpid(), getppid(), 2, 4);
            exit(1);
        }
        if (fork() == 0) {
            registerProc(getpid(), getppid(), 2, 5);
            exit(1);
        }
        if (fork() == 0) {
            registerProc(getpid(), getppid(), 2, 6);
            exit(1);
        }
        while (wait(&status) != -1) {
            nChilds += WEXITSTATUS(status);
        }
        exit(++nChilds);
    }
    while (wait(&status) != -1) {
        nChilds += WEXITSTATUS(status);
    }
    printf("%d childs were created\n", nChilds);
    printProcRegistrations();
    execl("/usr/bin/ls", "/usr/bin/ls", "-l", NULL);
    exit(0);
}
