#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>

sem_t* tourProducteur;
sem_t* tourConsommateur;
bool flag_de_fin = false;
int tailleTampon;
int * tampon;
int sommeChiffresGeneres = 0;
int sommeChiffresRecuperes = 0;
int nombreDeProducteurs;
int nombreDeConsommateurs;
int producteurSuivant = 0;
int consommateurSuivant = 0;

// fonction exécutée par les producteurs
void* producteur(void* pid) {
    int chiffreAleatoire;
    int *nombreDeChiffresGenere = calloc(1, sizeof(int));
    while (true) {
        chiffreAleatoire = (rand() % 9) + 1;
        sem_wait(tourProducteur);
        sommeChiffresGeneres += chiffreAleatoire;
        int index = (*(int*)pid + (nombreDeProducteurs * (*nombreDeChiffresGenere))) % tailleTampon;
        (*nombreDeChiffresGenere)++;
        tampon[index] = chiffreAleatoire;
        producteurSuivant++;
        sem_post(tourConsommateur);

        if (flag_de_fin) {
            pthread_exit(nombreDeChiffresGenere);
        }
    }
}

// fonction exécutée par les consommateurs
void* consommateur(void *cid) {
    int chiffreCourant;
    int *nombreDeChiffresRecuperes = calloc(1, sizeof(int));
    while (true) {
        sem_wait(tourConsommateur);
        int index = (*(int*)cid + (nombreDeConsommateurs * (*nombreDeChiffresRecuperes)++)) % tailleTampon;
        chiffreCourant = tampon[index];
        sommeChiffresRecuperes += chiffreCourant;
        sem_post(tourProducteur);
        consommateurSuivant++;

        if (chiffreCourant == 0) {
            pthread_exit(nombreDeChiffresRecuperes);
        }
    }
}

// fonction exécutée lorsque le signal SIGALRM est lancé
void activerFlagDeFin() {
    flag_de_fin = true;
}

// fonction main
int main(int argc, char* argv[]) {
    srand(time(NULL));

    tourProducteur = calloc(1, sizeof(sem_t));
    tourConsommateur = calloc(1, sizeof(sem_t));
    sem_init(tourProducteur, 0, 1);
    sem_init(tourConsommateur, 0, 0);

    nombreDeProducteurs = atoi(argv[1]);
    nombreDeConsommateurs = atoi(argv[2]);
    tailleTampon = atoi(argv[3]);

    tampon = calloc(tailleTampon, sizeof(int));

    int threadsPidProducteurs[nombreDeProducteurs];
    int threadsPidConsommateurs[nombreDeConsommateurs];
    pthread_t thProducteurs[nombreDeProducteurs];
    pthread_t thConsommateurs[nombreDeConsommateurs];
    // Démarrer les producteurs
    for (int i = 0; i < nombreDeProducteurs; i++) {
        threadsPidProducteurs[i] = i;
        pthread_create(&thProducteurs[i], NULL, producteur, (void*)&threadsPidProducteurs[i]);
    }
    // Démarrer les consommateurs
    for (int i = 0; i < nombreDeConsommateurs; i++) {
        threadsPidConsommateurs[i] = i;
        pthread_create(&thConsommateurs[i], NULL, consommateur, (void*)&threadsPidConsommateurs[i]);
    }

    // Armer l'alarme
    signal(SIGALRM, activerFlagDeFin);
    alarm(1);

    int nombreTotalDeChiffresGeneres = 0;
    int nombreTotalDeChiffresRecuperes = 0;
    int *valeurDeRetour;
    // Attendre la fin des producteurs
    for (int i = 0; i < nombreDeProducteurs; i++) {
        pthread_join(thProducteurs[i], (void **)&valeurDeRetour);
        printf("Producteurs i = %i\n", i);
        nombreTotalDeChiffresGeneres += (*valeurDeRetour);
        free(valeurDeRetour);
    }
    // Déposer des 0 dans le tampon
    for (int i = 0; i < nombreDeConsommateurs; i++) {
        tampon[(nombreTotalDeChiffresGeneres + i) % tailleTampon] = 0;
    }
    printf("tampon: %d, %d, %d, %d, %d\n", tampon[0], tampon[1], tampon[2], tampon[3], tampon[4]);
    // Attendre la fin des consommateurs
    for (int i = 0; i < nombreDeConsommateurs; i++) {
        pthread_join(thConsommateurs[i], (void**)&valeurDeRetour);
        printf("Consommateurs i = %i\n", i);
        nombreTotalDeChiffresRecuperes += (*valeurDeRetour); 
        free(valeurDeRetour);
    }

    printf("Somme mémorisée des chiffres produits: %d\n", sommeChiffresGeneres);
    printf("Somme mémorisée des chiffres consommés: %d\n", sommeChiffresRecuperes);
    printf("Nombre de chiffres produits par les producteurs: %d\n", nombreTotalDeChiffresGeneres);
    printf("Nombre de chiffres consommés par les consommateurs: %d\n", nombreTotalDeChiffresRecuperes);

    free(tampon);
    sem_destroy(tourProducteur);
    sem_destroy(tourConsommateur);
    return 0;
}
