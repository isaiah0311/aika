/**
 * \file window_windows.c
 * \author Isaiah Lateer
 *
 * Implementation of the window interface for Windows.
 */

#include "core/platform.h"

#ifdef AIKA_PLATFORM_WINDOWS

#include "window/window.h"

#include <string.h>

#include <windows.h>
#include <windowsx.h>

#include "core/application.h"
#include "input/input.h"
#include "logging/logging.h"

typedef struct aika_window {
    bool created;
    HINSTANCE instance;
    HWND handle;
} aika_window;

static aika_window window = { 0 };
static char const* class_name = "aika_window_class";

/**
 * Handles window messages.
 *
 * \param[in] handle Window handle.
 * \param[in] msg Message.
 * \param[in] wparam Additional message information.
 * \param[in] lparam Additional message information.
 * \return Message dependent resulting value.
 */
static LRESULT CALLBACK window_procedure(
    HWND handle,
    UINT msg,
    WPARAM wparam,
    LPARAM lparam) {

    LRESULT result = 0;

    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_CLOSE:
        aika_application_stop();
        break;
    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP: {
        aika_keyboard_key key = AIKA_KEYBOARD_KEY_UNKNOWN;
        switch (wparam) {
        case VK_BACK:
            key = AIKA_KEYBOARD_KEY_BACKSPACE;
            break;
        case VK_TAB:
            key = AIKA_KEYBOARD_KEY_TAB;
            break;
        case VK_RETURN:
            key = AIKA_KEYBOARD_KEY_ENTER;
            break;
        case VK_SHIFT:
            if (GetAsyncKeyState(VK_RSHIFT) & 0x8000)
                key = AIKA_KEYBOARD_KEY_RIGHT_SHIFT;
            else
                key = AIKA_KEYBOARD_KEY_LEFT_SHIFT;
            break;
        case VK_CONTROL:
            if (GetAsyncKeyState(VK_RCONTROL) & 0x8000)
                key = AIKA_KEYBOARD_KEY_RIGHT_CONTROL;
            else
                key = AIKA_KEYBOARD_KEY_LEFT_CONTROL;
            break;
        case VK_MENU:
            if (GetAsyncKeyState(VK_RMENU) & 0x8000)
                key = AIKA_KEYBOARD_KEY_RIGHT_ALT;
            else
                key = AIKA_KEYBOARD_KEY_LEFT_ALT;
            break;
        case VK_PAUSE:
            key = AIKA_KEYBOARD_KEY_PAUSE;
            break;
        case VK_CAPITAL:
            key = AIKA_KEYBOARD_KEY_CAPS_LOCK;
            break;
        case VK_ESCAPE:
            key = AIKA_KEYBOARD_KEY_ESCAPE;
            break;
        case VK_SPACE:
            key = AIKA_KEYBOARD_KEY_SPACE;
            break;
        case VK_PRIOR:
            key = AIKA_KEYBOARD_KEY_PAGE_UP;
            break;
        case VK_NEXT:
            key = AIKA_KEYBOARD_KEY_PAGE_DOWN;
            break;
        case VK_END:
            key = AIKA_KEYBOARD_KEY_END;
            break;
        case VK_HOME:
            key = AIKA_KEYBOARD_KEY_HOME;
            break;
        case VK_LEFT:
            key = AIKA_KEYBOARD_KEY_LEFT;
            break;
        case VK_UP:
            key = AIKA_KEYBOARD_KEY_UP;
            break;
        case VK_RIGHT:
            key = AIKA_KEYBOARD_KEY_RIGHT;
            break;
        case VK_DOWN:
            key = AIKA_KEYBOARD_KEY_DOWN;
            break;
        case VK_SELECT:
            key = AIKA_KEYBOARD_KEY_SELECT;
            break;
        case VK_PRINT:
            key = AIKA_KEYBOARD_KEY_PRINT;
            break;
        case VK_EXECUTE:
            key = AIKA_KEYBOARD_KEY_EXECUTE;
            break;
        case VK_SNAPSHOT:
            key = AIKA_KEYBOARD_KEY_PRINT_SCREEN;
            break;
        case VK_INSERT:
            key = AIKA_KEYBOARD_KEY_INSERT;
            break;
        case VK_DELETE:
            key = AIKA_KEYBOARD_KEY_DELETE;
            break;
        case VK_HELP:
            key = AIKA_KEYBOARD_KEY_HELP;
            break;
        case '0':
            key = AIKA_KEYBOARD_KEY_0;
            break;
        case '1':
            key = AIKA_KEYBOARD_KEY_1;
            break;
        case '2':
            key = AIKA_KEYBOARD_KEY_2;
            break;
        case '3':
            key = AIKA_KEYBOARD_KEY_3;
            break;
        case '4':
            key = AIKA_KEYBOARD_KEY_4;
            break;
        case '5':
            key = AIKA_KEYBOARD_KEY_5;
            break;
        case '6':
            key = AIKA_KEYBOARD_KEY_6;
            break;
        case '7':
            key = AIKA_KEYBOARD_KEY_7;
            break;
        case '8':
            key = AIKA_KEYBOARD_KEY_8;
            break;
        case '9':
            key = AIKA_KEYBOARD_KEY_9;
            break;
        case 'A':
            key = AIKA_KEYBOARD_KEY_A;
            break;
        case 'B':
            key = AIKA_KEYBOARD_KEY_B;
            break;
        case 'C':
            key = AIKA_KEYBOARD_KEY_C;
            break;
        case 'D':
            key = AIKA_KEYBOARD_KEY_D;
            break;
        case 'E':
            key = AIKA_KEYBOARD_KEY_E;
            break;
        case 'F':
            key = AIKA_KEYBOARD_KEY_F;
            break;
        case 'G':
            key = AIKA_KEYBOARD_KEY_G;
            break;
        case 'H':
            key = AIKA_KEYBOARD_KEY_H;
            break;
        case 'I':
            key = AIKA_KEYBOARD_KEY_I;
            break;
        case 'J':
            key = AIKA_KEYBOARD_KEY_J;
            break;
        case 'K':
            key = AIKA_KEYBOARD_KEY_K;
            break;
        case 'L':
            key = AIKA_KEYBOARD_KEY_L;
            break;
        case 'M':
            key = AIKA_KEYBOARD_KEY_M;
            break;
        case 'N':
            key = AIKA_KEYBOARD_KEY_N;
            break;
        case 'O':
            key = AIKA_KEYBOARD_KEY_O;
            break;
        case 'P':
            key = AIKA_KEYBOARD_KEY_P;
            break;
        case 'Q':
            key = AIKA_KEYBOARD_KEY_Q;
            break;
        case 'R':
            key = AIKA_KEYBOARD_KEY_R;
            break;
        case 'S':
            key = AIKA_KEYBOARD_KEY_S;
            break;
        case 'T':
            key = AIKA_KEYBOARD_KEY_T;
            break;
        case 'U':
            key = AIKA_KEYBOARD_KEY_U;
            break;
        case 'V':
            key = AIKA_KEYBOARD_KEY_V;
            break;
        case 'W':
            key = AIKA_KEYBOARD_KEY_W;
            break;
        case 'X':
            key = AIKA_KEYBOARD_KEY_X;
            break;
        case 'Y':
            key = AIKA_KEYBOARD_KEY_Y;
            break;
        case 'Z':
            key = AIKA_KEYBOARD_KEY_Z;
            break;
        case VK_LWIN:
            key = AIKA_KEYBOARD_KEY_LEFT_WINDOWS;
            break;
        case VK_RWIN:
            key = AIKA_KEYBOARD_KEY_RIGHT_WINDOWS;
            break;
        case VK_APPS:
            key = AIKA_KEYBOARD_KEY_APPLICATIONS;
            break;
        case VK_SLEEP:
            key = AIKA_KEYBOARD_KEY_SLEEP;
            break;
        case VK_NUMPAD0:
            key = AIKA_KEYBOARD_KEY_NUMPAD_0;
            break;
        case VK_NUMPAD1:
            key = AIKA_KEYBOARD_KEY_NUMPAD_1;
            break;
        case VK_NUMPAD2:
            key = AIKA_KEYBOARD_KEY_NUMPAD_2;
            break;
        case VK_NUMPAD3:
            key = AIKA_KEYBOARD_KEY_NUMPAD_3;
            break;
        case VK_NUMPAD4:
            key = AIKA_KEYBOARD_KEY_NUMPAD_4;
            break;
        case VK_NUMPAD5:
            key = AIKA_KEYBOARD_KEY_NUMPAD_5;
            break;
        case VK_NUMPAD6:
            key = AIKA_KEYBOARD_KEY_NUMPAD_6;
            break;
        case VK_NUMPAD7:
            key = AIKA_KEYBOARD_KEY_NUMPAD_7;
            break;
        case VK_NUMPAD8:
            key = AIKA_KEYBOARD_KEY_NUMPAD_8;
            break;
        case VK_NUMPAD9:
            key = AIKA_KEYBOARD_KEY_NUMPAD_9;
            break;
        case VK_MULTIPLY:
            key = AIKA_KEYBOARD_KEY_NUMPAD_MULTIPLY;
            break;
        case VK_ADD:
            key = AIKA_KEYBOARD_KEY_NUMPAD_PLUS;
            break;
        case VK_SEPARATOR:
            key = AIKA_KEYBOARD_KEY_NUMPAD_SEPERATOR;
            break;
        case VK_SUBTRACT:
            key = AIKA_KEYBOARD_KEY_NUMPAD_MINUS;
            break;
        case VK_DECIMAL:
            key = AIKA_KEYBOARD_KEY_NUMPAD_DECIMAL;
            break;
        case VK_DIVIDE:
            key = AIKA_KEYBOARD_KEY_NUMPAD_DIVIDE;
            break;
        case VK_F1:
            key = AIKA_KEYBOARD_KEY_F1;
            break;
        case VK_F2:
            key = AIKA_KEYBOARD_KEY_F2;
            break;
        case VK_F3:
            key = AIKA_KEYBOARD_KEY_F3;
            break;
        case VK_F4:
            key = AIKA_KEYBOARD_KEY_F4;
            break;
        case VK_F5:
            key = AIKA_KEYBOARD_KEY_F5;
            break;
        case VK_F6:
            key = AIKA_KEYBOARD_KEY_F6;
            break;
        case VK_F7:
            key = AIKA_KEYBOARD_KEY_F7;
            break;
        case VK_F8:
            key = AIKA_KEYBOARD_KEY_F8;
            break;
        case VK_F9:
            key = AIKA_KEYBOARD_KEY_F9;
            break;
        case VK_F10:
            key = AIKA_KEYBOARD_KEY_F10;
            break;
        case VK_F11:
            key = AIKA_KEYBOARD_KEY_F11;
            break;
        case VK_F12:
            key = AIKA_KEYBOARD_KEY_F12;
            break;
        case VK_F13:
            key = AIKA_KEYBOARD_KEY_F13;
            break;
        case VK_F14:
            key = AIKA_KEYBOARD_KEY_F14;
            break;
        case VK_F15:
            key = AIKA_KEYBOARD_KEY_F15;
            break;
        case VK_F16:
            key = AIKA_KEYBOARD_KEY_F16;
            break;
        case VK_F17:
            key = AIKA_KEYBOARD_KEY_F17;
            break;
        case VK_F18:
            key = AIKA_KEYBOARD_KEY_F18;
            break;
        case VK_F19:
            key = AIKA_KEYBOARD_KEY_F19;
            break;
        case VK_F20:
            key = AIKA_KEYBOARD_KEY_F20;
            break;
        case VK_F21:
            key = AIKA_KEYBOARD_KEY_F21;
            break;
        case VK_F22:
            key = AIKA_KEYBOARD_KEY_F22;
            break;
        case VK_F23:
            key = AIKA_KEYBOARD_KEY_F23;
            break;
        case VK_F24:
            key = AIKA_KEYBOARD_KEY_F24;
            break;
        case VK_NUMLOCK:
            key = AIKA_KEYBOARD_KEY_NUM_LOCK;
            break;
        case VK_SCROLL:
            key = AIKA_KEYBOARD_KEY_SCROLL_LOCK;
            break;
        case VK_OEM_1:
            key = AIKA_KEYBOARD_KEY_SEMICOLON;
            break;
        case VK_OEM_PLUS:
            key = AIKA_KEYBOARD_KEY_EQUAL;
            break;
        case VK_OEM_COMMA:
            key = AIKA_KEYBOARD_KEY_COMMA;
            break;
        case VK_OEM_MINUS:
            key = AIKA_KEYBOARD_KEY_MINUS;
            break;
        case VK_OEM_PERIOD:
            key = AIKA_KEYBOARD_KEY_PERIOD;
            break;
        case VK_OEM_2:
            key = AIKA_KEYBOARD_KEY_SLASH;
            break;
        case VK_OEM_3:
            key = AIKA_KEYBOARD_KEY_GRAVE;
            break;
        case VK_OEM_4:
            key = AIKA_KEYBOARD_KEY_LEFT_BRACKET;
            break;
        case VK_OEM_5:
            key = AIKA_KEYBOARD_KEY_BACKSLASH;
            break;
        case VK_OEM_6:
            key = AIKA_KEYBOARD_KEY_RIGHT_BRACKET;
            break;
        case VK_OEM_7:
            key = AIKA_KEYBOARD_KEY_SINGLE_QUOTE;
            break;
        }

        aika_input_process_key(key, msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
        break;
    }
    case WM_MOUSEMOVE:
        aika_input_process_mouse_move(
            GET_X_LPARAM(lparam),
            GET_Y_LPARAM(lparam));
        break;
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP: {
        aika_mouse_button button = AIKA_MOUSE_BUTTON_UNKNOWN;
        switch (msg) {
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
            button = AIKA_MOUSE_BUTTON_LEFT;
            break;
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
            button = AIKA_MOUSE_BUTTON_RIGHT;
            break;
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
            button = AIKA_MOUSE_BUTTON_MIDDLE;
            break;
        }

        aika_input_process_button(
            button,
            msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN ||
                msg == WM_MBUTTONDOWN);
        break;
    }
    case WM_MOUSEWHEEL:
        aika_input_process_mouse_wheel(
            (GET_WHEEL_DELTA_WPARAM(wparam) < 0) ? -1 : 1);
        break;
    default:
        result = DefWindowProcA(handle, msg, wparam, lparam);
        break;
    }

    return result;
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

    static DWORD const style = WS_OVERLAPPEDWINDOW;

    window.instance = GetModuleHandleA(NULL);
    if (!window.instance) {
        aika_fatal("Failed to create window: GetModuleHandleA.");
        return false;
    }

    WNDCLASSEXA window_class = { 0 };
    if (!GetClassInfoExA(window.instance, class_name, &window_class)) {
        HICON icon = LoadIcon(window.instance, IDI_APPLICATION);

        window_class.cbSize = sizeof(window_class);
        window_class.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
        window_class.lpfnWndProc = window_procedure;
        window_class.hInstance = window.instance;
        window_class.hIcon = icon;
        window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
        window_class.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
        window_class.lpszClassName = class_name;
        window_class.hIconSm = icon;

        if (!RegisterClassExA(&window_class)) {
            aika_fatal("Failed to create window: RegisterClassExA.");
            return false;
        }
    }

    RECT area = { 0, 0, width, height };
    if (!AdjustWindowRectEx(&area, style, 0, 0)) {
        aika_fatal("Failed to create window: AdjustWindowRectEx.");

        UnregisterClassA(class_name, window.instance);

        return false;
    }

    window.handle = CreateWindowExA(
        0,
        class_name,
        title,
        style,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        area.right - area.left,
        area.bottom - area.top,
        NULL,
        NULL,
        window.instance,
        NULL);
    if (!window.handle) {
        aika_fatal("Failed to create window: CreateWindowExA.");

        UnregisterClassA(class_name, window.instance);

        return false;
    }

    ShowWindow(window.handle, SW_SHOW);

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

    DestroyWindow(window.handle);
    UnregisterClassA(class_name, window.instance);

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

    MSG msg;
    while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
}

#else
extern int empty_translation_unit;
#endif
