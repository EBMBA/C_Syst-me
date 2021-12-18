/**
 * @file serveur.c
 * @author METRAL EMILE 
 * @brief Server receive from 2 clients numbers and sum them before send the result to the 2 clients
 * @version 0.1
 * @date 2021-12-18
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "conf.h"

void childrenProcess();
int writer(char *file, int value);
int reader(char *file);
void handler(int signum);

int result;
int client1_socket;
struct sigaction action;



int main(int argc, char *argv[])
{
    struct sockaddr_un name;
    int down_flag = 0;
    int ret;
    int connection_socket;
    int data_socket;
    char buffer[BUFFER_SIZE];
    int nbrConnections = -1;
    pid_t pid;

    // Fais le lien entre l'action et le signal
    action.sa_handler = handler;
    sigaction(SIGUSR1, &action, NULL);

    /* Create local socket. */

    connection_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if (connection_socket == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    /*
     * For portability clear the whole structure, since some
     * implementations have additional (nonstandard) fields in
     * the structure.
     */

    memset(&name, 0, sizeof(name));

    /* Bind socket to socket name. */

    name.sun_family = AF_UNIX;
    strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);

    ret = bind(connection_socket, (const struct sockaddr *)&name,
               sizeof(name));
    if (ret == -1)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    /*
     * Prepare for accepting connections. The backlog size is set
     * to 20. So while one request is being processed other requests
     * can be waiting.
     */

    ret = listen(connection_socket, 2);
    if (ret == -1)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    /* This is the main loop for handling connections. */
    while (1)
    {
        /* Wait for incoming connection. */
        client1_socket = accept(connection_socket, NULL, NULL);

        result = 0;

        pid = fork();

        if (pid < 0)
        {
            perror("No fork");
            exit(EXIT_FAILURE);
        }

        else if (pid == 0)
        {

            /* Wait for next data packet. */
            ret = read(client1_socket, buffer, sizeof(buffer));

            /* Add received summand. */
            result += atoi(buffer);
            printf("Child %d calculating results %d\n", (int) getpid(), result);
            writer(CLIENT1_FILE, result);
        }

        else
        {
            // Close first client connection for the main process
            close(client1_socket);

            // Wait for second client connection
            data_socket = accept(connection_socket, NULL, NULL);

            /* Wait for next data packet. */
            ret = read(data_socket, buffer, sizeof(buffer));

            /* Add received summand. */
            result += atoi(buffer);
            result += reader(CLIENT1_FILE);
            writer(RESULT_FILE, result);

            // Sending signal to child process
            kill(pid, SIGUSR1);
            sleep(1);

            /* Send result. */
            sprintf(buffer, "%d", result);
            ret = write(data_socket, buffer, sizeof(buffer));

            /* Close socket. */
            close(data_socket);

            break;
        }
    }

    // Arrête le processus du fils
    printf("Parent of child %d killing him\n", pid);
    kill(pid, SIGINT);

    close(connection_socket);

    /* Unlink the socket. */

    unlink(SOCKET_NAME);

    exit(EXIT_SUCCESS);
}

int writer(char *file, int value)
{

    int fd;
    int *file_memory;
    int tab[5];
    const size_t n = sizeof tab / sizeof tab[0];

    /* Prépare un fichier suffisamment long pour contenir le nombre . */
    fd = open(file, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    lseek(fd, FILE_LENGTH + 1, SEEK_SET);
    write(fd, " ", 1);
    lseek(fd, 0, SEEK_SET);
    /* Met en correspondance le fichier et la mémoire . */
    file_memory = mmap(0, FILE_LENGTH, PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    /* Ecrit la valeur dans la zone mise en correspondance . */
    file_memory[0] = value;

    /* Libère la mémoire ( facultatif car le programme se termine ) . */
    munmap(file_memory, FILE_LENGTH);
    return EXIT_SUCCESS;
}

int reader(char *file)
{
    int fd;
    int *file_memory;
    int value;

    /* Ouvre le fichier */
    fd = open(file, O_RDWR, S_IRUSR | S_IWUSR);
    /* Met en co rr es pon da nce le fichier et la mémoire */
    file_memory = mmap(0, FILE_LENGTH, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    /* Lit l’entier , l’affiche  */

    value = file_memory[0];

    // sprintf((char *) file_memory, " %d \n ", integer);
    /* Libère la mémoire ( facultatif car le programme se termine ) . */
    munmap(file_memory, FILE_LENGTH);
    return value;
}

void handler(int signum)
{
    char buffer[BUFFER_SIZE];
    result = reader(RESULT_FILE);

    /* Send result. */
    sprintf(buffer, "%d", result);
    write(client1_socket, buffer, sizeof(buffer));

    /* Close socket. */
    close(client1_socket);
}