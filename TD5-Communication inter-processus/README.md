
***CPE Lyon - 3ICS - Année 2021/22***

***Administration des systèmes Linux***

# TD5 – Communication inter-processus

Ce TD aborde les différentes approches permettant a deux processus de communiquer et d’échanger
des données.

## 1. Signaux

Les signaux sont des mécanismes permettant de manipuler et de communiquer avec des processus sous Linux. Le sujet des signaux est vaste ; nous traiterons ici quelques uns des signaux et techniques utilisés pour contrôler les processus.

Un signal est un message spécial envoyé à un processus. Les signaux sont asynchrones lorsqu’un processus reçoit un signal, il le traite immédiatement, sans même terminer la fonction ou la ligne de code en cours . 

Il y a plusieurs douzaines de signaux différents, chacun ayant une signification différente. Chaque type de signal est caractérisé par son numéro de signal, mais au sein
des programmes, on y fait souvent référence par un nom. Sous Linux, ils sont définis dans /usr/include/bits/signum.h (vous ne devriez pas inclure ce fichier directement dans vos programmes, utilisez plutôt <signal.h> ).

Lorsqu’un processus reçoit un signal, il peut agir de différentes façons, selon l’action enregistrée pour le signal. Pour chaque signal, il existe une action par défaut, qui détermine ce qui arrive au processus si le programme ne spécifie pas d’autre comportement. Pour la plupart des signaux, le programme peut indiquer un autre comportement – soit ignorer le signal, soit appeler un gestionnaire de signal, fonction chargée de traiter le signal. Si un gestionnaire de signal est utilisé, le programme en cours d’exécution est suspendu, le gestionnaire est exécuté, puis, une fois celui-ci terminé, le programme reprend.

Le système Linux envoie des signaux aux processus en réponse à des conditions spécifiques.

Par exemple, SIGBUS (erreur de bus), SIGSEGV (erreur de segmentation) et SIGFPE (exception de virgule flottante) peuvent être envoyés à un programme essayant d’effectuer une action non autorisée. L’action par défaut pour ces trois signaux est de terminer le processus et de produire un ficher core. (coredump)

Comme les signaux sont asynchrones, le programme principal peut être dans un état très fragile lorsque le signal est traité et donc pendant l’exécution du gestionnaire de signal. C’est pourquoi vous devriez éviter d’effectuer des opérations d’entrées/sorties ou d’appeler la plupart des fonctions système ou de la bibliothèque C depuis un gestionnaire de signal. Un gestionnaire de signal doit effectuer le minimum nécessaire au traitement du signal, puis repasser le contrôle au programme principal (ou terminer le programme). Dans la plupart des cas, cela consiste simplement à enregistrer que le signal est survenu. Le programme principal vérifie alors périodiquement si un
signal a été reçu et réagit en conséquence.

Il est possible qu’un gestionnaire de signal soit interrompu par l’arrivée d’un autre signal.

Bien que cela puisse sembler être un cas rare, si cela arrive, il peut être très difficile de diagnostiquer et résoudre le problème Un processus peut également envoyer des signaux à un autre processus. Une utilisation courante de ce mécanisme est de terminer un autre processus en lui envoyant un signal SIGTERM ou SIGKILL . Une autre utilisation courante est d’envoyer une commande à un programme en cours d’exécution. Deux signaux ”définis par l’utilisateur“ sont réservés à cet effet : SIGUSR1 et SIGUSR2. Le signal SIGHUP est également parfois utilisé dans ce but, habituellement pour réveiller un programme inactif ou provoquer une relecture du fichier de configuration.

-   [1A] La fonction sigaction permet de définir un handler pour un signal. En respectant les points particuliers mis en exergue dans les paragraphes précédents vous devez :

Écrire un programme qui se "forke" en 5 enfants.

Toutes les secondes sur une durée de 30 secondes, le processus père doit envoyer un signal
SIGUSR1 a l’un des enfants.

Le processus recevant le signal SIGUSR1 doit afficher le message « Hello from PID », PID
étant le pid du processus enfant.

Une fois les 30 secondes écoule, le processus parent doit envoyer un signal SIGINT aux 5
processus enfants (Attention aux processus Zombie)


```C
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

int childProcess()
{
    while (1)
    {
        sleep(1);
    }
    // exit(0);
}

void handler(int signum)
{
    printf(" Hello from PID %d\n", (int)getpid());
}

int main(int argc, char *argv[])
{

    /* fork a child process */
    struct sigaction action;
    pid_t pid[5] = {0};
    int i = 0;
    int nbrFils = 5;
    action.sa_handler = handler;

    sigaction(SIGUSR1, &action, NULL);

    for (i = 0; i < nbrFils; i++)
    {
        pid[i] = fork();
        srand(time(0));
        if (pid[i] < 0)
        {
            fprintf(stderr, "Fork Failed");
            return 1;
        }

        else if (pid[i] == 0)
        {
            childProcess();
        }

    }
    for (int n = 0; n < 30; n++)
    {
        int num = rand() % 4;
        kill(pid[num], SIGUSR1);
        sleep(1);
    }
    for (int p = 0; p < 4; p++)
    {
        kill(pid[p], SIGINT);
    }
    
}
```

-   [1B] Il existe une fonction signal qui permet de faire presque la même chose que
sigaction. Pourtant son utilisation est fortement déconseillé. Pourquoi ?
```
La fonction signal() n’empêche pas (nécessairement) les autres signaux d’arriver pendant l’exécution du gestionnaire actuel; sigaction() peut bloquer d’autres signaux jusqu’à ce que le gestionnaire actuel revienne.

La fonction signal() réinitialise (habituellement) l’action signal à SIG_DFL (par défaut) pour presque tous les signaux.

Le comportement exact du signal() varie selon les systèmes - et les normes permettent ces variations.
```

La communication inter-processus (inter-process communication, IPC) consiste à transférer des données entre les processus. Il existe au moins cinq types de communication inter-processus :
-   La mémoire partagée permet aux processus de communiquer simplement en lisant ou
écrivant dans un emplacement mémoire prédéfini.
-   La mémoire mappée est similaire à la mémoire partagée, excepté qu’elle est associée à un
fichier.
-   Les tubes permettent une communication séquentielle d’un processus à l’autre.
-   Les files FIFO sont similaires aux tubes excepté que des processus sans lien peuvent communiquer car le tube reçoit un nom dans le système de fichiers.
-   Les sockets permettent la communication entre des processus sans lien, pouvant se trouver sur des machines distinctes.

## 2- Mémoire partagée

Une des méthodes de communication inter processus les plus simples est d’utiliser la
mémoire partagée. La mémoire partagée permet à deux processus ou plus d’accéder à la même zone
mémoire comme s’ils avaient appelé malloc et avaient obtenu des pointeurs vers le même espace
mémoire. Lorsqu’un processus modifie la mémoire, tous les autres processus voient la modification.
La mémoire partagée est la forme de communication inter processus la plus rapide car tous
les processus partagent la même mémoire. L’accès à cette mémoire partagée est aussi rapide que
l’accès à la mémoire non partagée du processus et ne nécessite pas d’appel système ni d’entrée dans
le noyau. Elle évite également les copies de données inutiles.

Comme le noyau ne coordonne pas les accès à la mémoire partagée, vous devez mettre en place votre propre synchronisation. Par exemple, un processus ne doit pas effectuer de lecture avant
que des données aient été écrites et deux processus ne doivent pas écrire au même emplacement en
même temps. Une stratégie courante pour éviter ces conditions de concurrence est d’utiliser des
sémaphores.

La commande ipcs affichera la liste des segments mémoire en cours d’utilisation. Nous n’aborderons pas la notion de sémaphore, mais il est possible d’utiliser la mémoire partagée pour qu’un processus père et sont fils échangent des données. La synchronisation pourra se
faire via les signaux.

-   [2A] En vous basant le fichier TD5-shared_memory.c et en réutilisant le code du TD sur les processus (ou celui de l’exercice précédent) vous devez :

Ecrire un programme qui se « forke » (un seul processus fils)

Le processus parent devra afficher un nombre aléatoire compris entre 0 et 100 et l’écrire dans la mémoire partagée.

Après-quoi le processus parent devra envoyer un signal SIGUSR1 au processus fils pour lui signaler que des données sont disponibles dans la mémoire partagée.

Le processus fils devra lire les données disponibles et afficher son pid en même temps que
les données lues.

Après l’envoi de 5 valeurs aléatoires au processus fils, le processus parent mettra fin au
processus fils, et se terminera.

## 3- Mémoire mappée

La mémoire mappée permet à différents processus de communiquer via un fichier partagé.

Bien que vous puissiez concevoir l’utilisation de mémoire mappée comme étant à celle d’un segment de mémoire partagée avec un nom, vous devez être conscient qu’il existe des différences techniques. La mémoire mappée peut être utilisée pour la communication inter-processus ou comme un moyen pratique d’accéder au contenu d’un fichier.

Vous pouvez vous représenter la mémoire mappée comme l’allocation d’un tampon contenant la totalité d’un fichier, la lecture du fichier dans le tampon, puis (si le tampon est modifié) l’écriture de celui-ci dans le fichier. Linux gère les opérations de lecture et d’écriture à votre place.

-   [3A] Pour mettre en correspondance un fichier ordinaire avec la mémoire d’un processus, utilisez l’appel mmap. Cette fonction accepte 6 paramètres. Donnez le rôle de chacun des paramètres avec les valeurs possibles.
```
void * mmap (void *address, size_t length, int protect, int flags, int filedes, off_t offset)

    Adresse de départ en mémoire virtuelle.
    Taille de la projection.
    Protection (PROT_EXEC, PROT_READ, PROT_WRITE, PROT_NONE).
    Drapeau (MAP_SHARED, MAP_PRIVATE, MAP_POPULATE...).
    Descripteur de fichier.
    Position dans ce fichier.
```
-   [3B] Que signifie la ligne « PROT_READ | PROT_WRITE » dans le fichier reader.c.  `C'est pour la lecture et l’écriture de la région cartographiée.`
-   [3C] A quoi sert le drapeau MAP_SHARED ? `Ce drapeau est utilisé pour partager le mappage avec tous les autres processus, qui sont mappés à cet objet.`
-   [3D] Utilisez les fichiers TD5-reader.c et TD5-writer.c pour écrire deux programmes. Le premier programme écrira sous forme binaire le contenu d’un tableau d’entiers de 5 valeurs aléatoires dans la mémoire mappée. Le second programme devra lire ces valeurs depuis la mémoire mappée, et les afficher.

-   [3E] Que se passe-t-il si le fichier de « mapping » se trouve sur un partage réseau NTFS ou SMB ? Quelles perspectives entrevoyez vous dans l’usage d’une mémoire mappé par rapport a une mémoire partagée ?

## 4- Tubes (aka « pipes »)

Un tube est un dispositif de communication qui permet une communication à sens unique. Les données écrites sur l’« extrémité d’écriture » du tube sont lues depuis l’« extrémité de lecture».

Les tubes sont des dispositifs séquentiels ; les données sont toujours lues dans l’ordre où elles ont été écrites. Typiquement, un tube est utilisé pour la communication entre deux threads d’un même processus ou entre processus père et fils.

Dans un shell, le symbole | crée un tube. Par exemple, cette commande provoque la création par le shell de deux processus fils, l’un pour ls et l’autre pour less :

ls | less

Pour créer un tube, appelez la fonction pipe (man 3 pipe devrait vous être utile). L’appel à pipe stocke le descripteur de fichier en lecture à l’indice zéro et le descripteur de fichier en écriture à l’indice un.

```C
int pipe_fds [2];
pipe ( pipe_fds ) ;
```

Un appel à pipe crée des descripteurs de fichiers qui ne sont valide qu’au sein du processus appelant et de ses fils. Les descripteurs de fichiers d’un processus ne peuvent être transmis à des processus qui ne lui sont pas liés ; cependant, lorsqu’un processus appelle fork , les descripteurs de
fichiers sont copiés dans le nouveau processus. Ainsi, les tubes ne peuvent connecter que des processus liés.

-   [4A] Vous devez écrire un programme qui se « forke » :
- Le processus parent devra envoyer « n » valeurs aléatoires comprises entre 0 et 9 au
processus fils a travers un tube. La valeur de « n » est comprise entre 5 et 20.
- Le processus fils devra lire les toutes les valeurs qui lui sont transmises, calculer
leurs somme et quitter.
-Le processus parent doit se terminer lorsque le processus fils a fini sa tache.
● [4B] Quelle est le rôle/intérêt de la commande dup2 ?
● [4C] A quoi servent les commandes popen et pclose ?
5- Tubes nommés (FIFO, aka « named pipe »)
Une file premièr entré, premier sorti (first-in, first-out, FIFO) est un tube qui dispose d’un
nom dans le système de fichiers. Tout processus peut ouvrir ou fermer la FIFO ; les processus
raccordés aux extrémités du tube n’ont pas à avoir de lien de parenté. Les FIFO sont également
appelés canaux nommés (named pipe).
Vous pouvez créer une FIFO via la commande mkfifo . Indiquez l’emplacement où elle doit
être créée sur la ligne de commande. Par exemple, créez une FIFO dans /tmp/fifo en invoquant ces
commandes : mkfifo
Par exemple sur un premier terminal :
mkfifo /tmp/fifo
cat < /tmp/fifo
Dans un second terminal :
echo  "Hello world"  > /tmp/fifo
Pour créer une FIFO par programmation, utilisez la fonction mkfifo (man 3 mkfifo ?).
L’accès à une FIFO se fait de la même façon que pour un fichier ordinaire. Pour communiquer via
une FIFO, un programme doit l’ouvrir en écriture. Il est possible d’utiliser des fonction d’E/S de
bas niveau ( open , write , read , close , etc.) ou des fonctions d’E/S de la bibliothèque C ( fopen,
fprintf, fscanf, fclose, etc.).
Page 5/7
● [5A] Vous devez écrire un premier programme appelé send_rand qui écrit « n » valeurs
aléatoires dans un tube nommé. Ce programme prendra en argument le nom du tube, et l'option -n
qui sera suivi du nombre de valeurs aléatoires a envoyer dans le tube. Ce programme devra créer le
tube dans le répertoire /tmp si celui-ci n'existe pas déjà.
Exemple de syntaxe : send_rand mypipe -n 30 qui enverra 30 valeurs aléatoires dans le tube
nommé « mypipe »
● [5B] Le second programme appelé get_rand lira toutes les valeurs présentes dans le tube,
et affichera la moyenne des valeurs avant de quitter.
6- Socket
Un socket est un dispositif de communication bidirectionnel pouvant être utilisé pour
communiquer avec un autre processus sur la même machine ou avec un processus s’exécutant sur
d’autres machines. Nous n’aborderons ici que les sockets UNIX (PF_UNIX) , aussi appelés sockets
locaux, que vous avez entrevu dans le TD sur MySQL. Pour les sockets réseaux (PF_INET), vous
êtes invité a retourner voir le code concernant les projets du module DEV-INF-2.
Les sockets mettant en relation des processus situés sur le même ordinateur peuvent utiliser
l’espace de nommage local représenté par les constantes PF_LOCAL et PF_UNIX. Ils sont appelés
sockets locaux ou sockets de domaine UNIX. Leur adresse de socket, un nom de fichier, n’est
utilisée que lors de la création de connexions.
Le nom du socket est spécifié dans une struct sockaddr_un . Vous devez positionner le
champ sun_family à AF_LOCAL, qui représente un espace de nommage local. Le champ sun_path
spécifie le nom de fichier à utiliser et peut faire au plus 108 octets de long. La longueur réelle de la
struct sockaddr_un doit être calculée en utilisant la macro SUN_LEN .
Tout nom de fichier peut être utilisé, mais le processus doit avoir des autorisations
d’écriture sur le répertoire qui permettent l’ajout de fichiers. Pour se connecter à un socket, un
processus doit avoir des droits en lecture sur le fichier. Même si différents ordinateurs peuvent
partager le même système de fichier, seuls des processus s’exécutant sur le même ordinateur
peuvent communiquer via les sockets de l’espace de nommage local.
Le seul protocole permis pour l’espace de nommage local est 0.
Comme il est stocké dans un système de fichiers, un socket local est affiché comme un
fichier. Avec le type s (srwxrwx—x).
● [6A] En utilisant les sockets UNIX, vous devez écrire un programme client qui enverra
une valeur numérique unique a un programme serveur via les sockets UNIX. Ce programme sera
exécuté simultanément 2 fois. Les 2 processus contacteront le même serveur et chacun devra
envoyer une valeur a un processus serveur via les sockets.
● [6B] Le processus serveur attendra d’avoir deux valeurs numériques. Il devra alors
calculer la somme des deux valeurs, l’afficher et aussi la retourner aux processus clients, qui a leurs
tours, afficheront cette somme.
Page 6/7
Pour approfondir d’autres approches ;
http://jean-luc.massat.perso.luminy.univ-amu.fr/ens/docs/IPC.html
https://www.geeksforgeeks.org/ipc-using-message-queues/
https://upsilon.cc/~zack/teaching/1415/progsyst/cours-10-sysv-ipc.pdf
Page 7/7
