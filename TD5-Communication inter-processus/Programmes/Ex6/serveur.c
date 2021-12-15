#define SOCKET_NAME "/tmp/test.socket"
#define BUFFER_SIZE 12
#define FILE_LENGTH 0x100

/*
 * File server.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

void childrenProcess();
int writer(char *file, int value);
int reader(char *file);

int main(int argc, char *argv[])
{
    struct sockaddr_un name;
    int down_flag = 0;
    int ret;
    int connection_socket;
    int data_socket;
    int result;
    char buffer[BUFFER_SIZE];
    int nbrConnections = -1;
    pid_t pid[2];

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
        data_socket = accept(connection_socket, NULL, NULL);
        result = 0;
        if (data_socket == -1)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        else
        {
            nbrConnections++;
        }

        pid[nbrConnections] = fork();

        if (pid < 0)
        {
            perror("No fork");
            exit(EXIT_FAILURE);
        }

        else if (pid[0] == 0 && nbrConnections == 0)
        {

            /* Wait for next data packet. */
            ret = read(data_socket, buffer, sizeof(buffer));
            if (ret == -1)
            {
                perror("read");
                exit(EXIT_FAILURE);
            }

            /* Ensure buffer is 0-terminated. */
            buffer[sizeof(buffer) - 1] = 0;

            /* Handle commands. */

            if (!strncmp(buffer, "DOWN", sizeof(buffer)))
            {
                down_flag = 1;
                break;
            }

            /* Add received summand. */
            result += atoi(buffer);
            printf("Calculating results %d\n", nbrConnections);
            writer("./tmpfile.bin", result)
        }

        else if (pid[1] == 0 && nbrConnections == 1)
        {
            /* Wait for next data packet. */
            ret = read(data_socket, buffer, sizeof(buffer));
            if (ret == -1)
            {
                perror("read");
                exit(EXIT_FAILURE);
            }

            /* Ensure buffer is 0-terminated. */
            buffer[sizeof(buffer) - 1] = 0;

            /* Handle commands. */

            if (!strncmp(buffer, "DOWN", sizeof(buffer)))
            {
                down_flag = 1;
                break;
            }

            /* Add received summand. */
            result += atoi(buffer);
            result += reader("./tmpfile.bin");
        }

        nbrConnections++;

        if (nbrConnections > 1)
        {
            printf("Result = %d\n", result);
            break;
        }
    }

    /* Send result. */

    sprintf(buffer, "%d", result);
    ret = write(data_socket, buffer, sizeof(buffer));
    if (ret == -1)
    {
        perror("write");
        exit(EXIT_FAILURE);
    }

    strcpy(buffer, "END");
    ret = write(data_socket, buffer, strlen(buffer) + 1);
    if (ret == -1)
    {
        perror("write");
        exit(EXIT_FAILURE);
    }

    /* Close socket. */

    close(data_socket);

    /* Quit on DOWN command. */

    if (down_flag)
    {
        break;
    }

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

    /* Initialise le générateur de nombres aléatoires */
    srand(time(NULL));

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

    value = file_memory[i];

    // sprintf((char *) file_memory, " %d \n ", integer);
    /* Libère la mémoire ( facultatif car le programme se termine ) . */
    munmap(file_memory, FILE_LENGTH);
    return value;
}
