/**
 * \file window.h
 * \author Isaiah Lateer
 *
 * Interface for the window.
 */

#ifndef AIKA_WINDOW_HEADER
#define AIKA_WINDOW_HEADER

#include <stdbool.h>

/**
 * Creates the window.
 * 
 * \param[in] title Title.
 * \param[in] width Width.
 * \param[in] height Height.
 * \return Whether the window was created.
 */
bool aika_window_create(char const* title, int width, int height);

/** Destroys the window. */
void aika_window_destroy();

/** Sends all pending messages to the window. */
void aika_window_pump_messages();

#endif
