/**
 * \file application.h
 * \author Isaiah Lateer
 *
 * Interface for the application.
 */

#ifndef AIKA_APPLICATION_HEADER
#define AIKA_APPLICATION_HEADER

#include <stdbool.h>

/**
 * Creates the application.
 * 
 * \return Whether the application was created.
 */
bool aika_application_create();

/** Destroys the application. */
void aika_application_destroy();

/** Starts the application. */
void aika_application_start();

/** Stops the application. */
void aika_application_stop();

#endif
