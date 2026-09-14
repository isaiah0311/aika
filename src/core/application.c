/**
 * \file application.h
 * \author Isaiah Lateer
 *
 * Implementation of the application interface.
 */

#include "core/application.h"

#include <string.h>

#include "logging/logging.h"
#include "window/window.h"

typedef struct aika_application {
    bool created, running;
} aika_application;

static aika_application app = { 0 };

/** Main game loop. */
static void run() {
    while (app.running) {
        aika_window_pump_messages();
    }
}

/**
 * Creates the application.
 * 
 * \return Whether the application was created.
 */
bool aika_application_create() {
    if (app.created) {
        aika_warn("Failed to create application: "
            "Application has already been created.");
        return false;
    }

    if (!aika_window_create("Aika", 400, 300)) {
        aika_fatal("Failed to create application: aika_window_create.");
        return false;
    }

    app.created = true;

    aika_info("Created application.");

    return true;
}

/** Destroys the application. */
void aika_application_destroy() {
    if (!app.created) {
        aika_warn("Failed to destroy application: "
            "Application has not been created yet.");
        return;
    } else if (app.running) {
        aika_application_stop();
    }

    aika_window_destroy();

    memset(&app, 0, sizeof(app));

    aika_info("Destroyed application.");
}

/** Starts the application. */
void aika_application_start() {
    if (!app.created) {
        aika_warn("Failed to start application: "
            "Application has not been created yet.");
        return;
    } else if (app.running) {
        aika_warn("Failed to start application: "
            "Application is already running.");
        return;
    }

    app.running = true;

    aika_info("Started application.");

    run();
}

/** Stops the application. */
void aika_application_stop() {
    if (!app.created) {
        aika_warn("Failed to stop application: "
            "Application has not been created yet.");
        return;
    } else if (!app.running) {
        aika_warn("Failed to stop application: "
            "Application has not been started yet.");
        return;
    }

    app.running = false;

    aika_info("Stopped application.");
}
