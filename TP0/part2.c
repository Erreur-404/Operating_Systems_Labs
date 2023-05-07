#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
const char *WRITE_MSG = "77dbcb01f571f1c32e196c3a7d27f62e (printed using write)\n";
const char *PRINTF_MSG = "77dbcb01f571f1c32e196c3a7d27f62e (printed using printf)";

void part21 ()
{
    for (int i = 0; i < strlen(WRITE_MSG); i++) {
        printf("%c", PRINTF_MSG[i]);
    }
    write(1, WRITE_MSG, strlen(WRITE_MSG));
}

void part22 ()
{
    setvbuf(stdout, NULL, _IONBF, 0);
    part21();
}



int main (int argc, char* argv[])
{
    
    if (argc!=2)
    {   printf("Le programme a un seul paramètre : 1 ou 2\n");
        return 1;
    }
    switch (atoi(argv[1])) {
         case 1:        part21();
                        break;
         case 2:        part22();
                        break;
        default:        printf(" Le paramètre du programme est invalide\n");
                        return 1;
    }
    return 0;
}

