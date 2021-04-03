#include <stdio.h>
#include <windows.h>

// Those are the codes for the corresponding keys
#define SHIFT_KEY 0x10
#define ENTER_KEY 0x0D
#define CTRL_KEY 0x11
#define ALT_KEY 0x12
#define CAPSLOCK_KEY 0x14
#define NUMBER_ONE_KEY 0x31
#define P_KEY 0x50
#define S_KEY 0x53
#define PERIOD_KEY 0xBE
#define KEY_PRESSED_CODE -32768

// This code defines that we want to scan hardware keys
#define SCAN_KEYS 0

// This defines when a button is pressed
#define KEY_PRESS_EVENT 0

// This defines when a button is not pressed anymore
#define KEY_RELEASE_EVENT KEYEVENTF_KEYUP

// This function will check if the desired key is pressed or not (will return 1 if does)
int has_pressed_key(int key_code);

// This function will check if the desired key combination is pressed or not (will return 1 if does)
int has_pressed_key_combination(int first_key_code, int second_key_code);

// This function will press the desired key passed in the parameter
void press_key(int key_code);

// This function will return the status of capslock (will return 1 if does)
int verify_caps_state();


int main() {
  while (1) {
    // If the user press CTRL + ALT + S, the service stops (S stands for stop)
    if (has_pressed_key(CTRL_KEY) && has_pressed_key(ALT_KEY) && has_pressed_key(S_KEY)) {
      printf("Stopping Auto Capitalization service.\n");
      break;
    }
    // If the user press CTRL + ALT + P, the service pauses (P stands for pause)
    // The same command will resume the service
    if (has_pressed_key(CTRL_KEY) && has_pressed_key(ALT_KEY) && has_pressed_key(P_KEY)) {
      printf("Pausing Auto Capitalization service.\n");
      Sleep(200);
      while (1) {
        if (has_pressed_key(CTRL_KEY) && has_pressed_key(ALT_KEY) && has_pressed_key(P_KEY)) {
          Sleep(200);
          printf("Resuming Auto Capitalization service.\n");
          break;
        }
      }

    }
    // If the user presses period or enter keys, we want to activate capslock
    // if is not enabled yet.
    else if (has_pressed_key(PERIOD_KEY) || has_pressed_key(ENTER_KEY)) {
      printf("A trigger key was pressed.\n");

      if (!verify_caps_state(CAPSLOCK_KEY)) {
        printf("Caps lock was activated.\n");
        press_key(CAPSLOCK_KEY);
      }
    }
    else {
      // We want to deactivate the caps when another key is pressed and the state
      // of capslock is active
      for (int key = 64; key <= 90; key++) {
        if (has_pressed_key(key) && verify_caps_state()) {
          // Sleep(ONE_HUNDRED_MILLISSECONDS);
          printf("Caps lock was deactivated.\n");
          press_key(CAPSLOCK_KEY);
        }
      }
    }
  }

  return 0;
}

int has_pressed_key(int key_code) {
  if (GetAsyncKeyState(key_code) == KEY_PRESSED_CODE)
    return 1;

  return 0;
}

void press_key(int key_code) {
  INPUT input;

  input.type = INPUT_KEYBOARD;
  input.ki.wScan = SCAN_KEYS;
  input.ki.time = 0;
  input.ki.dwExtraInfo = 0;

  input.ki.wVk = key_code;
  input.ki.dwFlags = KEY_PRESS_EVENT;
  SendInput(1, &input, sizeof(INPUT));

  input.ki.dwFlags = KEY_RELEASE_EVENT;
  SendInput(1, &input, sizeof(INPUT));
}

int verify_caps_state() {
  int caps_state = GetKeyState(CAPSLOCK_KEY);

  if (!caps_state)
    return 0;

  return 1;
}

int has_pressed_key_combination(int first_key_code, int second_key_code) {
  if (has_pressed_key(first_key_code)) {
    if (has_pressed_key(second_key_code))
      return 1;
  }

  return 0;
}