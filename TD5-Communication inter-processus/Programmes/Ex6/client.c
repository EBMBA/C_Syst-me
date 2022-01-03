/**
 * @file client.c
 * @author METRAL EMILE 
 * @brief Client send number and receive a result 
 * @version 0.1
 * @date 2021-12-18
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "conf.h"


int main(int argc, char *argv[])
{
    struct sockaddr_un addr;
    int ret;
    int data_socket;
    char buffer[BUFFER_SIZE];

    /* Create local socket. */

    data_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if (data_socket == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    /*
     * For portability clear the whole structure, since some
     * implementations have additional (nonstandard) fields in
     * the structure.
     */

    memset(&addr, 0, sizeof(addr));

    /* Connect socket to socket address. */

    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);

    ret = connect(data_socket, (const struct sockaddr *)&addr,
                  sizeof(addr));
    if (ret == -1)
    {
        fprintf(stderr, "The server is down.\n");
        exit(EXIT_FAILURE);
    }

    /* Send argument. */

    ret = write(data_socket, argv[1], strlen(argv[1]) + 1);
    if (ret == -1)
    {
        perror("write");
        return EXIT_FAILURE;
    }

    /* Receive result. */

    ret = read(data_socket, buffer, sizeof(buffer));

    printf("Result = %s\n", buffer);

    /* Close socket. */
    close(data_socket);

    exit(EXIT_SUCCESS);
}