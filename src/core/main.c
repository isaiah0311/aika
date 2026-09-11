/**
 * \file main.c
 * \author Isaiah Lateer
 *
 * Entry point for the game.
 */

#include <stdio.h>
#include <stdlib.h>

#include "core/platform.h"

/**
 * Entry point for the game.
 *
 * \return Exit code.
 */
int main() {
#ifdef AIKA_PLATFORM_WINDOWS
    printf("Hello, Windows!\n");
#elif defined(AIKA_PLATFORM_LINUX)
    printf("Hello, Linux!\n");
#endif
    return EXIT_SUCCESS;
}
