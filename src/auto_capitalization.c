#include <stdio.h>
#include <windows.h>

// Those are the codes for the corresponding keys
#define PERIOD_KEY 0xBE
#define CTRL_KEY 0x11
#define ALT_KEY 0x12
#define CAPSLOCK_KEY 0x14
#define SPACE_KEY 0x20
#define S_KEY 0x53
#define KEY_PRESSED_CODE -32768

// The amount of time between each key press
#define TWO_HUNDRED_MILLISSECONDS 200
#define TWO_SECONDS 500

// This code defines that we want to scan hardware keys
#define SCAN_KEYS 0
#define KEY_PRESS_EVENT 0
#define KEY_RELEASE_EVENT KEYEVENTF_KEYUP

// This function will check if the desired key is pressed or not (will return 1 if does)
int has_pressed_key(int key_code);

// This function will press the desired key passed in the parameter
void press_key(int key_code);

// This function will check if caps is enabled or not (will return 1 if does)
int verify_caps_state();

int main()
{
  printf("Caps lock was activated.\n");
  press_key(CAPSLOCK_KEY);

  while (1)
  {
    if (has_pressed_key(CTRL_KEY) == 1 && has_pressed_key(ALT_KEY) == 1 && has_pressed_key(S_KEY) == 1)
    {
      printf("Stopping Auto Capitalization service.");
      break;
    }
    else if (has_pressed_key(PERIOD_KEY) == 1)
    {
      printf("Period key was pressed.\n");
      Sleep(TWO_HUNDRED_MILLISSECONDS);

      if (has_pressed_key(CAPSLOCK_KEY) == 0)
      {
        printf("Caps lock was activated.\n");
        press_key(SPACE_KEY);
        press_key(CAPSLOCK_KEY);
      }
    }
    else
    {
      for (int key = 64; key <= 90; key++)
      {
        if (has_pressed_key(key) == 1 && verify_caps_state() == 1)
        {
          printf("Caps lock was deactivated.\n");
          press_key(CAPSLOCK_KEY);
        }
      }
    }

    if (verify_caps_state() == 0)
    {
      Sleep(TWO_SECONDS);
      printf("Caps lock was activated.\n");
      press_key(CAPSLOCK_KEY);
    }
  }

  return 0;
}

int has_pressed_key(int key_code)
{
  if (GetAsyncKeyState(key_code) == KEY_PRESSED_CODE)
    return 1;

  return 0;
}

void press_key(int key_code)
{
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

int verify_caps_state()
{
  int caps_state = GetKeyState(CAPSLOCK_KEY);

  if (caps_state == 0)
    return 0;

  return 1;
}
