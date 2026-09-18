#define COLNUM 16
#define ROWNUM 8
#define KEY_NOP 0
#define LOCKOUT 15

//keep track of time since event (press or release)
//register initial edge event, then lockout for next LOCKOUT milliseconds
typedef struct KeyState {
    int state = HIGH;
    int eventTime = -1;
} KeyState;

int columnPins[COLNUM] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
int rowPins[ROWNUM] = {16, 17, 18, 19, 20, 21, 22, 23};

void (*setKey[])(uint8_t) = {
    [] (uint8_t key) -> void {Keyboard.set_key1(key);},
    [] (uint8_t key) -> void {Keyboard.set_key2(key);},
    [] (uint8_t key) -> void {Keyboard.set_key3(key);},
    [] (uint8_t key) -> void {Keyboard.set_key4(key);},
    [] (uint8_t key) -> void {Keyboard.set_key5(key);},
    [] (uint8_t key) -> void {Keyboard.set_key6 (key);}};

//maps the standard keyboard layout to the layout of the keyboard's internal key matrix for easier interpretation of the code
#define KEYMAP( \
    K5A,      K5B, K5C, K5D, K5E,   K5F, K5G, K5H, K5I,   K5J, K5K, K5L, K5M,   K5N, K5O, K5P, \
    \
    K4A, K4B, K4C, K4D, K4E, K4F, K4G, K4H, K4I, K4J, K4K, K4L, K4M, K4N,   K4O, K4P, K4Q,   K4R, K4S, K4T, K4U, \
    K3A, K3B, K3C, K3D, K3E, K3F, K3G, K3H, K3I, K3J, K3K, K3L, K3M, K3N,   K3O, K3P, K3Q,   K3R, K3S, K3T, K3U, \
    K2A, K2B, K2C, K2D, K2E, K2F, K2G, K2H, K2I, K2J, K2K, K2L, K2M,                         K2N, K2O, K2P, \
    K1A, K1B, K1C, K1D, K1E, K1F, K1G, K1H, K1I, K1J, K1K, K1L,                  K1M,        K1N, K1O, K1P, K1Q, \
    K0A, K0B, K0C, K0D, K0E,                                                K0F, K0G, K0H,   K0I,      K0J \
) { \
/* 00 */ { KEY_NOP,    KEY_NOP,    KEY##K5A, KEY_NOP,    KEY##K5E, KEY##K2F, KEY##K5F, KEY##K2G, KEY##K5G, KEY_NOP,    KEY##K2L,  KEY_NOP,    KEY##K0I, KEY##K0J, KEY##K1M, KEY##K0B }, \
/* 01 */ { KEY_NOP,    KEY##K1A, KEY##K3A, KEY##K2A, KEY##K5D, KEY##K3F, KEY##K4N, KEY##K3G, KEY##K3M, KEY##K5H, KEY##K3L,  KEY##K2N, KEY##K2O, KEY##K2P, KEY_NOP,    KEY_NOP }, \
/* 02 */ { KEY##K0A, KEY_NOP,    KEY##K4A, KEY##K5B, KEY##K5C, KEY##K4F, KEY##K5J, KEY##K4G, KEY##K4M, KEY##K5I, KEY##K4L,  KEY##K3O, KEY##K4O, KEY##K4Q, KEY##K4P, KEY_NOP }, \
/* 03 */ { KEY_NOP,    KEY_NOP,    KEY##K4B, KEY##K4C, KEY##K4D, KEY##K4E, KEY##K5K, KEY##K4H, KEY##K4I, KEY##K4J, KEY##K4K,  KEY##K5L, KEY##K5M, KEY##K3Q, KEY##K3P, KEY##K5N }, \
/* 04 */ { KEY_NOP,    KEY_NOP,    KEY##K3B, KEY##K3C, KEY##K3D, KEY##K3E, KEY_NOP,    KEY##K3H, KEY##K3I, KEY##K3J, KEY##K3K,  KEY##K3R, KEY##K3S, KEY##K3T, KEY##K3U, KEY##K5O }, \
/* 05 */ { KEY_NOP,    KEY_NOP,    KEY##K2B, KEY##K2C, KEY##K2D, KEY##K2E, KEY##K3N, KEY##K2H, KEY##K2I, KEY##K2J, KEY##K2K,  KEY##K1N, KEY##K1O, KEY##K1P, KEY##K1Q, KEY_NOP }, \
/* 06 */ { KEY##K0E, KEY##K1L, KEY##K1B, KEY##K1C, KEY##K1D, KEY##K1E, KEY##K2M, KEY##K1H, KEY##K1I, KEY##K1J, KEY_NOP,     KEY##K4R, KEY##K4S, KEY##K4T, KEY##K5P, KEY_NOP }, \
/* 07 */ { KEY_NOP,    KEY_NOP,    KEY_NOP,    KEY_NOP,    KEY_NOP,    KEY##K1F, KEY##K0C, KEY##K1G, KEY_NOP,    KEY_NOP,    KEY##K1K,  KEY##K0G, KEY##K0H, KEY##K4U, KEY##K0F, KEY##K0D } \
}
/*         0         1         2         3         4         5         6         7         8         9         10          11         12         13         14         15 */

//maps the row and column in the keyboard's key matrix to the actual keycode
static const uint32_t PROGMEM keymaps[ROWNUM][COLNUM] = KEYMAP(
    _ESC, _F1, _F2, _F3, _F4,  _F5, _F6, _F7, _F8,  _F9, _F10, _F11, _F12,                                          _PRINTSCREEN, _SCROLL_LOCK, _PAUSE,
    _TILDE,  _1,   _2,   _3,   _4,   _5,   _6,   _7,   _8,   _9,   _0,   _MINUS,         _EQUAL,    _BACKSPACE,     _INSERT,      _HOME,       _PAGE_UP,        _NUM_LOCK,PAD_SLASH,PAD_ASTERIX,PAD_MINUS,
    _TAB,     _Q,   _W,   _E,   _R,   _T,   _Y,   _U,   _I,   _O,   _P,   _LEFT_BRACE, _RIGHT_BRACE, _BACKSLASH,    _DELETE,      _END,        _PAGE_DOWN,         PAD_7,       PAD_8,    PAD_9, PAD_PLUS,
    _CAPS_LOCK,_A,   _S,   _D,   _F,   _G,   _H,   _J,   _K,   _L,       _SEMICOLON,     _QUOTE,         _ENTER,                                                   PAD_4,       PAD_5,    PAD_6,
    _LEFT_SHIFT,_Z,   _X,   _C,   _V,   _B,   _N,   _M,   _COMMA,       _PERIOD,        _SLASH,     _RIGHT_SHIFT,                   _UP,                           PAD_1,       PAD_2,    PAD_3, PAD_ENTER,
    _LEFT_CTRL,_LEFT_ALT,            _SPACE,           _RIGHT_ALT,                                   _RIGHT_CTRL,             _LEFT,_DOWN,_RIGHT,                   PAD_0,               PAD_PERIOD
);

//keystate matrix is used to keep track of and detect the state of all keys
KeyState keyStateMatrix[ROWNUM][COLNUM];

void setup() {

  //build and initialise keystate matrix
    for (int row = 0; row < ROWNUM; row++) {
        for (int col = 0; col < COLNUM; col++) {
            KeyState key;
            keyStateMatrix[row][col] = key;
        }
    }

    //columns are used for output
    for (int col = 0; col < COLNUM; col++) {
        pinMode(columnPins[col], OUTPUT_OPENDRAIN);
        digitalWrite(columnPins[col], HIGH);
    }

    //rows are read for input
    for (int row = 0; row < ROWNUM; row++) {
        pinMode(rowPins[row], INPUT);
    }

    //disable unused ports
    for (int pin = 24; pin <= 39; pin++) {
        pinMode(pin, INPUT_DISABLE);
    }
}

/*
returns true if the key at that row and column is a modifier key,
otherwise returns false
*/
bool isModifier(int row, int col) {
    uint32_t key = keymaps[row][col];

    if (key == KEY_LEFT_CTRL || key == KEY_RIGHT_CTRL || key == KEY_LEFT_ALT ||
        key == KEY_RIGHT_ALT || key == KEY_LEFT_SHIFT || key == KEY_RIGHT_SHIFT) {
        return true;
    }
    return false;
}

void loop() {

    
    int usbKeys[] = {0,0,0,0,0,0};
    int usbMod = 0;

    while (true) {
    
        for (int col = 0; col <= 15; col++) {
            digitalWrite(columnPins[col], LOW);
            delay(1);//give it an instant to pull the pin low
            for (int row = 0; row <= 7; row++) {

                //if a key event hasn't happened in the last LOCKOUT milliseconds on this key
                if (keyStateMatrix[row][col].eventTime == -1 || (millis() - keyStateMatrix[row][col].eventTime > LOCKOUT)) {

                    uint8_t newState = digitalRead(rowPins[row]);
                    int keyCode = keymaps[row][col];

                    //rising edge event, key released
                    if (newState == HIGH && keyStateMatrix[row][col].state == LOW) {
                        keyStateMatrix[row][col].state = HIGH;
                        keyStateMatrix[row][col].eventTime = millis();
                        
                        //remove key from usbkeys array or modifier int
                        if (isModifier(row, col)) {
                            usbMod = usbMod & (~keyCode);
                        } else {
                            for (int index = 0; index < 6; index++) {
                                if (usbKeys[index] == keyCode) {
                                    usbKeys[index] = 0;
                                }
                            }
                        }
                    //falling edge, key pressed
                    } else if (newState == LOW && keyStateMatrix[row][col].state == HIGH) {
                        bool rowMatch = false;
                        bool colMatch = false;      

                        //check the row for keys pressed. If there is a key pressed in same row, different column,
                        //then check the different column as well.
                        for (int tempCol = 0; tempCol <= 15; tempCol++) {
                            if (tempCol != col && keyStateMatrix[row][tempCol].state == LOW) {
                                for (int tempRow = 0; tempRow <= 7; tempRow++) {
                                    if (tempRow != row && keyStateMatrix[tempRow][tempCol].state == LOW) {
                                        //GHOSTING DETECTED
                                        goto block;
                                    }
                                }
                                rowMatch = true;
                            }
                        }

                        //check the column for keys pressed. If there is a key pressed in the same column, different row,
                        //then check the row as well
                        for (int tempRow = 0; tempRow <= 7; tempRow++) {
                            if (tempRow != row && keyStateMatrix[tempRow][col].state == LOW) {
                                for (int tempCol = 0; tempCol <= 15; tempCol++) {
                                    if (tempCol != col && keyStateMatrix[tempRow][tempCol].state == LOW) {
                                        //GHOSTING DETECTED
                                        goto block;
                                    }
                                }
                                colMatch = true;
                            }
                        }

                        //if there was a key pressed in the same column and a key pressed in the same row as the key,
                        //there will be ghosting, ignore input
                        if (rowMatch && colMatch) {
                            goto block;
                        }

                        //try to add to usb key array or modifier int
                        if (isModifier(row, col)) {
                            usbMod = usbMod | keyCode;
                        } else {
                            for (int index = 0; index < 6; index++) {
                                if (usbKeys[index] == 0) {
                                    usbKeys[index] = keyCode;
                                    break;
                                } else if (index == 5) {
                                    //6 keys already registered
                                    goto block;
                                }
                            }
                        }

                        keyStateMatrix[row][col].state = LOW;
                        keyStateMatrix[row][col].eventTime = millis();            

                        block:;

                    }
                }
            }
            digitalWrite(columnPins[col], HIGH);
            //give it a ms to bring the pin to a high impedance state
            delay(1);
        }

        //sync teensyduino key press state with our own
        for (int key = 0; key < 6; key++) {
            setKey[key](usbKeys[key]);
        }
        Keyboard.set_modifier(usbMod);
        Keyboard.send_now();
    }
}

