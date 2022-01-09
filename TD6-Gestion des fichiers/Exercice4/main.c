#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/**
 * @brief Return a random integer between MIN and MAX, inclusive.  Obtain randomness from /dev/random.
 *
 * @param min
 * @param max
 * @return int
 */
int random_number()
{

    /*  Store a file descriptor opened to /dev/random in a static
        variable.  That way, we don't need to open the file every time
        this function is called.
    */

    static int dev_random_fd = -1;

    char *next_random_byte;

    int bytes_to_read;

    unsigned random_value;

    /* If this is the first time this function is called, open a file
       descriptor to /dev/random. */

    if (dev_random_fd == -1)
    {

        dev_random_fd = open("/dev/random", O_RDONLY);

        if (dev_random_fd == -1)
        {
            return -1;
        }
    }

    /* Read enough random bytes to fill an integer variable. */

    next_random_byte = (char *)&random_value;

    bytes_to_read = sizeof(random_value);

    /* Loop until we've read enough bytes. Because /dev/random is filled
       from user-generated actions, the read may block and may only
       return a single random byte at a time. */

    do
    {

        int bytes_read;

        bytes_read = read(dev_random_fd, next_random_byte, bytes_to_read);

        bytes_to_read -= bytes_read;

        next_random_byte += bytes_read;

    } while (bytes_to_read > 0);

    return random_value;
}

int main(int argc, char *argv[])
{
    char *file = "/tmp/filelockExemple";
    int fd;
    struct flock lock;

    printf("opening %s\n", file);

    /* Open a file descriptor to the file.  */
    fd = open(file, O_WRONLY);
    printf("locking\n");

    /* Initialize the flock structure.  */
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;

    /* Place a write lock on the file.  */
    fcntl(fd, F_SETLKW, &lock);
    printf("locked; hit Enter to unlock... ");

    /* Write random numbers. */
     /* Wait for the user to hit Enter. */
   
    int random_n = random_number();
    char writing_number2[100];

    sprintf(writing_number2, "%d", random_n);
    printf("%s", writing_number2);

    // writing in the file 
    write(fd, writing_number2, sizeof(writing_number2));
    getchar() ;

    printf("unlocking\n");

    /* Release the lock. */
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLKW, &lock);

    close(fd);

    return 0;
}