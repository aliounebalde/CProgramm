#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define N 3

/*typedef enum etatThread
{
    mort,
    vivant
} etatThread;*/

// structure info equipe
typedef struct infoEquipe
{
    int numEquipe;
    int numThread;
} infoEquipe;
// la routine des threads
void *f(void *arg)
{
    infoEquipe *info = (infoEquipe *)arg;
    while (1)
    {

        printf("TOUJOURS VIVANT !EQUIPE %d Joueur %d\n", info->numEquipe, info->numThread);

        sleep(1);
    }
}
// fonction qui permet de tirer un numThread au hazard
int hasard()
{
    static int testPremierAppel = 1;
    if (testPremierAppel)
    {
        srand((unsigned)time(NULL) ^ (unsigned)(getpid()));
        testPremierAppel = 0;
    }
    return rand() % N;
}

// une fonction qui permet de faire la somme des elem d'un tab( vous verrer l'utilité plus tard)
int sum(int tab[])
{
    int somme = 0;
    for (int i = 0; i < N; i++)
    {
        somme += tab[i];
    }
    return somme;
}

int main()
{

    // on cree un tableau de thread pour l'equipe 1 et 2
    pthread_t *equipe1 = NULL;
    pthread_t *equipe2 = NULL;

    // on alloue la taille des deux tableaux de maniere dynamique avec malloc
    equipe1 = malloc(sizeof(int) * N);
    if (equipe1 == NULL)
    {
        perror("pb malloc equipe1");
        exit(EXIT_FAILURE);
    }
    equipe2 = malloc(sizeof(int) * N);
    if (equipe2 == NULL)
    {
        perror("pb malloc equipe2");
        exit(EXIT_FAILURE);
    }

    // on initialise la structure qui doit contenir le numEquipe et le numThread
    infoEquipe infoEquipe1 = {1, 0};
    infoEquipe infoEquipe2 = {2, 0};

    // on cree un tab qui va contenir l'etat de chaque thread (1 vivant et 0 mort)
    int *etatThread1 = NULL;
    etatThread1 = malloc(sizeof(int) * N);
    if (etatThread1 == NULL)
    {
        perror("pb malloc etatTH 1");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < N; i++)
    {
        etatThread1[i] = 1; // mettre tous les threads en vie
    }

    int *etatThread2 = NULL;
    etatThread2 = malloc(sizeof(int) * N);
    if (etatThread2 == NULL)
    {
        perror("pb malloc etatTH 2");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < N; i++)
    {
        etatThread2[i] = 1; // mettre tous les threads en vie
    }

    // victime c'est le numThread qu'on doit essayer de tuer au hasard
    int victime = 0;

    // lancement des threads de l'equipe 1
    for (int i = 0; i < N; i++)
    {
        infoEquipe1.numThread = i;
        pthread_create(&equipe1[i], NULL, f, &infoEquipe1);
        sleep(1);
    }
    // lancement des threads de l'equipe 2
    for (int i = 0; i < N; i++)
    {
        infoEquipe2.numThread = i;
        if (pthread_create(&equipe2[i], NULL, f, &infoEquipe2) != 0)
        {
            perror("pb creation thread");
            exit(EXIT_FAILURE);
        }
        sleep(1);
    }

    // demarrage de la partie

    printf("la partie commence !!! (On tire en premier sur l'Equipe 1)\n");
    // plusieurs aproche possible ici pour tester la fin du jeu
    // dans mon cas j'essaye de voir si la somme des etatThread d'une equipe est à 0
    while (sum(etatThread1) != 0 && sum(etatThread2) != 0)
    {

        victime = hasard();
        if (etatThread1[victime] == 1)
        {
            if (pthread_cancel(equipe1[victime]) != 0)
            {
                perror("pb cancel");
                exit(EXIT_FAILURE);
            }
            printf("Equipe 1 --> Joueur %d touché \n", victime);
            etatThread1[victime] = 0;
            if (sum(etatThread1) == 0)
            {
                break;
            }
        }
        else
        {
            printf("Equipe 1 --> tir raté!\n");
        }

        victime = hasard();

        if (etatThread2[victime] == 1)
        {
            if (pthread_cancel(equipe2[victime]) != 0)
            {
                perror("pb cancel equipe2");
                exit(EXIT_FAILURE);
            }
            printf("Equipe 2 --> Joueur %d touché \n", victime);
            etatThread2[victime] = 0;
            if (sum(etatThread2) == 0)
            {
                break;
            }
        }
        else
        {
            printf("Equipe 2 --> tir raté!\n");
        }
    }
    // on tue tous les threads restants ( :)flemme d'optimiser )
    for (int i = 0; i < N; i++)
    {
        if (pthread_cancel(equipe1[i]) != 0)
        {
            perror("pb cancel");
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i < N; i++)
    {
        if (pthread_cancel(equipe2[i]) != 0)
        {
            perror("pb cancel");
            exit(EXIT_FAILURE);
        }
    }

    // On verifie l'equipe gagnante
    if (sum(etatThread1) > sum(etatThread2))
    {
        printf("Equipe 1 victoire %d a %d \n", sum(etatThread1), sum(etatThread2));
    }
    else
    {
        printf("Equipe 2 victoire %d a %d \n", sum(etatThread2), sum(etatThread1));
    }
    free(equipe1);
    free(equipe2);
    return 0;
}