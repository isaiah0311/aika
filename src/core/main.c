/**
 * \file main.c
 * \author Isaiah Lateer
 *
 * Entry point for the game.
 */

#include <stdlib.h>

#include "logging/logging.h"

/**
 * Entry point for the game.
 *
 * \return Exit code.
 */
int main() {
    aika_fatal("Message.");
    aika_error("Message.");
    aika_warn("Message.");
    aika_info("Message.");
    aika_debug("Message.");

    return EXIT_SUCCESS;
}
