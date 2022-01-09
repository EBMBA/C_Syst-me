***CPE Lyon - 3ICS - Année 2021/22***
***Administration des systèmes Linux***
# TD2 – Service MySQL/MariaDB
***Vous devrez répondre a toutes les questions et rendre un document numérique avec vos réponses.***
***Emile METRAL***

L’objectif de ce TD est :
-   D’installer le service mysql/mariadb et de le sécuriser.
-   D’injecter des DB dans le serveur et de gérer les sauvegardes.
-   De gérer les accès aux différentes bases de données
-   D’analyser l’intérêt d’un serveur esclave et de le mettre en place.

- [TD2 – Service MySQL/MariaDB](#td2--service-mysqlmariadb)
  - [1- Récupérez et déployez la VM (CentOS 8 Minimal) , ou installer une CentOS 7 ou 8 a partir de l’ISO](#1--récupérez-et-déployez-la-vm-centos-8-minimal--ou-installer-une-centos-7-ou-8-a-partir-de-liso)
  - [2- Désactivation  du pare-feu.](#2--désactivation--du-pare-feu)
  - [3- Installation](#3--installation)
  - [4-Prise en main](#4-prise-en-main)
  - [5- Démarrez le service mariadb.](#5--démarrez-le-service-mariadb)
  - [6- Connexion au service](#6--connexion-au-service)
  - [7-Sécurisation](#7-sécurisation)
  - [8- Création d’une DB via un script SQL.](#8--création-dune-db-via-un-script-sql)
  - [9-Gestion des comptes et accès](#9-gestion-des-comptes-et-accès)
  - [10 Sauvegarde et restauration](#10-sauvegarde-et-restauration)
  - [11- Outils graphiques.](#11--outils-graphiques)
  - [12- Réplication comme solution de sauvegarde ?](#12--réplication-comme-solution-de-sauvegarde-)
## 1- Récupérez et déployez la VM (CentOS 8 Minimal) , ou installer une CentOS 7 ou 8 a partir de l’ISO
-   La VM doit être une CentOS MINIMAL ( SANS Interface Graphique ) avec : 
    -   Une seule carte réseau branchée sur le VMNET8
    -   2 GO de RAM
    -   CPU avec 2 cores. 
## 2- Désactivation  du pare-feu.
-   Désactivez le firewall et vérifiez qu’il est bien inactif
## 3- Installation 
-   Installez le package mariadb-server `sudo yum install mariadb-server`
-   [3A] Installez le package mariadb. Que contient ce package ?
```bash
Il contient le client mariadb. 
```
-   [3B] Qu’est ce que mariadb par rapport a mysql ? `MariaDB est un fork de mysql créé après le départ d'un développeur de mysql`
## 4-Prise en main
-   [4A] Quel est le fichier de configuration de configuration de mariadb ?`/etc/my.cnf`
-   [4B] Quel est son format ?`.cnf`
-   [4C] Quel est une de ses particularités ? `Son contenu est en format clé -> valeur.`
-   [4D] Quel est le répertoire de travail utilisé par le serveur pour stocker les bases de données?`/var/lib/mysql`
-   [4E] Cette emplacement vous semble-t-il pertinent ? Justifiez votre réponse.` Ce n'est pas pertinent car le répertoire /var est fait pour stocker les données produites par les programmes du systèmes.` 
-   [4F] Proposez une configuration plus conforme a celle d’un serveur de production.`Il faudrait plutôt  stocker les bases de données dans le repertoire srv car il est fait pour stocker les données des services proposés par le système`
## 5- Démarrez le service mariadb.
-   [5A] Sur quel port est a l’écoute le service mariadb par défaut ? Quelle commande avez vous utilisée ? `netstat -tlpn | grep mysql ==> 3306`
-   [5B] Sur quelle·s IP le service est-il a l’écoute ? `Sur toutes` 
## 6- Connexion au service
Sur un système Linux, il existe 2 méthodes pour se connecter au service mariadb, l’une  d’entre elle est l’utilisation du protocole TCP/IP
-   [6A] Quelle commande vous permet de joindre votre serveur mariadb ?`mysql -u WebUser -h DBSERVER -p`
-   [6B] Que permet de faire l’option  --protocol ?`Permet définir le protocole que l'on souhaite utiliser entre le client et le serveur.`
-   [6C] Quelle est l’autre méthode permettant de joindre un service mariadb ? Quelle est sa limite ? `Par un client graphique comme MySQL Workbench `
-   [6D] Quel est le protocole utilisé par défaut par le client (cf la commande précédente ) pour joindre le service mariadb ? Comment l’avez vous prouvé ?  ` netstat -tlpn | grep mysql ==> tcp`
## 7-Sécurisation
Le service mariadb/mysqld possède son propre système de gestion des comptes et des accès.
-   [7A] Quel est le compte administrateur utilisé par mariadb ?`root sans mot de passe`
-   [7B] Que ce passe-t-il si vous tentez de vous connecter sur le service mariadb et utilisant l’IP de la VM ? (l’IP , pas l’adresse de bouclage!!) `Nous ne sommes pas autorisé à se connecter` 
-   [7C] Que ce passe-t-il si vous  utilisez l’adresse 127.0.0.1.  Comment avez vous prouvé que vous êtes bien connecté en TCP/IP. Quelle commande avez vous utilisée pour le prouver ? `On peut se connecter en local avec le compte root.  `
Quelle commande avez vous utilisé pour vous connecter ? 
-   [7D] Même question que précédemment, mais en utilisant localhost. `On peut se connecter en local avec le compte root.  `
-   [7E] Vous avez constaté que le serveur ne demande jamais de mot de passe. Sur quoi repose la sécurité des données ? Quels sont les risques sur un serveur de production hébergeant par exemple un service Apache, Tomcat ou autre et le service mariadb ? `L'accès est limité en local mais si sur le serveur d'autre service sont installés un hacker pourrait passer par celui-ci pour accéder aux bases de données. `
-   Le script mysql_secure_installation permet de sécuriser une installation de mariadb.
    -   Fixer un mdp pour le compte root
    -   Supprimer le compte anonyme
    -   Autorisez les accès distants pour le compte root
    -   Laissez la DB test sur le serveur
    -   Accepter l’application immédiate des changements (Flush des privilèges)
## 8- Création d’une DB via un script SQL.
Le script SQL WORLDDB-FINAL-UTF8.sql permet de créer des tables  et de les alimenter dans une DB préalablement crée. 
-   [8A] Créer la DB worlddb. Comment avez vous procédé ?
```sql
create database worlddb;
```
-   [8B] Affichez la liste des DB gérées par mariadb ?
```sql
show databases;
+--------------------+
| Database           |
+--------------------+
| information_schema |
| mysql              |
| performance_schema |
| worlddb            |
+--------------------+
```
-   [8C] Injecter le script  WORLDDB-FINAL-UTF8.sql.  Quelle commande avez vous 
utilisée ?
```sql
source /root/WORLDDB-FINAL-UTF8.sql;
```
-   [8D] Quelles sont les tables présentes ? Quelle commande avez vous utilisé ?
```sql
show tables;
+-------------------+
| Tables_in_worlddb |
+-------------------+
| city              |
| country           |
| countrylanguage   |
+-------------------+
```
-   [8E] Combien de pays sont stockés dans la DB ?
```sql
use worlddb; 
select count(name) from country;                                                                                                                                                            
+-------------+
| count(name) |
+-------------+
|         239 |
+-------------+
```
-   [8F] Donnez un schéma de la DB.
```sql
worlddb
|--- country
|--- city
|--- countrylanguage
```
-   [8G] Le fichier sakila-db.zip contient une DB de démonstration. Injectez cette DB dans le  serveur. Comment avez vous procédé ?
```sql
source /root/sakila-db/sakila-data.sql;
```
-   [8H] Affiché les 5 premiers acteurs de la DB sakila (table actor)
```sql
select * from actor limit 5;
+----------+------------+--------------+---------------------+
| actor_id | first_name | last_name    | last_update         |
+----------+------------+--------------+---------------------+
|        1 | PENELOPE   | GUINESS      | 2006-02-15 04:34:33 |
|        2 | NICK       | WAHLBERG     | 2006-02-15 04:34:33 |
|        3 | ED         | CHASE        | 2006-02-15 04:34:33 |
|        4 | JENNIFER   | DAVIS        | 2006-02-15 04:34:33 |
|        5 | JOHNNY     | LOLLOBRIGIDA | 2006-02-15 04:34:33 |
+----------+------------+--------------+---------------------+
```
-   [8I] Quelles sont les DB gérées par le serveur ?
```sql
SHOW databases;
+--------------------+
| Database           |
+--------------------+
| information_schema |
| mysql              |
| performance_schema |
| sakila             |
| worlddb            |
+--------------------+
```
-   [8J] Supprimez la DB sakila, et vérifiez la suppression. Quelles commandes avez vous utilisées ?
```sql
DROP DATABASE sakila; 
SHOW DATABASES;
+--------------------+
| Database           |
+--------------------+
| information_schema |
| mysql              |
| performance_schema |
| worlddb            |
+--------------------+
```
## 9-Gestion des comptes et accès
***L’accès en tant administrateur  n’est plus possible a n’importe qui, ce qui est une bonne  chose, mais maintenant les développeurs et les applications utilisant les DB sont probablement aussi bloqués. En effet, les développeurs ont la mauvaise tendance a utiliser le compte root pour travailler et a paramétrer les applications pour qu’elles utilisent le compte root sans mot de passe...***
-   [9A] Quelles seraient les conséquences possibles si l’application web, par exemple, présente des failles de sécurité ? `Extraction de la base de donnés / Corruption des données`
-   [9B] Qu’est ce qu’une injection SQL ? `Le fait d'injecter des requêtes SQL afin d'extraire/ corrompre les données des bases de données du système `
-   [9C] Créez un compte « worldbuser ». Quelle commande SQL permet de créer un 
utilisateur ?
```sql
CREATE USER 'worldbuser'@'%' IDENTIFIED BY 'password';
```
-   [9D] Donnez tous les droits a l’utilisateur « worldbuser » sur la DB « worlddb ». Quelle commande SQL permet de donner tous les droits sur une DB a un utilisateur particulier ?
```sql
GRANT ALL PRIVILEGES ON worlddb.* TO 'worldbuser'@'%';
FLUSH PRIVILEGES;
```
-   [9E] Quelle commande SQL permet de voir les permissions pour un utilisateur ? Quels sont les droits pour l’utilisateur « worlddbuser » ?
```sql
show grants for "worldbuser"@"%";
+-----------------------------------------------------------------------------------------------------------+
| Grants for worldbuser@%                                                                                   |
+-----------------------------------------------------------------------------------------------------------+
| GRANT USAGE ON *.* TO `worldbuser`@`%` IDENTIFIED BY PASSWORD '*174A73B0395057C15A6496863E8DDB59ADE89582' |
| GRANT ALL PRIVILEGES ON `worlddb`.* TO `worldbuser`@`%`                                                   |
+-----------------------------------------------------------------------------------------------------------+
```
-   [9F] A quoi sert la commande SQL « FLUSH PRIVILEGES ; » ?`Appliquer les modifications sur les privilèges sans redémarrer ou recharger le service.`
-   [9G] Comment prouver que les accès sont bien fonctionnels pour le compte « worlddbuser ». `En se connectant avec le compte et en lançant une requête sql comme un use worlddb; select count(name) from country; `
-   [9H] Quel sont les tables accessibles avec le compte « worlddbuser ». Quelles commandes
avez vous utilisées pour le prouver ?
```sql
Avec l utilisateur worldbuser:
show databases;
+--------------------+
| Database           |
+--------------------+
| information_schema |
| worlddb            |
+--------------------+

use worlddb;
SHOW tables ;
+-------------------+
| Tables_in_worlddb |
+-------------------+
| city              |
| country           |
| countrylanguage   |
+-------------------+
```
-   [9I] Quelle commande SQL permet de révoquer des droits ? ` REVOKE droit ON relation FROM utilisateur `
-   [9J] A quoi sert le script mysql_setpermission ? `peut aider  d'ajouter des utilisateurs ou des bases de données ou changer le mot de passe d'un utilisateur mais il permet surtout de définir des droits sur les tables de manière intéractif`
-   [9K] A quoi sert le script mysqlaccess ? `Permet de vérifier les droits d'accès.`
## 10 Sauvegarde et restauration
***Les données récoltées et accumulées au sein d’une entreprise est un véritable trésor bien plus précieux que vous ne pourriez l’imaginer. C’est une véritable banque d’informations et de connaissances qui permet à l’entreprise de progresser et grossir petit à petit. Ce serait un véritable drame si des contenus importants venaient à disparaître, à être altérés ou à être visibles par des personnes malveillantes. Si les données étaient perdues, il faudrait pratiquement reprendre à zéro pour retrouver l’essentiel des informations importantes.***
-   [10A] Quels sont les principaux dangers liés à une mauvaise sécurité ? 

Pour faire un POC dans la DB worlddb:
- [10B] Changer la population de tous les pays en la fixant a 10. Donnez la 
commande utilisée.
```sql
UPDATE country
SET capital=10;
```
- [10C] Faire une sauvegarde de la DB « worlddb » en nommant le script  dump-
worldb.sql. Vous fournirez ce script avec les autres documents qui vous sont demandés.
```sql
mysqldump -u root -p --databases worlddb > dump-worldb.sql;
```
- [10D] Quelle commande est utilisable pour faire une restauration d’une DB ?
```sql
mysql -u root -p < dump-worldb.sql;
```
● Il existe d’autres méthodes pour faire des sauvegardes de DB. Le lien suivant devrait vous éclairer https://mariadb.com/kb/en/library/backup-and-restore-overview/ 
- [10E] Qu’est ce qu’une sauvegarde logique ? `C'est l'extraction des commandes permettant de restaurer  la structure, les données, les procédures et les fonctions stockées, etc.`
- [10F] Quels sont les différents type de sauvegardes physiques ? ` Incrémentales --> sauvegarde la différence de données depuis la dernière sauvegarde. Complètes --> sauvegarde l'entiereté des données.`
- [10G] Quelles autres commandes sont utilisées pour faire une sauvegarde ? `percona xtrabackup / xtrabackup` 

Les méthodes de sauvegardes précédentes procèdent a chaque fois a une sauvegarde complète des DB, ce qui peut prendre beaucoup de temps et demander beaucoup d’espace de stockage. Le lien suivant propose une autre approche pour les sauvegardes : 
https://mariadb.com/kb/en/library/incremental-backup-and-restore-with-mariabackup/
- [10H] Quel type de sauvegarde est proposé ? `La sauvegarde incrémentale est proposée.`
- [10I] Quels avantages présent-il ? `La sauvegarde incrémentale est donc plus légère que la sauvegarde complète, plus rapide également en traitement.`
- [10J] Quels sont les problèmes posés par ce type de sauvegarde ? `Cependant, en cas d'une sauvegarde incrémentale erronnée, les prochaines sauvegardes incrémentales ne pourront être effectuées. Il faudra donc refaire une sauvegarde complète après une incrémentale échouée.`
## 11- Outils graphiques.
Les outils graphiques apportent un certains confort de travail. Il en existe de très nombreux 
souvent payant, d’autres gratuits :
* mySQLWorkbench (risque d’incompatibilité avec mariadb)
* HeidiSQL (uniquement sur Windows)
* Dbeaver
Dans la majorité des cas, il est possible de faire une connexion directe sur un serveur mariadb, ou alors de faire une connexion a travers un tunnel SSH
-   [11A] Quel est l’intérêt d’une connexion a travers un tunnel SSH ? `Les commandes effectuées, les données récupérées via ces commandes également ne peuvent être interceptées car le SSH chiffre les communications.`
 
## 12- Réplication comme solution de sauvegarde ?
***La réplication sur un serveur ou plusieurs serveurs esclaves peut être utilisée comme stratégie complémentaire a la sauvegarde. Regardez le lien suivant : https://mariadb.com/kb/en/library/replication-as-a-backup-solution/***
-   [12A] Quels sont les limites de la réplication ?`La réplication ne peut protéger contre les pertes de données ou une erreur de manipulation.`
-   [12A] Quels sont les usages courant des serveurs de réplications? `La réplication est notamment utilisée dans les cas de failover ou bien dans un système de load-balancing.`
(https://mariadb.com/kb/en/mariadb-maxscale-22-maxscale-failover-with-keepalived-and-maxctrl/)

La mise en place d’un serveur mariadb maitre avec un esclave est assez simple.
-   Créez une nouvelle VM, installez mariadb et les outils nécessaires
-   Vérifiez la connectivité entre les VMs
-   Ping ?
-   Accès a mariadb depuis la machine esclave ? 
-   Configuration du serveur maitre (/etc/my.cnf.d/server.cnf, ne pas oublier de redémarrer le service après la création du fichier)
```
[mariadb]
log-bin=mysql-bin
server-id=1
log-basename=masterSQLM
expire_logs_days=1
#binlog_expire_logs_seconds=60
#replicate_do_db = réplication de certaines db
```
-   Création d’un compte pour la réplication ou attribution des droits sur un compte existant  
sur le serveur maitre 
```sql
CREATE USER 'replicationUser'@'192.168.100.186' IDENTIFIED BY 'password';
GRANT REPLICATION SLAVE ON *.* TO replicationUser;
GRANT ALL PRIVILEGES ON *.* TO 'replicationUser'@'192.168.100.186' IDENTIFIED BY 'password' WITH GRANT OPTION;
FLUSH PRIVILEGES ;
SHOW MASTER STATUS;

FLUSH TABLES WITH READ LOCK;
SHOW MASTER STATUS;
```

```bash
mysqldump -u root -p --databases worlddb > worlddb.sql
scp worlddb.sql root@192.168.100.186:/root
Sur le slave : mysql -u root -p < worlddb.sql

```

```sql 
UNLOCK TABLES;
```

-   Configuration du slave
```
[mariadb]
server-id=2
```

```sql
CHANGE MASTER TO
  MASTER_HOST='192.168.100.8',
  MASTER_USER='replicationUser',
  MASTER_PASSWORD='password',
  MASTER_PORT=3306,
  MASTER_LOG_FILE='masterSQLM-bin.000001',
  MASTER_LOG_POS=333,
  MASTER_CONNECT_RETRY=10;

START SLAVE;

SHOW SLAVE STATUS \G
*************************** 1. row ***************************
        Slave_IO_State: Waiting for master to send event
        Master_Host: 192.168.100.8
        Master_User: replicationUser
        Master_Port: 3306
        Connect_Retry: 10
        Master_Log_File: masterSQLM-bin.000001
        Read_Master_Log_Pos: 333
        Relay_Log_File: mariadb-relay-bin.000002
        Relay_Log_Pos: 560
        Relay_Master_Log_File: masterSQLM-bin.000001
        Slave_IO_Running: Yes
        Slave_SQL_Running: Yes

```

-   On teste la configuration 
Sur le master :
```sql
create database test;
show databases;
+--------------------+
| Database           |
+--------------------+
| information_schema |
| mysql              |
| performance_schema |
| test               |
| worlddb            |
+--------------------+
```
Sur le slave 
```sql
show databases;
+--------------------+
| Database           |
+--------------------+
| information_schema |
| mysql              |
| performance_schema |
| test               |
| worlddb            |
+--------------------+
```