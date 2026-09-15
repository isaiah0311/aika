/**
 * \file input.c
 * \author Isaiah Lateer
 *
 * Implementation of the input interface.
 */

#include "input/input.h"

#include <string.h>

typedef struct aika_keyboard {
    bool keys[AIKA_KEYBOARD_KEY_COUNT];
} aika_keyboard;

typedef struct aika_mouse {
    bool buttons[AIKA_MOUSE_BUTTON_COUNT];
    int x, y;
    short wheel;
} aika_mouse;

typedef struct aika_input {
    aika_keyboard keyboard, prev_keyboard;
    aika_mouse mouse, prev_mouse;
} aika_input;

static aika_input input = { 0 };

/**
 * Processes a key event.
 *
 * \param[in] key Keyboard key.
 * \param[in] pressed Whether the key was pressed.
 */
void aika_input_process_key(aika_keyboard_key key, bool pressed) {
    input.keyboard.keys[key] = pressed;
}

/**
 * Processes a button event.
 *
 * \param[in] button Mouse button.
 * \param[in] pressed Whether the button was pressed.
 */
void aika_input_process_button(aika_mouse_button button, bool pressed) {
    input.mouse.buttons[button] = pressed;
}

/**
 * Processes a mouse motion event.
 *
 * \param[in] x Horizontal position.
 * \param[in] y Vertical position.
 */
void aika_input_process_mouse_move(int x, int y) {
    input.mouse.x = x;
    input.mouse.y = y;
}

/**
 * Processes a mouse wheel event.
 *
 * \param[in] wheel Scroll direction.
 */
void aika_input_process_mouse_wheel(short wheel) {
    input.mouse.wheel = wheel;
}

/**
 * Checks to see if the given key is up.
 *
 * \param[in] key Keyboard key.
 * \return Whether the key is up.
 */
bool aika_input_is_key_up(aika_keyboard_key key) {
    return !input.keyboard.keys[key];
}

/**
 * Checks to see if the given key was up previously.
 *
 * \param[in] key Keyboard key.
 * \return Whether the key was up.
 */
bool aika_input_was_key_up(aika_keyboard_key key) {
    return !input.prev_keyboard.keys[key];
}

/**
 * Checks to see if the given key is down.
 *
 * \param[in] key Keyboard key.
 * \return Whether the key is down.
 */
bool aika_input_is_key_down(aika_keyboard_key key) {
    return input.keyboard.keys[key];
}

/**
 * Checks to see if the given key was down previously.
 *
 * \param[in] key Keyboard key.
 * \return Whether the key was down.
 */
bool aika_input_was_key_down(aika_keyboard_key key) {
    return input.prev_keyboard.keys[key];
}

/**
 * Checks to see if the given button is up.
 *
 * \param[in] button Mouse button.
 * \return Whether the button is up.
 */
bool aika_input_is_button_up(aika_mouse_button button) {
    return !input.mouse.buttons[button];
}

/**
 * Checks to see if the given button was up previously.
 *
 * \param[in] button Mouse button.
 * \return Whether the button was up.
 */
bool aika_input_was_button_up(aika_mouse_button button) {
    return !input.prev_mouse.buttons[button];
}

/**
 * Checks to see if the given button is down.
 *
 * \param[in] button Mouse button.
 * \return Whether the button is down.
 */
bool aika_input_is_button_down(aika_mouse_button button) {
    return input.mouse.buttons[button];
}

/**
 * Checks to see if the given button was down previously.
 *
 * \param[in] button Mouse button.
 * \return Whether the button was down.
 */
bool aika_input_was_button_down(aika_mouse_button button) {
    return input.prev_mouse.buttons[button];
}

/**
 * Gets the current position of the mouse.
 *
 * \param[out] x Horizontal position.
 * \param[out] y Vertical position.
 */
void aika_input_get_mouse_position(int* x, int* y) {
    *x = input.mouse.x;
    *y = input.mouse.y;
}

/**
 * Gets the previous position of the mouse.
 *
 * \param[out] x Horizontal position.
 * \param[out] y Vertical position.
 */
void aika_input_get_prev_mouse_position(int* x, int* y) {
    *x = input.prev_mouse.x;
    *y = input.prev_mouse.y;
}

/**
 * Gets the current state of the mouse wheel.
 *
 * \return Mouse wheel direction.
 */
short aika_input_get_mouse_wheel() {
    return input.mouse.wheel;
}

/**
 * Gets the previous state of the mouse wheel.
 *
 * \return Mouse wheel direction.
 */
short aika_input_get_prev_mouse_wheel() {
    return input.prev_mouse.wheel;
}

/** Clears the current and previous input states. */
void aika_input_clear() {
    memset(&input, 0, sizeof(input));
}

/** Copies the current input state into the previous input state. */
void aika_input_save_state() {
    memcpy(&input.prev_keyboard, &input.keyboard, sizeof(input.prev_keyboard));
    memcpy(&input.prev_mouse, &input.mouse, sizeof(input.prev_mouse));
}
