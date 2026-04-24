#include "CommandSelectorModal.h"
#include "Application/Utils/HelpLegend.h"
#include "Application/Utils/char.h"

static int GetDisplayCommandCount() {
    int count = CommandList::GetCount() - 1;
    return (count > 0) ? count : 0;
}

static FourCC GetDisplayCommandAt(int index) {
    return CommandList::GetAt(index + 1);
}

CommandSelectorModal::CommandSelectorModal(View &parentView,
                                           FourCC *liveTarget,
                                           ModalViewCallback previewCb):
    ModalView(parentView),
    selectedRow_(0),
    selectedCol_(0),
    selectedCommand_(I_CMD_NONE),
    parentView_(parentView),
    liveTarget_(liveTarget),
    savedCmd_(liveTarget ? *liveTarget : I_CMD_NONE),
    previewCb_(previewCb) {
    FourCC initial = liveTarget_ ? *liveTarget_ : I_CMD_NONE;
    if (initial != I_CMD_NONE) {
        moveToCommand(initial);
    } else {
        moveToCommand(I_CMD_ARPG);
    }
}

CommandSelectorModal::~CommandSelectorModal() {}

int CommandSelectorModal::commandToRow(FourCC command) const {
    int idx = CommandList::IndexOf(command) - 1;
    if (idx < 0) {
        return 0;
    }
    return idx / GRID_COLUMNS;
}

int CommandSelectorModal::commandToCol(FourCC command) const {
    int idx = CommandList::IndexOf(command) - 1;
    if (idx < 0) {
        return 0;
    }
    return idx % GRID_COLUMNS;
}

FourCC CommandSelectorModal::cellAtGridPos(int row, int col) const {
    int count = GetDisplayCommandCount();
    int index = row * GRID_COLUMNS + col;
    if (index >= count || index < 0) {
        return I_CMD_NONE;
    }
    return GetDisplayCommandAt(index);
}

void CommandSelectorModal::moveToCommand(FourCC command) {
    int idx = CommandList::IndexOf(command);
    if (idx < 0) {
        command = I_CMD_ARPG;
    }
    selectedCommand_ = command;
    selectedRow_ = commandToRow(command);
    selectedCol_ = commandToCol(command);
}

void CommandSelectorModal::navigateGrid(int deltaRow, int deltaCol) {
    int count = GetDisplayCommandCount();
    int rows = (count + GRID_COLUMNS - 1) / GRID_COLUMNS;

    int newRow = selectedRow_;
    int newCol = selectedCol_;
    int tries = rows * GRID_COLUMNS;

    while (tries-- > 0) {
        newRow += deltaRow;
        newCol += deltaCol;

        if (newRow < 0) {
            newRow = rows - 1;
        } else if (newRow >= rows) {
            newRow = 0;
        }

        if (newCol < 0) {
            newCol = GRID_COLUMNS - 1;
        } else if (newCol >= GRID_COLUMNS) {
            newCol = 0;
        }

        FourCC candidate = cellAtGridPos(newRow, newCol);
        if (candidate != I_CMD_NONE) {
            selectedRow_ = newRow;
            selectedCol_ = newCol;
            selectedCommand_ = candidate;
            if (liveTarget_) {
                *liveTarget_ = selectedCommand_;
            }
            if (previewCb_) {
                previewCb_(parentView_, *this);
            }
            return;
        }
    }
}

void CommandSelectorModal::ProcessButtonMask(unsigned short mask, bool pressed) {
    if (!pressed) {
        return;
    }

    if (mask & EPBM_UP) {
        navigateGrid(-1, 0);
        isDirty_ = true;
    } else if (mask & EPBM_DOWN) {
        navigateGrid(1, 0);
        isDirty_ = true;
    } else if (mask & EPBM_LEFT) {
        navigateGrid(0, -1);
        isDirty_ = true;
    } else if (mask & EPBM_RIGHT) {
        navigateGrid(0, 1);
        isDirty_ = true;
    } else if (mask & EPBM_A) {
        EndModal(1);  // Confirm selection
    } else if (mask & EPBM_B) {
        if (liveTarget_) {
            *liveTarget_ = savedCmd_;
        }
        EndModal(0);  // Cancel
    }
}

void CommandSelectorModal::DrawView() {
    int count = GetDisplayCommandCount();
    int rows = (count + GRID_COLUMNS - 1) / GRID_COLUMNS;
    int width = GRID_COLUMNS * 5;

    SetWindow(width, rows);

    GUITextProperties props;

    // Draw grid
    char cellStr[6];
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < GRID_COLUMNS; c++) {
            FourCC cmd = cellAtGridPos(r, c);
            if (cmd == I_CMD_NONE) {
                continue;
            }
            bool isSelected = (r == selectedRow_ && c == selectedCol_);

            fourCC2char(cmd, cellStr);
            cellStr[4] = 0;

            props.invert_ = isSelected;
            SetColor(isSelected ? CD_HILITE2 : CD_NORMAL);
            DrawString(c * 5, r, cellStr, props);
        }
    }

    props.invert_ = false;
    SetColor(CD_NORMAL);

    std::string *cmdStr = getHelpLegend(selectedCommand_);
    for (int i = 0; i < 3; i++) {
        // Clear legend area first so shorter lines don't leave stale text.
        View::DrawString(10, i, "                              ", props);
        View::DrawString(10, i, cmdStr[i].c_str(), props);

    }
}

void CommandSelectorModal::OnPlayerUpdate(PlayerEventType, unsigned int) {}
void CommandSelectorModal::OnFocus() {}
