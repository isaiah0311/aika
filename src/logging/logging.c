/**
 * \file logging.c
 * \author Isaiah Lateer
 *
 * Implementation of the logging interface.
 */

#include "logging/logging.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 256

/**
 * Writes a log message.
 *
 * \param[in] level Severity level.
 * \param[in] msg Message.
 */
void aika_log_write(aika_log_level level, char const* msg, ...) {
    static char const* levels[] = {
        "[FATAL] ",
        "[ERROR] ",
        "[WARN] ",
        "[INFO] "
#ifndef NDEBUG
        , "[DEBUG] "
#endif
    };

    if (level < AIKA_LOG_LEVEL_FATAL || level >
#ifdef NDEBUG
    AIKA_LOG_LEVEL_INFO
#else
    AIKA_LOG_LEVEL_DEBUG
#endif
    ) {
        return;
    }

    FILE* const stream = level <= AIKA_LOG_LEVEL_ERROR ? stderr : stdout;

    char buffer[BUFFER_SIZE];
    int const prefix_count = snprintf(
        buffer,
        sizeof(buffer),
        "%s",
        levels[level]);

    va_list args;
    va_start(args, msg);
    int const msg_count = vsnprintf(
        buffer + prefix_count,
        sizeof(buffer) - prefix_count,
        msg,
        args);
    va_end(args);

    size_t const written = prefix_count + (msg_count < 0 ? 0 : msg_count);
    size_t const position = written + 1 < sizeof(buffer)
        ? written
        : sizeof(buffer) - 2;
    memcpy(buffer + position, "\n", 2);
    
    fputs(buffer, stream);
    if (stream == stderr) {
        fflush(stream);
    }
}
