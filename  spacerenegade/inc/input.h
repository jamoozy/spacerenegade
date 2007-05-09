#ifndef INPUT_H
#define INPUT_H

enum { SR_KEY_A = 0, SR_KEY_B, SR_KEY_C, SR_KEY_D, SR_KEY_E, SR_KEY_F, SR_KEY_G,
	   SR_KEY_H, SR_KEY_I, SR_KEY_J, SR_KEY_K, SR_KEY_L, SR_KEY_M, SR_KEY_N,
	   SR_KEY_O, SR_KEY_P, SR_KEY_Q, SR_KEY_R, SR_KEY_S, SR_KEY_T, SR_KEY_U,
	   SR_KEY_V, SR_KEY_W, SR_KEY_X, SR_KEY_Y, SR_KEY_Z,

       SR_KEY_SPACE, 

       SR_KEY_F1, SR_KEY_F2, SR_KEY_F3, SR_KEY_F4, SR_KEY_F5, SR_KEY_F6,
       SR_KEY_F7, SR_KEY_F8, SR_KEY_F9, SR_KEY_F10, SR_KEY_F11, SR_KEY_F12,

       SR_KEY_1, SR_KEY_2, SR_KEY_3, SR_KEY_4, SR_KEY_5, SR_KEY_6,
       SR_KEY_7, SR_KEY_8, SR_KEY_9, SR_KEY_10, SR_KEY_11, SR_KEY_12,

       SR_KEY_LEFT, SR_KEY_UP, SR_KEY_RIGHT, SR_KEY_DOWN,
	   
       SR_KEY_PAGE_UP, SR_KEY_PAGE_DOWN, SR_KEY_HOME, SR_KEY_END, SR_KEY_INSERT,

       SR_NUM_KEYS };

class Keyboard
{
	static Keyboard *keyboard;

public:
	static Keyboard *getKeyboard();
	static void cleanUp();



// ---- Object part ------

private:
	bool *keys;
	Keyboard();

public:


	~Keyboard();

	bool isDown(int key);
	void setDown(int key);
	void setUp(int key);
};

void readKeyboard(unsigned char key, int x, int y);
void readKeyboardUp(unsigned char key, int x, int y);

void readSpecialKeys(int key, int x, int y);
void readSpecialKeysUp(int key, int x, int y);

void mouseButtHandler(int button, int state, int x, int y);
void mouseMoveHandler(int x, int y);

#endif
