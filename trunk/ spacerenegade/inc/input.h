#ifndef INPUT_H
#define INPUT_H

enum { SR_KEY_A = 0, SR_KEY_B, SR_KEY_C, SR_KEY_D, SR_KEY_E, SR_KEY_F, SR_KEY_G,
	   SR_KEY_H, SR_KEY_I, SR_KEY_J, SR_KEY_K, SR_KEY_L, SR_KEY_M, SR_KEY_N,
	   SR_KEY_O, SR_KEY_P, SR_KEY_Q, SR_KEY_R, SR_KEY_S, SR_KEY_T, SR_KEY_U,
	   SR_KEY_V, SR_KEY_W, SR_KEY_X, SR_KEY_Y, SR_KEY_Z, SR_KEY_SPACE, SR_NUM_KEYS };

class Keyboard
{
	bool *keys;
	static Keyboard *keyboard;
	Keyboard();

public:

	static Keyboard *getKeyboard();
	static void cleanUp();

	~Keyboard();

	bool isDown(int key);
	void setDown(int key);
	void setUp(int key);
};

void readKeyboard(unsigned char key, int x, int y);

void readKeyboardUp(unsigned char key, int x, int y);

void readSpecialKeys(int key, int x, int y);

void mouseButtHandler(int button, int state, int x, int y);

void mouseMoveHandler(int x, int y);

#endif
