
#include "NewProjectDialog.h"
#include "Application/Utils/RandomNames.h"

static char *buttonText[BUTTONS_LENGTH] = {
	(char *)"Regen",
	(char *)"Ok",
	(char *)"Cancel"
} ;

#define DIALOG_WIDTH 20

// QWERTY keyboard layout (5 rows)
static const char* keyboardLayout[] = {
	"1234567890",
	"QWERTYUIOP",
	"ASDFGHJKL",
	"qwertyuiop",
	"asdfghjkl",
	"zxcvbnm.-_",
	"[____] <-"
};

static const int keyboardRows = 7;

NewProjectDialog::NewProjectDialog(View &view):ModalView(view) {}

NewProjectDialog::~NewProjectDialog() {
}

char NewProjectDialog::getKeyAtPosition(int row, int col) {
    if (row < 0 || row >= keyboardRows) return '\0';
	const char* rowStr = keyboardLayout[row];
	
	// Handle special row with [____] and <-
	if (row == 6) {
		if (col >= 0 && col < 7) return ' '; // [____] (space)
		if (col >= 8 && col < 12) return '\b'; // <- (backspace)
		return '\0';
	}
	
	int len = strlen(rowStr);
	if (col < 0 || col >= len) return '\0';
	return rowStr[col];
}

void NewProjectDialog::findCharacterInKeyboard(char ch, int &outRow, int &outCol) {
	// Search for character in keyboard layout
	for (int row = 0; row < keyboardRows - 1; row++) { // Exclude special row
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
	// Handle space specially
	if (ch == ' ') {
		outRow = 6;
		outCol = 0;
		return;
	}
	// Character not found, don't change position
}

void NewProjectDialog::DrawView() {

    SetWindow(DIALOG_WIDTH, keyboardMode_ ? 12 : 5);

    GUITextProperties props;

    SetColor(CD_NORMAL);

    // Draw string

    int x = (DIALOG_WIDTH - MAX_NAME_LENGTH) / 3;

    char buffer[2];
    buffer[1]=0 ;
    for (int i = 0; i < MAX_NAME_LENGTH; i++) {
        props.invert_ =
            ((i == currentChar_) && (selected_ == 0) && !keyboardMode_);
        buffer[0]=name_[i] ;
        DrawString(x + i, 2, buffer, props);
    }

    // Draw keyboard if in keyboard mode
    if (keyboardMode_) {
		SetColor(CD_NORMAL);
		for (int row = 0; row < keyboardRows; row++) {
			const char* rowStr = keyboardLayout[row];
			int len = strlen(rowStr);
			int startX = (DIALOG_WIDTH - len) / 2;
			
			// Special handling for last row with [____] and <-
			if (row == 6) {
				// Draw [____]
				props.invert_ = (row == keyboardRow_ && keyboardCol_ < 7);
				DrawString(startX, 4 + row, "[____]", props);
				
				// Draw <-
				props.invert_ = (row == keyboardRow_ && keyboardCol_ >= 8);
				DrawString(startX + 8, 4 + row, "<-", props);
			} else {
				for (int col = 0; col < len; col++) {
					props.invert_ = (row == keyboardRow_ && col == keyboardCol_);
					buffer[0] = rowStr[col];
					DrawString(startX + col, 4 + row, buffer, props);
				}
			}
		}
        // Draw cursor indicator when in keyboard mode
        DrawString(x + currentChar_, 3, "-", props);
        props.invert_ = false;
		return; // Don't draw buttons in keyboard mode
    }

    // Draw buttons

	SetColor(CD_NORMAL) ;
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
	keyboardRow_ = 1; // Start on QWERTY row
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
			} else if (ch != '\0') {
				name_[currentChar_] = ch;
				lastChar_ = ch;
				if (currentChar_ < MAX_NAME_LENGTH - 1) {
					currentChar_++;
					// Jump keyboard cursor to the character at new position
					char nextChar = name_[currentChar_];
					if (nextChar != ' ') {
						findCharacterInKeyboard(nextChar, keyboardRow_, keyboardCol_);
                    }
                }
            }
            isDirty_ = true;
			return;
        }
        if (mask == EPBM_UP) {
			keyboardRow_ = (keyboardRow_ - 1 + keyboardRows) % keyboardRows;
			// Clamp column to valid range for new row
			if (keyboardRow_ == 6) {
				// Special row: [____] is 0-6, <- is 8-11
				if (keyboardCol_ >= 7) keyboardCol_ = 8; // Move to <-
			} else {
				int maxCol = strlen(keyboardLayout[keyboardRow_]) - 1;
				if (keyboardCol_ > maxCol) keyboardCol_ = maxCol;
			}
			isDirty_ = true;
			return;
        }
        if (mask == EPBM_DOWN) {
			keyboardRow_ = (keyboardRow_ + 1) % keyboardRows;
			// Clamp column to valid range for new row
			if (keyboardRow_ == 6) {
				// Special row: [____] is 0-6, <- is 8-11
				if (keyboardCol_ >= 7) keyboardCol_ = 8; // Move to <-
			} else {
				int maxCol = strlen(keyboardLayout[keyboardRow_]) - 1;
				if (keyboardCol_ > maxCol) keyboardCol_ = maxCol;
			}
			isDirty_ = true;
			return;
        }
        if (mask == EPBM_LEFT) {
			if (keyboardRow_ == 6) {
				// Special row: toggle between [____] and <-
				keyboardCol_ = (keyboardCol_ < 8) ? 8 : 0;
			} else {
				int maxCol = strlen(keyboardLayout[keyboardRow_]) - 1;
				keyboardCol_ = (keyboardCol_ - 1 + maxCol + 1) % (maxCol + 1);
			}
			isDirty_ = true;
			return;
		}
		if (mask == EPBM_RIGHT) {
			if (keyboardRow_ == 6) {
				// Special row: toggle between [____] and <-
				keyboardCol_ = (keyboardCol_ < 8) ? 8 : 0;
			} else {
				int maxCol = strlen(keyboardLayout[keyboardRow_]) - 1;
				keyboardCol_ = (keyboardCol_ + 1) % (maxCol + 1);
			}
			isDirty_ = true;
			return;
		}
		// Exit keyboard mode with B
		if (mask == EPBM_B) {
			keyboardMode_ = false;
			isDirty_ = true;
			return;
		}
		return;
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
            if ((mask & EPBM_UP) || (mask & EPBM_DOWN)) {
                // Toggle keyboard mode with A+UP or A+DOWN
                if (selected_ == 0) {
                    keyboardMode_ = !keyboardMode_;
                    // When entering keyboard mode, jump to current character
                    if (keyboardMode_) {
                        char currentCh = name_[currentChar_];
                        if (currentCh != ' ') {
                            findCharacterInKeyboard(currentCh, keyboardRow_,
                                                    keyboardCol_);
                        }
                    }
                    isDirty_ = true;
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
