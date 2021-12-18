/**
 * @file conf.h
 * @author Emile METRAL
 * @brief define & include for client.c and server.c
 * @version 0.1
 * @date 2021-12-18
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#if !defined(CONF_H)
#define CONF_H

#define SOCKET_NAME "/tmp/test6.socket"
#define RESULT_FILE "./tmpfile.bin"
#define CLIENT1_FILE "./tmpclient1.bin"
#define BUFFER_SIZE 12
#define FILE_LENGTH 0x100

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>
#include <sys/stat.h>
#include <errno.h>

#endif // CONF_H
