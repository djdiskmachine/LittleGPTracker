
#include "NewProjectDialog.h"
#include "Application/Utils/KeyboardLayout.h"
#include "Application/Utils/RandomNames.h"

static char *buttonText[BUTTONS_LENGTH] = {(char *)"Random", (char *)"Ok",
                                           (char *)"Cancel"};

#define DIALOG_WIDTH 20

NewProjectDialog::NewProjectDialog(View &view, Path currentPath)
    : ModalView(view), currentPath_(currentPath) {}

NewProjectDialog::~NewProjectDialog() {}

// Move text cursor left (-1) or right (+1) and update keyboard position
void NewProjectDialog::moveCursor(int direction) {
    int newPos = currentChar_ + direction;
    if (newPos >= 0 && newPos < MAX_NAME_LENGTH) {
        currentChar_ = newPos;
        findCharacterInKeyboard(name_[currentChar_], keyboardRow_,
                                keyboardCol_);
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
                props.invert_ =
                    (row == keyboardRow_ && isInSpaceSection(keyboardCol_));
                DrawString(startX, 4 + row, "[_]", props);

                // Draw <-
                props.invert_ =
                    (row == keyboardRow_ && isInBackSection(keyboardCol_));
                DrawString(startX + 4, 4 + row, "<-", props);

                // Draw END
                props.invert_ =
                    (row == keyboardRow_ && isInDoneSection(keyboardCol_));
                DrawString(startX + 8, 4 + row, "OK", props);
            } else {
                for (int col = 0; col < len; col++) {
                    props.invert_ =
                        (row == keyboardRow_ && col == keyboardCol_);
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
    View::EnableNotification();
}

void NewProjectDialog::OnPlayerUpdate(PlayerEventType,
                                      unsigned int currentTick) {};

void NewProjectDialog::OnFocus() {
	selected_=currentChar_=0;
    memset(name_, ' ', MAX_NAME_LENGTH + 1);
    lastChar_ = 'A';
    keyboardMode_ = false;
    keyboardRow_ = 2;
    keyboardCol_ = 0;
};

void NewProjectDialog::ProcessButtonMask(unsigned short mask, bool pressed) {

    if (!pressed)
        return;

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
            return;
        } else if (mask == EPBM_L) {
            // Move cursor left
            moveCursor(-1);
            isDirty_ = true;
            return;
        } else if (mask == EPBM_R) {
            // Move cursor right
            moveCursor(1);
            isDirty_ = true;
            return;
        } else if (mask == EPBM_UP) {
            keyboardRow_ = (keyboardRow_ - 1 + KEYBOARD_ROWS) % KEYBOARD_ROWS;
            clampKeyboardColumn(keyboardRow_, keyboardCol_);
            isDirty_ = true;
            return;
        } else if (mask == EPBM_DOWN) {
            keyboardRow_ = (keyboardRow_ + 1) % KEYBOARD_ROWS;
            clampKeyboardColumn(keyboardRow_, keyboardCol_);
            isDirty_ = true;
            return;
        } else if (mask == EPBM_LEFT) {
            cycleKeyboardColumn(keyboardRow_, -1, keyboardCol_);
            isDirty_ = true;
            return;
        } else if (mask == EPBM_RIGHT) {
            cycleKeyboardColumn(keyboardRow_, 1, keyboardCol_);
            isDirty_ = true;
            return;
        } else if (mask == EPBM_START) {
            keyboardMode_ = false;
            isDirty_ = true;
            return;
        }
        return;
    } else if (mask & EPBM_A) {
        if (mask == EPBM_A) {
            std::string randomName = "";
            switch (selected_) {
            case 0:
                // Toggle keyboard mode
                keyboardMode_ = !keyboardMode_;
                // When entering keyboard mode, jump to current character
                if (keyboardMode_) {
                    findCharacterInKeyboard(name_[currentChar_], keyboardRow_,
                                            keyboardCol_);
                }
                isDirty_ = true;
                break;
            case 1:
                do {
                    randomName = getRandomName();
                    std::fill(name_ + randomName.length(),
                              name_ + sizeof(name_) / sizeof(name_[0]), ' ');
                    strncpy(name_, randomName.c_str(), randomName.length());
                    lastChar_ = currentChar_ = randomName.length() - 1;
                } while (currentPath_.Descend(GetName()).Exists());
                isDirty_ = true;
                break;
            case 2:
                if (currentPath_.Descend(GetName()).Exists()) {
                    std::string res("Name " + std::string(name_) + " busy");
                    View::SetNotification(res.c_str(), -6);
                } else {
                    EndModal(1);
                }
                break;
            case 3:
                EndModal(0);
                break;
            }
        }
        if (mask & EPBM_UP) {
            name_[currentChar_]+=1;
			lastChar_=name_[currentChar_] ;
			isDirty_=true ;
        }
        if (mask&EPBM_DOWN) {
			name_[currentChar_]-=1;
			lastChar_=name_[currentChar_] ;
			isDirty_=true ;
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
