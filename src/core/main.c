/**
 * \file main.c
 * \author Isaiah Lateer
 *
 * Entry point for the game.
 */

#include <stdlib.h>

#include "core/application.h"

/**
 * Entry point for the game.
 *
 * \return Exit code.
 */
int main() {
    if (!aika_application_create()) {
        return EXIT_FAILURE;
    }

    aika_application_start();
    aika_application_destroy();

    return EXIT_SUCCESS;
}
