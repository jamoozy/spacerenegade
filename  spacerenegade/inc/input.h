#ifndef INPUT_H
#define INPUT_H

////////////////////////////////////////////////////////////////////////////////
// ------------------------- Key/Button name enums -------------------------- //
////////////////////////////////////////////////////////////////////////////////

// For all keys.
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

// For all buttons.
enum { SR_MOUSE_LEFT = 0, SR_MOUSE_CENTER, SR_MOUSE_RIGHT };



////////////////////////////////////////////////////////////////////////////////
// ------------------------- Input Device Classes --------------------------- //
////////////////////////////////////////////////////////////////////////////////

// This class knows all about which keys are pressed.
class Keyboard
{
	static Keyboard *keyboard;       // Only allowable instance of this class.

public:
	static Keyboard *getKeyboard();  // Get the only allowable instance.
	static void cleanUp();           // Delete the only instance.

// ---- Object part ------

private:
	bool *keys;   // The keys -- true means the key is pressed.
	Keyboard();   // Create the key array of size SR_NUM_KEYS.

public:
	~Keyboard();  // Delete keys.

	bool isDown(int key) const;  // Returns true if the key is pressed.
	void setDown(int key);       // Set key as pushed.
	void setUp(int key);         // Set key as not pushed.
};


// This class holds details about what the mouse is up to.
class Mouse
{
	static Mouse *mouse;       // The only allowable mouse object.

public:
	static Mouse *getMouse();  // Get the only allowable mouse object.
	static void cleanUp();     // Destroy the only mouse object.

// ---- Object part ------

private:
	int lastX, lastY,   // Last known position of the mouse cursor.
	    diffX, diffY;   // Difference between last position and the
	                    // position before.
	bool *buttons;      // The 3 mouse buttons.  True if pushed.

	Mouse();  // Makes everything 0 and buttons new bool[3].

public:
	~Mouse(); // Delete butttons.

	void setLastMousePos(int x, int y);  // Updates last* and diff*
	void isDown(int button) const;       // Returns true if the button is pressed.
	void setDown(int button);            // Set button as pushed.
	void setUp(int button);              // Set button as not pushed.
	double getDiffX() const;
	double getDiffY() const;
	void clearDiffs();
};



////////////////////////////////////////////////////////////////////////////////
// ------------------------- Global GLUT callbacks -------------------------- //
////////////////////////////////////////////////////////////////////////////////

// Detect key input and act upon the keyboard object accordingly ...
//    i.e. set the corresponding key as pressed or not pressed.
void readKeyboard(unsigned char key, int x, int y);
void readKeyboardUp(unsigned char key, int x, int y);
void readSpecialKeys(int key, int x, int y);
void readSpecialKeysUp(int key, int x, int y);

// Update the mouse object.
void mouseButtHandler(int button, int state, int x, int y);
void mouseMoveHandler(int x, int y);

void handleTacticalInput();

#endif

