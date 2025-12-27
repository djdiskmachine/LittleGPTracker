#ifndef _KEYBOARD_LAYOUT_H_
#define _KEYBOARD_LAYOUT_H_

#include <cstring>

// Keyboard layout configuration
#define SPACE_ROW 7
#define KEYBOARD_ROWS (SPACE_ROW + 1)

#define SPCE_START 0
#define SPCE_END 3
#define BACK_START 4
#define BACK_END 6
#define DONE_START 8
#define DONE_END 10

static const char* keyboardLayout[] = {
	"1234567890",
	"QWERTYUIOP",
	"ASDFGHJKL@",
	"ZXCVBNM,?>",
	"qwertyuiop",
	"asdfghjkl|",
	"zxcvbnm-_<",
	"[_] <-  OK"
};

// Helper functions for special row section detection
inline bool isInSpaceSection(int col) { return col < SPCE_END; }
inline bool isInBackSection(int col) { return col >= BACK_START && col < BACK_END; }
inline bool isInDoneSection(int col) { return col >= DONE_START; }

// Get the character at a specific keyboard position
inline char getKeyAtPosition(int row, int col) {
	if (row < 0 || row >= KEYBOARD_ROWS) return '\0';
	const char* rowStr = keyboardLayout[row];
	
	// Handle special row with SPC, BACK, and DONE
	if (row == SPACE_ROW) {
		if (col >= 0 && col < SPCE_END) return ' ';        // [_] (space)
		if (col >= BACK_START && col < BACK_END) return '\b'; // <- (backspace)
		if (col >= DONE_START && col < DONE_END) return '\r'; // OK (carriage return)
		return '\0';
	}
	
	int len = strlen(rowStr);
	if (col < 0 || col >= len) return '\0';
	return rowStr[col];
}

// Find a character's position in the keyboard layout
inline void findCharacterInKeyboard(char ch, int &outRow, int &outCol) {
	if (ch == ' ') return; // Skip space character
	
	// Search for character in keyboard layout (excluding special row)
	for (int row = 0; row < SPACE_ROW; row++) {
		const char* rowStr = keyboardLayout[row];
		int len = strlen(rowStr);
		for (int col = 0; col < len; col++) {
			if (rowStr[col] == ch) {
				outRow = row;
				outCol = col;
				return;
			}
		}
	}
	// Character not found, don't change position
}

// Clamp keyboard column to valid range for current row
inline void clampKeyboardColumn(int row, int& col) {
	if (row == SPACE_ROW) {
		if (col < SPCE_END) col = SPCE_START;
		else if (col <= BACK_END) col = BACK_START;
		else col = DONE_START;
	} else {
		int maxCol = strlen(keyboardLayout[row]) - 1;
		if (col > maxCol) col = 0;
	}
}

// Cycle keyboard column left (-1) or right (+1) within current row
inline void cycleKeyboardColumn(int row, int direction, int& col) {
	if (row == SPACE_ROW) {
		if (direction < 0) { // LEFT
			if (isInSpaceSection(col)) col = DONE_START;
			else if (isInBackSection(col)) col = SPCE_START;
			else col = BACK_START;
		} else { // RIGHT
			if (isInBackSection(col)) col = DONE_START;
			else if (isInDoneSection(col)) col = SPCE_START;
			else col = BACK_START;
		}
	} else {
		int maxCol = strlen(keyboardLayout[row]) - 1;
		col = (col + direction + maxCol + 1) % (maxCol + 1);
	}
}

#endif
