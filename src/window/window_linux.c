/**
 * \file platform_linux.c
 * \author Isaiah Lateer
 *
 * Implementation of the window interface for Linux.
 */

#include "core/platform.h"

#ifdef AIKA_PLATFORM_LINUX

#include "window/window.h"

#include <string.h>

#include <X11/Xutil.h>

#include "core/application.h"
#include "logging/logging.h"

typedef struct aika_window {
    bool created;
    Display* display;
    Window handle;
    Atom wm_delete_window;
} aika_window;

static aika_window window = { 0 };
static bool error;

/**
 * Sets a flag when an error has occurred.
 *
 * \param[in] display Connection to the X server.
 * \param[in] event Error event.
 * \return Result.
 */
static int error_handler(Display* display, XErrorEvent* event) {
    (void) display;
    (void) event;

    error = true;

    return 0;
}

/**
 * Checks if an event belongs to the given window.
 *
 * \param[in] display Connection to the X server.
 * \param[in] event Event.
 * \param[in] arg Window pointer.
 * \return Whether the event belongs to the given window.
 */
static int predicate(Display* display, XEvent* event, XPointer arg) {
    (void) display;
    return event->xany.window == *(Window*) arg;
}

/**
 * Creates the window.
 * 
 * \param[in] title Title.
 * \param[in] width Width.
 * \param[in] height Height.
 * \return Whether the window was created.
 */
bool aika_window_create(char const* title, int width, int height) {
    if (window.created) {
        aika_warn("Failed to create window: Window has already been created.");
        return false;
    }

    XErrorHandler prev_error_handler = XSetErrorHandler(error_handler);
    error = false;

    window.display = XOpenDisplay(NULL);
    if (!window.display || error) {
        aika_fatal("Failed to create window: XOpenDisplay.");

        XSetErrorHandler(prev_error_handler);

        return false;
    }

    int const screen = DefaultScreen(window.display);

    XSetWindowAttributes attributes = {
        .background_pixel = BlackPixel(window.display, screen)
    };

    window.handle = XCreateWindow(
        window.display,
        RootWindow(window.display, screen),
        0,
        0,
        width,
        height,
        0,
        DefaultDepth(window.display, screen),
        InputOutput,
        DefaultVisual(window.display, screen),
        CWBackPixel,
        &attributes);

    XSync(window.display, False);
    if (error) {
        aika_fatal("Failed to create window: XCreateWindow.");

        XCloseDisplay(window.display);
        XSetErrorHandler(prev_error_handler);

        return false;
    }

    XSizeHints hints = {
        .flags = PSize,
        .width = width,
        .height = height
    };
    
    XSetWMNormalHints(window.display, window.handle, &hints);
    
    XSync(window.display, False);
    if (error) {
        aika_fatal("Failed to create window: XSetWMNormalHints.");

        XDestroyWindow(window.display, window.handle);
        XCloseDisplay(window.display);
        XSetErrorHandler(prev_error_handler);

        return false;
    }

    window.wm_delete_window = XInternAtom(
        window.display,
        "WM_DELETE_WINDOW",
        False);

    XSync(window.display, False);
    if (error) {
        aika_fatal("Failed to create window: XInternAtom.");

        XDestroyWindow(window.display, window.handle);
        XCloseDisplay(window.display);
        XSetErrorHandler(prev_error_handler);

        return false;
    }

    int result = XSetWMProtocols(
        window.display,
        window.handle,
        &window.wm_delete_window,
        1);

    XSync(window.display, False);
    if (!result || error) {
        aika_fatal("Failed to create window: XSetWMProtocols.");

        XDestroyWindow(window.display, window.handle);
        XCloseDisplay(window.display);
        XSetErrorHandler(prev_error_handler);

        return false;
    }

    result = XStoreName(window.display, window.handle, title);

    XSync(window.display, False);
    if (!result || error) {
        aika_error("Failed to set window title: XStoreName.");
        error = false;
    }

    XMapWindow(window.display, window.handle);

    XSync(window.display, False);
    if (error) {
        aika_fatal("Failed to create window: XMapWindow.");

        XDestroyWindow(window.display, window.handle);
        XCloseDisplay(window.display);
        XSetErrorHandler(prev_error_handler);

        return false;
    }

    XSetErrorHandler(prev_error_handler);

    window.created = true;

    aika_info("Created window.");

    return true;
}

/** Destroys the window. */
void aika_window_destroy() {
    if (!window.created) {
        aika_warn("Failed to destroy window: Window has not been created yet.");
        return;
    }
    
    XUnmapWindow(window.display, window.handle);
    XDestroyWindow(window.display, window.handle);;
    XCloseDisplay(window.display);

    memset(&window, 0, sizeof(window));

    aika_info("Destroyed window.");
}

/** Sends all pending messages to the window. */
void aika_window_pump_messages() {
    if (!window.created) {
        aika_warn("Failed to pump window messages: "
            "Window has not been created yet.");
        return;
    }

    XEvent event;    
    while (XCheckIfEvent(
        window.display,
        &event,
        predicate,
        (XPointer) &window.handle)) {

        switch (event.type) {
        case ClientMessage:
            if ((Atom) event.xclient.data.l[0] == window.wm_delete_window) {
                aika_application_stop();
            }

            break;
        }
    }
}

#else
extern int empty_translation_unit;
#endif
