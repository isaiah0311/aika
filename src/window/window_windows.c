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

#include "core/application.h"
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
