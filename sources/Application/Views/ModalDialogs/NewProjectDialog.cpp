
#include "NewProjectDialog.h"
#include "Application/Utils/RandomNames.h"

static char *buttonText[BUTTONS_LENGTH] = {(char *)"Random", (char *)"Ok",
                                           (char *)"Cancel"};

#define DIALOG_WIDTH 20
#define SPACE_ROW 7

#define KEYBOARD_ROWS (SPACE_ROW + 1)

#define SPCE_START 0
#define SPCE_END 3
#define BACK_START 4
#define BACK_END 6
#define DONE_START 8
#define DONE_END 10

static const char *keyboardLayout[] = {"1234567890", "QWERTYUIOP", "ASDFGHJKL@",
                                       "ZXCVBNM,?>", "qwertyuiop", "asdfghjkl|",
                                       "zxcvbnm-_<", "[_] <-  OK"};

NewProjectDialog::NewProjectDialog(View &view):ModalView(view) {}

NewProjectDialog::~NewProjectDialog() {}

char NewProjectDialog::getKeyAtPosition(int row, int col) {
    if (row < 0 || row >= KEYBOARD_ROWS)
        return '\0';
    const char *rowStr = keyboardLayout[row];

    // Handle special row with SPC and <-
	if (row == SPACE_ROW) {
        if (col >= 0 && col < SPCE_END)
            return ' ';                                       // [_] (space)
        if (col >= BACK_START && col < BACK_END) return '\b'; // <- (backspace)
        if (col >= DONE_START && col < DONE_END) return '\r'; // END (carriage return)
		return '\0';
    }

    int len = strlen(rowStr);
    if (col < 0 || col >= len) return '\0';
	return rowStr[col];
}

void NewProjectDialog::findCharacterInKeyboard(char ch, int &outRow,
                                               int &outCol) {
    if (ch == ' ')
        return; // Skip blankspace character
                // Search for character in keyboard layout
    for (int row = 0; row < SPACE_ROW; row++) { // Exclude special row
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
}

void NewProjectDialog::DrawView() {

    SetWindow(DIALOG_WIDTH, keyboardMode_ ? 15 : 5);

    GUITextProperties props;

    SetColor(CD_NORMAL);

    // Draw string

    int x = (DIALOG_WIDTH - MAX_NAME_LENGTH) / 3;

    char buffer[2];
    buffer[1] = 0;
    for (int i = 0; i < MAX_NAME_LENGTH; i++) {
        props.invert_ = ((i == currentChar_) && (selected_ == 0));
        buffer[0]=name_[i] ;
        DrawString(x + i, 2, buffer, props);
    }

    // Draw keyboard if in keyboard mode
    if (keyboardMode_) {
        SetColor(CD_NORMAL);
        for (int row = 0; row < KEYBOARD_ROWS; row++) {
            const char* rowStr = keyboardLayout[row];
            int len = strlen(rowStr);
            int startX = (DIALOG_WIDTH - len) / 2;

            // Special handling for last row with SPC and <-
			if (row == SPACE_ROW) {
				// Draw SPACE
				props.invert_ = (row == keyboardRow_ && keyboardCol_ < SPCE_END);
				DrawString(startX, 4 + row, "[_]", props);

                // Draw <-
                props.invert_ = (row == keyboardRow_ && keyboardCol_ >= BACK_START && keyboardCol_ < BACK_END);
                DrawString(startX + 4, 4 + row, "<-", props);

                // Draw END
                props.invert_ =
                    (row == keyboardRow_ && keyboardCol_ >= DONE_START);
                DrawString(startX + 8, 4 + row, "OK", props);
            } else {
				for (int col = 0; col < len; col++) {
					props.invert_ = (row == keyboardRow_ && col == keyboardCol_);
                    buffer[0] = rowStr[col];
                    DrawString(startX + col, 4 + row, buffer, props);
                }
            }
        }
        props.invert_ = false;
        int xOffset = 0, yOffset = 13;
        DrawString(x + xOffset, yOffset, "A=input, B=erase", props);
        DrawString(x + xOffset, yOffset + 2, "L, R=move cursor", props);
		return; // Don't draw buttons in keyboard mode
    }

    // Draw buttons

    SetColor(CD_NORMAL);
    props.invert_=false ;

    int offset = DIALOG_WIDTH / (BUTTONS_LENGTH + 1);

    for (int i = 0; i < BUTTONS_LENGTH; i++) {
        const char *text = buttonText[i];
        x = (offset * (i + 1) - strlen(text) / BUTTONS_LENGTH) - 2;
        props.invert_=(selected_==i+1) ;
        DrawString(x, 4, text, props);
    }
};

void NewProjectDialog::OnPlayerUpdate(PlayerEventType ,unsigned int currentTick) {
};

void NewProjectDialog::OnFocus() {
	selected_=currentChar_=0;
    memset(name_, ' ', MAX_NAME_LENGTH + 1);
    lastChar_ = 'A';
    keyboardMode_ = false;
    keyboardRow_ = 2;
    keyboardCol_ = 0;
};

void NewProjectDialog::ProcessButtonMask(unsigned short mask,bool pressed) {

	if (!pressed) return ;

    // Handle keyboard mode navigation first
    if (keyboardMode_) {
        if (mask == EPBM_A) {
            // Insert character at current position
            char ch = getKeyAtPosition(keyboardRow_, keyboardCol_);
			if (ch == '\b') {
				// Backspace: delete character and move cursor left
				if (currentChar_ > 0) {
					currentChar_--;
					name_[currentChar_] = ' ';
                }
            } else if (ch == '\r') {
                // END key: exit keyboard mode (same as START)
                keyboardMode_ = false;
                isDirty_ = true;
                // EndModal(0);
                return;
            } else if (ch != '\0') {
                name_[currentChar_] = ch;
				lastChar_ = ch;
				if (currentChar_ < MAX_NAME_LENGTH - 1) {
                    currentChar_++;
                    findCharacterInKeyboard(name_[currentChar_], keyboardRow_,
                                            keyboardCol_);
                }
            }
            isDirty_ = true;
            return;
        } else if (mask == EPBM_B) {
            // Backspace: delete character and move cursor left
            if (currentChar_ > 0) {
                currentChar_--;
                name_[currentChar_] = ' ';
                isDirty_ = true;
            }
        } else if (mask == EPBM_L) {
            // Move cursor left
            if (currentChar_ > 0) {
                currentChar_--;
                findCharacterInKeyboard(name_[currentChar_], keyboardRow_, keyboardCol_);
                isDirty_ = true;
                return;
            }
        } else if (mask == EPBM_R) {
            // Move cursor right
            if (currentChar_ < MAX_NAME_LENGTH - 1) {
                currentChar_++;
                findCharacterInKeyboard(name_[currentChar_], keyboardRow_, keyboardCol_);
                isDirty_ = true;
                return;
            }
        }
        if (mask == EPBM_UP) {
            keyboardRow_ = (keyboardRow_ - 1 + KEYBOARD_ROWS) % KEYBOARD_ROWS;
            // Clamp column to valid range for new row
            if (keyboardRow_ == SPACE_ROW) {
				if (keyboardCol_ <  SPCE_END) keyboardCol_ = SPCE_START; // Move to [_]
				if (keyboardCol_ >= SPCE_END && keyboardCol_ <= BACK_END) keyboardCol_ = BACK_START; // Move to <-
				if (keyboardCol_ >  BACK_END) keyboardCol_ = DONE_START; // Move to DONE
            } else {
                int maxCol = strlen(keyboardLayout[keyboardRow_]) - 1;
                if (keyboardCol_ > maxCol) keyboardCol_ = 0;
            }
            isDirty_ = true;
            return;
        }
        if (mask == EPBM_DOWN) {
            keyboardRow_ = (keyboardRow_ + 1) % KEYBOARD_ROWS;
            // Clamp column to valid range for new row
            if (keyboardRow_ == SPACE_ROW) {
				// Special row: SPC is 0-6, <- is 8-11
                if (keyboardCol_ < SPCE_END)
                    keyboardCol_ = SPCE_START; // Move to [_]
                if (keyboardCol_ >= SPCE_END && keyboardCol_ <= BACK_END) keyboardCol_ = BACK_START; // Move to <-
				if (keyboardCol_ >  BACK_END) keyboardCol_ = DONE_START; // Move to DONE
            } else {
                int maxCol = strlen(keyboardLayout[keyboardRow_]) - 1;
                if (keyboardCol_ > maxCol)
                    keyboardCol_ = 0;
            }
            isDirty_ = true;
            return;
        }
        if (mask == EPBM_LEFT) {
            if (keyboardRow_ == SPACE_ROW) {
                // Cycle backward: END -> BACK -> SPACE -> END ...
                if (keyboardCol_ < SPCE_END) {
                    keyboardCol_ = DONE_START; // Move to DONE
                } else if (keyboardCol_ >= SPCE_END &&
                           keyboardCol_ < DONE_START) {
                    keyboardCol_ = SPCE_START; // Move to SPACE
                } else if (keyboardCol_ >= DONE_START) {
                    keyboardCol_ = BACK_START; // Move to BACK
                }
            } else {
                int maxCol = strlen(keyboardLayout[keyboardRow_]) - 1;
                keyboardCol_ = (keyboardCol_ - 1 + maxCol + 1) % (maxCol + 1);
            }
            isDirty_ = true;
            return;
        }
        if (mask == EPBM_RIGHT) {
            if (keyboardRow_ == SPACE_ROW) {
                // Cycle forward: SPACE -> BACK -> END -> SPACE ...
                if (keyboardCol_ >= BACK_START && keyboardCol_ < BACK_END) {
                    keyboardCol_ = DONE_START; // Move to DONE
                } else if (keyboardCol_ >= DONE_START) {
                    keyboardCol_ = SPCE_START; // Move to SPACE
                } else if (keyboardCol_ >= SPCE_START &&
                           keyboardCol_ < BACK_END) {
                    keyboardCol_ = BACK_START; // Move to BACK
                }
            } else {
                int maxCol = strlen(keyboardLayout[keyboardRow_]) - 1;
                keyboardCol_ = (keyboardCol_ + 1) % (maxCol + 1);
            }
            isDirty_ = true;
            return;
        } // Exit keyboard mode with Start
        if (mask == EPBM_START) {
            keyboardMode_ = false;
            isDirty_ = true;
            // EndModal(1); // We want to clear out the keyboard mode in the background too
			return;
		}
        return;
    } else if ((mask == EPBM_A)) {
        // Toggle keyboard mode with A
        if (selected_ == 0) {
            keyboardMode_ = !keyboardMode_;
            // When entering keyboard mode, jump to current character
            if (keyboardMode_) {
                findCharacterInKeyboard(name_[currentChar_], keyboardRow_,
                                        keyboardCol_);
            }
            isDirty_ = true;
        }
    }

    if (mask & EPBM_B) {

    } else {

        // A modifier
        if (mask & EPBM_A) {
            if (mask == EPBM_A) {
                std::string randomName = getRandomName();
                switch (selected_) {
                case 0:
                    if (name_[currentChar_] == ' ') {
                        name_[currentChar_] = lastChar_;
                    }
                    isDirty_ = true;
                    break;
                case 1:
                    std::fill(name_ + randomName.length(),
                              name_ + sizeof(name_) / sizeof(name_[0]), ' ');
                    strncpy(name_, randomName.c_str(), randomName.length());
                    lastChar_ = currentChar_ = randomName.length() - 1;
                    isDirty_ = true;
                    break;
                case 2:
                    EndModal(1);
                    break;
                case 3:
                    EndModal(0);
                    break;
                }
            }
        } else {

            // R Modifier

            if (mask & EPBM_R) {
            } else {
                // No modifier
                if (mask == EPBM_UP) {
                    selected_ = (selected_ == 0) ? 1 : 0;
                    isDirty_ = true;
                }
                if (mask == EPBM_DOWN) {
                    selected_ = (selected_ == 0) ? 1 : 0;
                    isDirty_ = true;
                }

                if (mask == EPBM_LEFT) {
                    switch (selected_) {
                    case 0:
                        if (currentChar_ > 0)
                            currentChar_--;
                        break;
                    case 1:
                    case 2:
                    case 3:
                        if (selected_ > 0)
                            selected_--;
                        break;
                    }
                    isDirty_ = true;
                }
                if (mask == EPBM_RIGHT) {
                    switch (selected_) {
                    case 0:
                        if (currentChar_ < MAX_NAME_LENGTH - 1)
                            currentChar_++;
                        else
                            selected_++;
                        break;
                    case 1:
                    case 2:
                    case 3:
                        if (selected_ < BUTTONS_LENGTH)
                            selected_++;
                        break;
                    }
                    isDirty_ = true;
                }
            }
        }
    }
};

std::string NewProjectDialog::GetName() {
    for (int i = MAX_NAME_LENGTH; i >= 0; i--) {
        if (name_[i]==' ') {
            name_[i] = 0;
        } else {
            break;
        }
    }
    std::string name = "lgpt_";
    name += name_;
	return name;
}
