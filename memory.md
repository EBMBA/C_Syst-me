# 1. Processus 
Une instance courante d’un programme est appelée un processus. Si vous avez deux fenêtres de terminal qui s’affichent sur votre écran, alors vous exécutez probablement le même programme de terminal deux fois — vous avez deux processus de terminal. Chaque fenêtre de terminal exécute probablement un shell ; chaque shell exécutant est un autre process. Lorsque vous invoquez une commande à partir d’un interpréteur de commandes, le programme correspondant est exécuté dans un nouveau processus ; le processus shell reprend lorsque ce processus se termine.

Les programmeurs avancés utilisent souvent plusieurs processus coopérants dans une seule application pour permettre à l’application de faire plus d’une chose à la fois, pour augmenter la robustesse de l’application, et de faire usage de programmes déjà existants.

La plupart des fonctions de manipulation de processus décrites dans ce chapitre sont similaires à celles des autres systèmes UNIX. La plupart sont déclarés dans le fichier d’en-tête ; vérifiez la page de manuel pour chaque fonction.

Un processus zombie est un processus qui s’est terminé mais qui n’a pas encore été nettoyé. C’est la responsabilité du processus parent de nettoyer ses enfants zombies.

# 2. Threads

Les threads, comme les processus, sont un mécanisme qui permet à un programme de faire plus d’une chose à la fois. Comme pour les processus, les threads semblent s’exécuter simultanément ; le noyau Linux les planifie de manière asynchrone, interrompant chaque thread de temps en temps pour donner aux autres une chance de s’exécuter.

Conceptuellement, un fil existe dans un processus. Les fils sont une unité d’exécution plus fine que les processus. Lorsque vous invoquez un programme, Linux crée un nouveau processus et dans ce processus crée un thread unique, qui exécute le programme séquentiellement. Ce thread peut créer des threads supplémentaires ; tous ces threads exécutent le même programme dans le même processus, mais chaque thread peut exécuter une partie différente du programme à tout moment.

Le processus enfant exécute initialement le programme de son parent, avec la mémoire virtuelle de son parent, les descripteurs de fichiers, et ainsi de suite copiés. Le processus enfant peut modifier sa mémoire, fermer les descripteurs de fichiers, et ainsi de suite sans affecter son parent, et vice versa. Quand un programme crée un autre thread, rien n’est copié. La création et le thread créé partagent le même espace mémoire, les descripteurs de fichiers et d’autres ressources système que l’original. Si un thread change la valeur d’une variable, par exemple, l’autre thread verra par la suite la valeur modifiée. De même, si un thread ferme un descripteur de fichier, d’autres threads peuvent ne pas lire ou écrire sur ce descripteur de fichier. Parce qu’un processus et tous ses threads ne peuvent exécuter qu’un seul programme à la fois, si un thread à l’intérieur d’un processus appelle l’une des fonctions exec, tous les autres threads sont terminés (le nouveau programme peut, bien sûr, créer de nouveaux threads).

# 3. Processus vs Threads

Pour certains programmes qui bénéficient de la concurrence, la décision d’utiliser des processus ou des fils peut être difficile. Voici quelques lignes directrices pour vous aider à décider quel modèle de concurrence convient le mieux à votre programme :

- Tous les threads d’un programme doivent exécuter le même exécutable. Un processus fils, par contre, peut exécuter un exécutable différent en appelant une fonction exec.

- Un thread errant peut nuire à d’autres threads dans le même processus car les threads partagent le même espace mémoire virtuel et d’autres ressources. Par exemple, une mémoire sauvage écrit par un pointeur non initialisé dans un thread peut corrompre la mémoire visible dans un autre thread.

- Un processus errant, d’autre part, ne peut pas le faire parce que chaque processus a une copie de l’espace mémoire du programme.

- La copie de mémoire pour un nouveau processus ajoute un surcroît de performance par rapport à la création d’un nouveau thread. Cependant, la copie n’est effectuée que lorsque la mémoire est modifiée, de sorte que la pénalité est minime si le processus fils ne lit que la mémoire.

- Les threads doivent être utilisés pour les programmes qui ont besoin de parallélisme à grains fins. Par exemple, si un problème peut être divisé en plusieurs tâches presque identiques, les threads peuvent être un bon choix. Des processus devraient être utilisés pour les programmes qui nécessitent un parallélisme plus grossier.

- Partager des données entre les threads est trivial parce que les threads partagent la même mémoire. (Cependant, il faut faire très attention pour éviter les conditions raciales, comme décrit précédemment.) Le partage de données entre les processus nécessite l’utilisation de mécanismes de IPC. Cela peut être plus lourd, mais rend les processus multiples moins susceptibles de souffrir de bogues de concurrence.

# 4. IPC 

Les signaux ne fournissent aucun moyen pour le parent de communiquer avec l’enfant, sauf via des arguments en ligne de commande et des variables d’environnement, ni aucun moyen pour l’enfant de communiquer avec le parent, sauf via le statut de sortie de l’enfant. Aucun de ces mécanismes ne permet de communiquer avec le processus de l’enfant pendant qu’il est en cours d’exécution, et ces mécanismes ne permettent pas de communiquer avec un processus en dehors de la relation parent-enfant.

La communication interprocess (IPC) est le transfert de données entre les processus. Par exemple, un navigateur Web peut demander une page Web à un serveur Web, qui envoie ensuite des données HTML. Ce transfert de données utilise généralement des sockets.

Cinq types de communication interprocess :

- La mémoire partagée permet aux processus de communiquer simplement en lisant et en écrivant à un emplacement de mémoire spécifié.

- La mémoire mappée est similaire à la mémoire partagée, sauf qu’elle est associée à un fichier dans le système de fichiers.

- Les pipes permettent une communication séquentielle d’un processus à un processus connexe.

- Les FIFOs sont similaires aux pipes, sauf que des processus non apparentés peuvent communiquer parce que le pipe reçoit un nom dans le système de fichiers.

- Les sockets supportent la communication entre des processus indépendants même sur des ordinateurs différents.

Ces types de IPC diffèrent selon les critères suivants :

- Qu’ils limitent la communication aux processus connexes (processus avec un ancêtre commun), aux processus non liés partageant le même système de fichiers, ou à tout ordinateur connecté à un réseau

- Si un processus de communication se limite à écrire des données ou à lire des données seulement

- Le nombre de processus autorisés à communiquer

- Si les processus de communication sont synchronisés par l’IPC – par exemple, un processus de lecture s’arrête jusqu’à ce que les données soient disponibles pour lire