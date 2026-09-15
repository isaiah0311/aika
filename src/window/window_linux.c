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

#include <X11/XKBlib.h>
#include <X11/Xutil.h>

#include "core/application.h"
#include "input/input.h"
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
        .background_pixel = BlackPixel(window.display, screen),
        .event_mask = KeyPressMask | KeyReleaseMask | ButtonPressMask |
            ButtonReleaseMask | PointerMotionMask
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
        CWBackPixel | CWEventMask,
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
        case KeyPress:
        case KeyRelease: {
            const KeyCode key_code = event.xkey.keycode;
            int const shift_mask = event.xkey.state & ShiftMask ? 1 : 0;
            const KeySym key_sym = XkbKeycodeToKeysym(
                window.display,
                key_code,
                0,
                shift_mask);

            aika_keyboard_key key = AIKA_KEYBOARD_KEY_UNKNOWN;
            switch (key_sym) {
            case XK_quoteright:
            case XK_quotedbl:
                key = AIKA_KEYBOARD_KEY_SINGLE_QUOTE;
                break;
            case XK_plus:
            case XK_equal:
                key = AIKA_KEYBOARD_KEY_EQUAL;
                break;
            case XK_comma:
            case XK_less:
                key = AIKA_KEYBOARD_KEY_COMMA;
                break;
            case XK_minus:
            case XK_underscore:
                key = AIKA_KEYBOARD_KEY_MINUS;
                break;
            case XK_period:
            case XK_greater:
                key = AIKA_KEYBOARD_KEY_PERIOD;
                break;
            case XK_slash:
            case XK_question:
                key = AIKA_KEYBOARD_KEY_SLASH;
                break;
            case XK_space:
                key = AIKA_KEYBOARD_KEY_SPACE;
                break;
            case XK_semicolon:
            case XK_colon:
                key = AIKA_KEYBOARD_KEY_SEMICOLON;
                break;
            case XK_0:
            case XK_parenright:
                key = AIKA_KEYBOARD_KEY_0;
                break;
            case XK_1:
            case XK_exclam:
                key = AIKA_KEYBOARD_KEY_1;
                break;
            case XK_2:
            case XK_at:
                key = AIKA_KEYBOARD_KEY_2;
                break;
            case XK_3:
            case XK_numbersign:
                key = AIKA_KEYBOARD_KEY_3;
                break;
            case XK_4:
            case XK_dollar:
                key = AIKA_KEYBOARD_KEY_4;
                break;
            case XK_5:
            case XK_percent:
                key = AIKA_KEYBOARD_KEY_5;
                break;
            case XK_6:
            case XK_asciicircum:
                key = AIKA_KEYBOARD_KEY_6;
                break;
            case XK_7:
            case XK_ampersand:
                key = AIKA_KEYBOARD_KEY_7;
                break;
            case XK_8:
            case XK_asterisk:
                key = AIKA_KEYBOARD_KEY_8;
                break;
            case XK_9:
            case XK_parenleft:
                key = AIKA_KEYBOARD_KEY_9;
                break;
            case XK_A:
            case XK_a:
                key = AIKA_KEYBOARD_KEY_A;
                break;
            case XK_B:
            case XK_b:
                key = AIKA_KEYBOARD_KEY_B;
                break;
            case XK_C:
            case XK_c:
                key = AIKA_KEYBOARD_KEY_C;
                break;
            case XK_D:
            case XK_d:
                key = AIKA_KEYBOARD_KEY_D;
                break;
            case XK_E:
            case XK_e:
                key = AIKA_KEYBOARD_KEY_E;
                break;
            case XK_F:
            case XK_f:
                key = AIKA_KEYBOARD_KEY_F;
                break;
            case XK_G:
            case XK_g:
                key = AIKA_KEYBOARD_KEY_G;
                break;
            case XK_H:
            case XK_h:
                key = AIKA_KEYBOARD_KEY_H;
                break;
            case XK_I:
            case XK_i:
                key = AIKA_KEYBOARD_KEY_I;
                break;
            case XK_J:
            case XK_j:
                key = AIKA_KEYBOARD_KEY_J;
                break;
            case XK_K:
            case XK_k:
                key = AIKA_KEYBOARD_KEY_K;
                break;
            case XK_L:
            case XK_l:
                key = AIKA_KEYBOARD_KEY_L;
                break;
            case XK_M:
            case XK_m:
                key = AIKA_KEYBOARD_KEY_M;
                break;
            case XK_N:
            case XK_n:
                key = AIKA_KEYBOARD_KEY_N;
                break;
            case XK_O:
            case XK_o:
                key = AIKA_KEYBOARD_KEY_O;
                break;
            case XK_P:
            case XK_p:
                key = AIKA_KEYBOARD_KEY_P;
                break;
            case XK_Q:
            case XK_q:
                key = AIKA_KEYBOARD_KEY_Q;
                break;
            case XK_R:
            case XK_r:
                key = AIKA_KEYBOARD_KEY_R;
                break;
            case XK_S:
            case XK_s:
                key = AIKA_KEYBOARD_KEY_S;
                break;
            case XK_T:
            case XK_t:
                key = AIKA_KEYBOARD_KEY_T;
                break;
            case XK_U:
            case XK_u:
                key = AIKA_KEYBOARD_KEY_U;
                break;
            case XK_V:
            case XK_v:
                key = AIKA_KEYBOARD_KEY_V;
                break;
            case XK_W:
            case XK_w:
                key = AIKA_KEYBOARD_KEY_W;
                break;
            case XK_X:
            case XK_x:
                key = AIKA_KEYBOARD_KEY_X;
                break;
            case XK_Y:
            case XK_y:
                key = AIKA_KEYBOARD_KEY_Y;
                break;
            case XK_Z:
            case XK_z:
                key = AIKA_KEYBOARD_KEY_Z;
                break;
            case XK_bracketleft:
            case XK_braceleft:
                key = AIKA_KEYBOARD_KEY_LEFT_BRACKET;
                break;
            case XK_backslash:
            case XK_bar:
                key = AIKA_KEYBOARD_KEY_BACKSLASH;
                break;
            case XK_bracketright:
            case XK_braceright:
                key = AIKA_KEYBOARD_KEY_RIGHT_BRACKET;
                break;
            case XK_grave:
            case XK_asciitilde:
                key = AIKA_KEYBOARD_KEY_GRAVE;
                break;
            case XK_BackSpace:
                key = AIKA_KEYBOARD_KEY_BACKSPACE;
                break;
            case XK_Tab:
                key = AIKA_KEYBOARD_KEY_TAB;
                break;
            case XK_Return:
                key = AIKA_KEYBOARD_KEY_ENTER;
                break;
            case XK_Pause:
                key = AIKA_KEYBOARD_KEY_PAUSE;
                break;
            case XK_Scroll_Lock:
                key = AIKA_KEYBOARD_KEY_SCROLL_LOCK;
                break;
            case XK_Escape:
                key = AIKA_KEYBOARD_KEY_ESCAPE;
                break;
            case XK_Home:
                key = AIKA_KEYBOARD_KEY_HOME;
                break;
            case XK_Left:
                key = AIKA_KEYBOARD_KEY_LEFT;
                break;
            case XK_Up:
                key = AIKA_KEYBOARD_KEY_UP;
                break;
            case XK_Right:
                key = AIKA_KEYBOARD_KEY_RIGHT;
                break;
            case XK_Down:
                key = AIKA_KEYBOARD_KEY_DOWN;
                break;
            case XK_Prior:
                key = AIKA_KEYBOARD_KEY_PAGE_UP;
                break;
            case XK_Next:
                key = AIKA_KEYBOARD_KEY_PAGE_DOWN;
                break;
            case XK_End:
                key = AIKA_KEYBOARD_KEY_END;
                break;
            case XK_Select:
                key = AIKA_KEYBOARD_KEY_SELECT;
                break;
            case XK_Print:
                key = AIKA_KEYBOARD_KEY_PRINT;
                break;
            case XK_Execute:
                key = AIKA_KEYBOARD_KEY_EXECUTE;
                break;
            case XK_Insert:
                key = AIKA_KEYBOARD_KEY_INSERT;
                break;
            case XK_Help:
                key = AIKA_KEYBOARD_KEY_HELP;
                break;
            case XK_Num_Lock:
                key = AIKA_KEYBOARD_KEY_NUM_LOCK;
                break;
            case XK_KP_Multiply:
                key = AIKA_KEYBOARD_KEY_NUMPAD_MULTIPLY;
                break;
            case XK_KP_Add:
                key = AIKA_KEYBOARD_KEY_NUMPAD_PLUS;
                break;
            case XK_KP_Separator:
                key = AIKA_KEYBOARD_KEY_NUMPAD_SEPERATOR;
                break;
            case XK_KP_Subtract:
                key = AIKA_KEYBOARD_KEY_NUMPAD_MINUS;
                break;
            case XK_KP_Decimal:
                key = AIKA_KEYBOARD_KEY_NUMPAD_DECIMAL;
                break;
            case XK_KP_Divide:
                key = AIKA_KEYBOARD_KEY_NUMPAD_DIVIDE;
                break;
            case XK_KP_0:
                key = AIKA_KEYBOARD_KEY_NUMPAD_0;
                break;
            case XK_KP_1:
                key = AIKA_KEYBOARD_KEY_NUMPAD_1;
                break;
            case XK_KP_2:
                key = AIKA_KEYBOARD_KEY_NUMPAD_2;
                break;
            case XK_KP_3:
                key = AIKA_KEYBOARD_KEY_NUMPAD_3;
                break;
            case XK_KP_4:
                key = AIKA_KEYBOARD_KEY_NUMPAD_4;
                break;
            case XK_KP_5:
                key = AIKA_KEYBOARD_KEY_NUMPAD_5;
                break;
            case XK_KP_6:
                key = AIKA_KEYBOARD_KEY_NUMPAD_6;
                break;
            case XK_KP_7:
                key = AIKA_KEYBOARD_KEY_NUMPAD_7;
                break;
            case XK_KP_8:
                key = AIKA_KEYBOARD_KEY_NUMPAD_8;
                break;
            case XK_KP_9:
                key = AIKA_KEYBOARD_KEY_NUMPAD_9;
                break;
            case XK_F1:
                key = AIKA_KEYBOARD_KEY_F1;
                break;
            case XK_F2:
                key = AIKA_KEYBOARD_KEY_F2;
                break;
            case XK_F3:
                key = AIKA_KEYBOARD_KEY_F3;
                break;
            case XK_F4:
                key = AIKA_KEYBOARD_KEY_F4;
                break;
            case XK_F5:
                key = AIKA_KEYBOARD_KEY_F5;
                break;
            case XK_F6:
                key = AIKA_KEYBOARD_KEY_F6;
                break;
            case XK_F7:
                key = AIKA_KEYBOARD_KEY_F7;
                break;
            case XK_F8:
                key = AIKA_KEYBOARD_KEY_F8;
                break;
            case XK_F9:
                key = AIKA_KEYBOARD_KEY_F9;
                break;
            case XK_F10:
                key = AIKA_KEYBOARD_KEY_F10;
                break;
            case XK_F11:
                key = AIKA_KEYBOARD_KEY_F11;
                break;
            case XK_F12:
                key = AIKA_KEYBOARD_KEY_F12;
                break;
            case XK_F13:
                key = AIKA_KEYBOARD_KEY_F13;
                break;
            case XK_F14:
                key = AIKA_KEYBOARD_KEY_F14;
                break;
            case XK_F15:
                key = AIKA_KEYBOARD_KEY_F15;
                break;
            case XK_F16:
                key = AIKA_KEYBOARD_KEY_F16;
                break;
            case XK_F17:
                key = AIKA_KEYBOARD_KEY_F17;
                break;
            case XK_F18:
                key = AIKA_KEYBOARD_KEY_F18;
                break;
            case XK_F19:
                key = AIKA_KEYBOARD_KEY_F19;
                break;
            case XK_F20:
                key = AIKA_KEYBOARD_KEY_F20;
                break;
            case XK_F21:
                key = AIKA_KEYBOARD_KEY_F21;
                break;
            case XK_F22:
                key = AIKA_KEYBOARD_KEY_F22;
                break;
            case XK_F23:
                key = AIKA_KEYBOARD_KEY_F23;
                break;
            case XK_F24:
                key = AIKA_KEYBOARD_KEY_F24;
                break;
            case XK_Shift_L:
                key = AIKA_KEYBOARD_KEY_LEFT_SHIFT;
                break;
            case XK_Shift_R:
                key = AIKA_KEYBOARD_KEY_RIGHT_SHIFT;
                break;
            case XK_Control_L:
                key = AIKA_KEYBOARD_KEY_LEFT_CONTROL;
                break;
            case XK_Control_R:
                key = AIKA_KEYBOARD_KEY_RIGHT_CONTROL;
                break;
            case XK_Caps_Lock:
                key = AIKA_KEYBOARD_KEY_CAPS_LOCK;
                break;
            case XK_Meta_L:
                key = AIKA_KEYBOARD_KEY_LEFT_WINDOWS;
                break;
            case XK_Meta_R:
                key = AIKA_KEYBOARD_KEY_RIGHT_WINDOWS;
                break;
            case XK_Alt_L:
                key = AIKA_KEYBOARD_KEY_LEFT_ALT;
                break;
            case XK_Alt_R:
                key = AIKA_KEYBOARD_KEY_RIGHT_ALT;
                break;
            case XK_Delete:
                key = AIKA_KEYBOARD_KEY_DELETE;
                break;
            }

            aika_input_process_key(key, event.type == KeyPress);
            break;
        }
        case ButtonPress:
        case ButtonRelease: {
            aika_mouse_button button = AIKA_MOUSE_BUTTON_UNKNOWN;
            short wheel = 0;
            
            switch (event.xbutton.button) {
            case Button1:
                button = AIKA_MOUSE_BUTTON_LEFT;
                break;
            case Button2:
                button = AIKA_MOUSE_BUTTON_MIDDLE;
                break;
            case Button3:
                button = AIKA_MOUSE_BUTTON_RIGHT;
                break;
            case Button4:
                wheel = -1;
                break;
            case Button5:
                wheel = 1;
                break;
            }

            if (wheel) {
                aika_input_process_mouse_wheel(wheel);
            } else {
                aika_input_process_button(button, event.type == ButtonPress);
            }

            break;
        }
        case MotionNotify:
            aika_input_process_mouse_move(event.xmotion.x, event.xmotion.y);
            break;
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
