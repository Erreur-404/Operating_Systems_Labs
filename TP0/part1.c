#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main () {
    int fd = open("output2.txt", O_TRUNC | O_WRONLY, 'w');
    if (fd == -1) {
        printf("Appel système open a échoué\n");
        return -1;
    }
    
    void *buffer;
    printf("Saisissez votre texte suivi de CTRL-D :\n");
    while (read(0, buffer, 1)) {
        write(fd, buffer, 1);
    } 
    close(fd);
    return 0;
}
    
