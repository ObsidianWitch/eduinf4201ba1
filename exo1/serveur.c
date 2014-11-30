#include <stdlib.h>
#include <stdio.h>

/*
TODO le serveur reçoit les informations du client, les affiche, et retourne au client son PID ainsi
que la chaîne reçue
*/

/**
 * Server entry point, the following arguments are required :
 *     - port on which the clients will connect
 */
int main(int argc, const char* argv[]) {
    if (argc < 2) {
        puts("Missing arguments.");
        printf("Usage : %s port\n", argv[0]);

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
