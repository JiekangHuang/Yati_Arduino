#include <Esplora.h>
#include <Keyboard.h>

bool buttonStates[8];

const byte buttons[] = {
  JOYSTICK_DOWN,
  JOYSTICK_LEFT,
  JOYSTICK_UP,
  JOYSTICK_RIGHT,
  SWITCH_RIGHT,
  SWITCH_LEFT,
  SWITCH_UP,
  SWITCH_DOWN,
};

const char keystrokes[] = {
  0x73, //s
  0x61, //a
  0x77, //w
  0x64, //d
  KEY_LEFT_CTRL,
  0x6D, //m
  KEY_LEFT_SHIFT,
  0x20 //space
};

void setup() {
	Keyboard.begin();
}

void loop() {
	for (byte thisButton = 0; thisButton < 8; thisButton++) {
		bool lastState = buttonStates[thisButton];
		bool newState = Esplora.readButton(buttons[thisButton]);
		if (lastState != newState) {
			if (newState == PRESSED) {
				Keyboard.press(keystrokes[thisButton]);
			}
			else if (newState == RELEASED) {
				Keyboard.release(keystrokes[thisButton]);
			}
		}

		buttonStates[thisButton] = newState;
	}

	delay(50);
}