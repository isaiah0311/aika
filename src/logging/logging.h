/**
 * \file logging.h
 * \author Isaiah Lateer
 *
 * Interface for logging.
 */

#ifndef AIKA_LOGGING_HEADER
#define AIKA_LOGGING_HEADER

#ifdef _MSC_VER
#include <sal.h>
#define AIKA_PRINTF_FORMAT _Printf_format_string_
#define AIKA_PRINTF_ATTRIBUTE
#elif defined(__GNUC__) || defined(__clang__)
#define AIKA_PRINTF_FORMAT
#define AIKA_PRINTF_ATTRIBUTE __attribute__((format(printf, 2, 3)))
#else
#define AIKA_PRINTF_FORMAT
#define AIKA_PRINTF_ATTRIBUTE
#endif

typedef enum aika_log_level {
    AIKA_LOG_LEVEL_FATAL,
    AIKA_LOG_LEVEL_ERROR,
    AIKA_LOG_LEVEL_WARN,
    AIKA_LOG_LEVEL_INFO
#ifndef NDEBUG
    , AIKA_LOG_LEVEL_DEBUG
#endif
} aika_log_level;

/**
 * Writes a log message.
 *
 * \param[in] level Severity level.
 * \param[in] msg Message.
 */
void aika_log_write(
    aika_log_level level,
    AIKA_PRINTF_FORMAT char const* msg, ...) AIKA_PRINTF_ATTRIBUTE;

#define aika_fatal(...) aika_log_write(AIKA_LOG_LEVEL_FATAL, ##__VA_ARGS__)
#define aika_error(...) aika_log_write(AIKA_LOG_LEVEL_ERROR, ##__VA_ARGS__)
#define aika_warn(...) aika_log_write(AIKA_LOG_LEVEL_WARN, ##__VA_ARGS__)
#define aika_info(...) aika_log_write(AIKA_LOG_LEVEL_INFO, ##__VA_ARGS__)

#ifdef NDEBUG
#define aika_debug(...) ((void) 0)
#else
#define aika_debug(...) aika_log_write(AIKA_LOG_LEVEL_DEBUG, ##__VA_ARGS__)
#endif

#endif
