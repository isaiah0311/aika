/**
 * \file platform.h
 * \author Isaiah Lateer
 *
 * Definitions for platform.
 */

#ifndef AIKA_PLATFORM_HEADER
#define AIKA_PLATFORM_HEADER

#ifdef _WIN32
#define AIKA_PLATFORM_WINDOWS
#elif defined(__linux__)
#define AIKA_PLATFORM_LINUX
#else
#error "[FATAL] Failed to compile: Platform not supported."
#endif

#endif
