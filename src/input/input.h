/**
 * \file input.h
 * \author Isaiah Lateer
 *
 * Interface for input.
 */

#ifndef AIKA_INPUT_HEADER
#define AIKA_INPUT_HEADER

#include <stdbool.h>

typedef enum aika_keyboard_key {
    // Alphabet
    AIKA_KEYBOARD_KEY_A,
    AIKA_KEYBOARD_KEY_B,
    AIKA_KEYBOARD_KEY_C,
    AIKA_KEYBOARD_KEY_D,
    AIKA_KEYBOARD_KEY_E,
    AIKA_KEYBOARD_KEY_F,
    AIKA_KEYBOARD_KEY_G,
    AIKA_KEYBOARD_KEY_H,
    AIKA_KEYBOARD_KEY_I,
    AIKA_KEYBOARD_KEY_J,
    AIKA_KEYBOARD_KEY_K,
    AIKA_KEYBOARD_KEY_L,
    AIKA_KEYBOARD_KEY_M,
    AIKA_KEYBOARD_KEY_N,
    AIKA_KEYBOARD_KEY_O,
    AIKA_KEYBOARD_KEY_P,
    AIKA_KEYBOARD_KEY_Q,
    AIKA_KEYBOARD_KEY_R,
    AIKA_KEYBOARD_KEY_S,
    AIKA_KEYBOARD_KEY_T,
    AIKA_KEYBOARD_KEY_U,
    AIKA_KEYBOARD_KEY_V,
    AIKA_KEYBOARD_KEY_W,
    AIKA_KEYBOARD_KEY_X,
    AIKA_KEYBOARD_KEY_Y,
    AIKA_KEYBOARD_KEY_Z,

    // Numbers
    AIKA_KEYBOARD_KEY_0,
    AIKA_KEYBOARD_KEY_1,
    AIKA_KEYBOARD_KEY_2,
    AIKA_KEYBOARD_KEY_3,
    AIKA_KEYBOARD_KEY_4,
    AIKA_KEYBOARD_KEY_5,
    AIKA_KEYBOARD_KEY_6,
    AIKA_KEYBOARD_KEY_7,
    AIKA_KEYBOARD_KEY_8,
    AIKA_KEYBOARD_KEY_9,

    // Symbols
    AIKA_KEYBOARD_KEY_MINUS,
    AIKA_KEYBOARD_KEY_EQUAL,
    AIKA_KEYBOARD_KEY_LEFT_BRACKET,
    AIKA_KEYBOARD_KEY_RIGHT_BRACKET,
    AIKA_KEYBOARD_KEY_SLASH,
    AIKA_KEYBOARD_KEY_BACKSLASH,
    AIKA_KEYBOARD_KEY_COMMA,
    AIKA_KEYBOARD_KEY_PERIOD,
    AIKA_KEYBOARD_KEY_SINGLE_QUOTE,
    AIKA_KEYBOARD_KEY_SEMICOLON,
    AIKA_KEYBOARD_KEY_GRAVE,

    // Typewriter
    AIKA_KEYBOARD_KEY_SPACE,
    AIKA_KEYBOARD_KEY_BACKSPACE,
    AIKA_KEYBOARD_KEY_TAB,
    AIKA_KEYBOARD_KEY_ENTER,
    AIKA_KEYBOARD_KEY_LEFT_SHIFT,
    AIKA_KEYBOARD_KEY_RIGHT_SHIFT,
    AIKA_KEYBOARD_KEY_CAPS_LOCK,

    // Arrows
    AIKA_KEYBOARD_KEY_UP,
    AIKA_KEYBOARD_KEY_LEFT,
    AIKA_KEYBOARD_KEY_DOWN,
    AIKA_KEYBOARD_KEY_RIGHT,

    // System
    AIKA_KEYBOARD_KEY_ESCAPE,
    AIKA_KEYBOARD_KEY_LEFT_CONTROL,
    AIKA_KEYBOARD_KEY_RIGHT_CONTROL,
    AIKA_KEYBOARD_KEY_LEFT_ALT,
    AIKA_KEYBOARD_KEY_RIGHT_ALT,
    AIKA_KEYBOARD_KEY_LEFT_WINDOWS,
    AIKA_KEYBOARD_KEY_RIGHT_WINDOWS,

    // Other
    AIKA_KEYBOARD_KEY_PRINT_SCREEN,
    AIKA_KEYBOARD_KEY_SCROLL_LOCK,
    AIKA_KEYBOARD_KEY_PAUSE,
    AIKA_KEYBOARD_KEY_SELECT,
    AIKA_KEYBOARD_KEY_PRINT,
    AIKA_KEYBOARD_KEY_EXECUTE,
    AIKA_KEYBOARD_KEY_HELP,
    AIKA_KEYBOARD_KEY_APPLICATIONS,
    AIKA_KEYBOARD_KEY_SLEEP,
    AIKA_KEYBOARD_KEY_INSERT,
    AIKA_KEYBOARD_KEY_DELETE,
    AIKA_KEYBOARD_KEY_HOME,
    AIKA_KEYBOARD_KEY_END,
    AIKA_KEYBOARD_KEY_PAGE_UP,
    AIKA_KEYBOARD_KEY_PAGE_DOWN,

    // Function
    AIKA_KEYBOARD_KEY_F1,
    AIKA_KEYBOARD_KEY_F2,
    AIKA_KEYBOARD_KEY_F3,
    AIKA_KEYBOARD_KEY_F4,
    AIKA_KEYBOARD_KEY_F5,
    AIKA_KEYBOARD_KEY_F6,
    AIKA_KEYBOARD_KEY_F7,
    AIKA_KEYBOARD_KEY_F8,
    AIKA_KEYBOARD_KEY_F9,
    AIKA_KEYBOARD_KEY_F10,
    AIKA_KEYBOARD_KEY_F11,
    AIKA_KEYBOARD_KEY_F12,
    AIKA_KEYBOARD_KEY_F13,
    AIKA_KEYBOARD_KEY_F14,
    AIKA_KEYBOARD_KEY_F15,
    AIKA_KEYBOARD_KEY_F16,
    AIKA_KEYBOARD_KEY_F17,
    AIKA_KEYBOARD_KEY_F18,
    AIKA_KEYBOARD_KEY_F19,
    AIKA_KEYBOARD_KEY_F20,
    AIKA_KEYBOARD_KEY_F21,
    AIKA_KEYBOARD_KEY_F22,
    AIKA_KEYBOARD_KEY_F23,
    AIKA_KEYBOARD_KEY_F24,

    // Numpad
    AIKA_KEYBOARD_KEY_NUMPAD_0,
    AIKA_KEYBOARD_KEY_NUMPAD_1,
    AIKA_KEYBOARD_KEY_NUMPAD_2,
    AIKA_KEYBOARD_KEY_NUMPAD_3,
    AIKA_KEYBOARD_KEY_NUMPAD_4,
    AIKA_KEYBOARD_KEY_NUMPAD_5,
    AIKA_KEYBOARD_KEY_NUMPAD_6,
    AIKA_KEYBOARD_KEY_NUMPAD_7,
    AIKA_KEYBOARD_KEY_NUMPAD_8,
    AIKA_KEYBOARD_KEY_NUMPAD_9,
    AIKA_KEYBOARD_KEY_NUMPAD_PLUS,
    AIKA_KEYBOARD_KEY_NUMPAD_MINUS,
    AIKA_KEYBOARD_KEY_NUMPAD_MULTIPLY,
    AIKA_KEYBOARD_KEY_NUMPAD_DIVIDE,
    AIKA_KEYBOARD_KEY_NUMPAD_DECIMAL,
    AIKA_KEYBOARD_KEY_NUMPAD_SEPERATOR,
    AIKA_KEYBOARD_KEY_NUM_LOCK,

    // Miscellaneous
    AIKA_KEYBOARD_KEY_UNKNOWN,
    AIKA_KEYBOARD_KEY_COUNT
} aika_keyboard_key;

typedef enum aika_mouse_button {
    AIKA_MOUSE_BUTTON_LEFT,
    AIKA_MOUSE_BUTTON_MIDDLE,
    AIKA_MOUSE_BUTTON_RIGHT,
    AIKA_MOUSE_BUTTON_UNKNOWN,
    AIKA_MOUSE_BUTTON_COUNT
} aika_mouse_button;

/** Clears the current and previous input states. */
void aika_input_clear();

/** Copies the current input state into the previous input state. */
void aika_input_save_state();

/**
 * Processes a key event.
 *
 * \param[in] key Keyboard key.
 * \param[in] pressed Whether the key was pressed.
 */
void aika_input_process_key(aika_keyboard_key key, bool pressed);

/**
 * Processes a button event.
 *
 * \param[in] button Mouse button.
 * \param[in] pressed Whether the button was pressed.
 */
void aika_input_process_button(aika_mouse_button button, bool pressed);

/**
 * Processes a mouse motion event.
 *
 * \param[in] x Horizontal position.
 * \param[in] y Vertical position.
 */
void aika_input_process_mouse_move(int x, int y);

/**
 * Processes a mouse wheel event.
 *
 * \param[in] wheel Scroll direction.
 */
void aika_input_process_mouse_wheel(short wheel);

/**
 * Checks to see if the given key is up.
 *
 * \param[in] key Keyboard key.
 * \return Whether the key is up.
 */
bool aika_input_is_key_up(aika_keyboard_key key);

/**
 * Checks to see if the given key was up previously.
 *
 * \param[in] key Keyboard key.
 * \return Whether the key was up.
 */
bool aika_input_was_key_up(aika_keyboard_key key);

/**
 * Checks to see if the given key is down.
 *
 * \param[in] key Keyboard key.
 * \return Whether the key is down.
 */
bool aika_input_is_key_down(aika_keyboard_key key);

/**
 * Checks to see if the given key was down previously.
 *
 * \param[in] key Keyboard key.
 * \return Whether the key was down.
 */
bool aika_input_was_key_down(aika_keyboard_key key);

/**
 * Checks to see if the given button is up.
 *
 * \param[in] button Mouse button.
 * \return Whether the button is up.
 */
bool aika_input_is_button_up(aika_mouse_button button);

/**
 * Checks to see if the given button was up previously.
 *
 * \param[in] button Mouse button.
 * \return Whether the button was up.
 */
bool aika_input_was_button_up(aika_mouse_button button);

/**
 * Checks to see if the given button is down.
 *
 * \param[in] button Mouse button.
 * \return Whether the button is down.
 */
bool aika_input_is_button_down(aika_mouse_button button);

/**
 * Checks to see if the given button was down previously.
 *
 * \param[in] button Mouse button.
 * \return Whether the button was down.
 */
bool aika_input_was_button_down(aika_mouse_button button);

/**
 * Gets the current position of the mouse.
 *
 * \param[out] x Horizontal position.
 * \param[out] y Vertical position.
 */
void aika_input_get_mouse_position(int* x, int* y);

/**
 * Gets the previous position of the mouse.
 *
 * \param[out] x Horizontal position.
 * \param[out] y Vertical position.
 */
void aika_input_get_prev_mouse_position(int* x, int* y);

/**
 * Gets the current state of the mouse wheel.
 *
 * \return Mouse wheel direction.
 */
short aika_input_get_mouse_wheel();

/**
 * Gets the previous state of the mouse wheel.
 *
 * \return Mouse wheel direction.
 */
short aika_input_get_prev_mouse_wheel();

#endif
