# CProgramm
l s'agit d'écrire un jeu (une sorte de "roulette russe") qui consiste à avoir 2 équipes de
threads joueurs qui s'entretuent. Dès qu'une équipe s'est fait tuer TOUS ses threads
joueurs, elle a perdu la partie.
Il y aura donc toujours 2 équipes dans le jeu ni plus ni moins. Ça ne bougera pas. Par
contre, la taille N des équipes peut varier d'où ceci (obligatoirement) en tête de votre
fichier C: #define N 3. Quand votre programme commencera à tourner il faudra veiller
à vérifier que ça marche pour d'autres valeurs de N ! Par conséquent, les tableaux
utilisés dans votre programme pourraient tous être alloues dynamiquement (malloc()
ou calloc()). Ce n'est pas obligatoire dans cet exercice mais ça serait bien vu...
Déroulé du "jeu":
Le thread principal lance successivement les N threads de l'équipe1 puis les N threads
de l'équipe2. Chaque thread (joueur) devra connaitre son numéro d'équipe 1 ou 2 et
également son numéro de joueur de 0 à N-1.
Les threads représentant les joueurs sont simplistes. Ils tournent indéfiniment pour
afficher un message avec leur numéro d'équipe et leur numéro de joueur et font une
pause de 1s avant de recommencer à l'infini.
C'est toujours le thread principal qui fait tous les traitements décrits ci-après. Ensuite,
tant que la partie n'est pas terminée on joue ! Avantage à l'équipe 1 qui commence à
tirer au hasard un nombre compris entre 0 et N-1 pour identifier une potentielle victime
d'en face. Si le thread visé est "vivant" alors il est tué sinon le coup de feu est raté ! Et
on passe à l'équipe suivante et ainsi de suite. On suppose (!!) que vous aurez besoin
de tableaux pour stocker l'état des threads de chaque équipe (par exemple valeur 0
pour mort et valeur 1 pour vivant).
Après la fin d'une partie le thread principal tue le ou les survivants (Hé oui !). Et ils
vérifient bien que tout le monde est mort. Il affiche bien évidemment le score de la
partie et qui a gagné : l'équipe 1 ou 2.
Fonction C utile pour vous:
int hasard() {
static int testPremierAppel = 1;
if (testPremierAppel) {
srand((unsigned)time(NULL) ^ (unsigned)(getpid()));
testPremierAppel = 0;
}
return rand();
}
