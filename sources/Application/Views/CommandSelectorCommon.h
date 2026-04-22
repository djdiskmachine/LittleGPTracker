#ifndef _COMMAND_SELECTOR_COMMON_H_
#define _COMMAND_SELECTOR_COMMON_H_

#include "Application/Instruments/CommandList.h"
#include "Application/Utils/char.h"
#include "Application/Views/BaseClasses/View.h"

namespace CommandSelectorCommon {

static const int kColumns = 5;
static const int kCellPitch = 5;
static const int kScreenCenterX = 20;

inline int getPopupInnerWidth() { return kColumns * kCellPitch; }

inline int getPopupStartX() { return kScreenCenterX - (getPopupInnerWidth() / 2); }

inline int getCount() { return CommandList::GetCount(); }

inline int getRows() {
    int count = getCount();
    return (count + kColumns - 1) / kColumns;
}

inline int getPopupStartY(const GUIPoint &anchor) {
    int startY = anchor._y + (16 - getRows()) / 2;
    if (startY < anchor._y) {
        startY = anchor._y;
    }
    return startY;
}

/*
* Used to not overwrite the popup with the play cursor
*/
inline bool popupContainsPoint(const GUIPoint &anchor, int x, int y) {
    int startX = getPopupStartX() - 1;
    int endX = getPopupStartX() + getPopupInnerWidth();
    int startY = getPopupStartY(anchor) - 1;
    int endY = getPopupStartY(anchor) + getRows();
    return x >= startX && x <= endX && y >= startY && y <= endY;
}

inline int wrapIndex(int index) {
    int count = getCount();
    if (count <= 0) {
        return 0;
    }
    while (index < 0) {
        index += count;
    }
    return index % count;
}

inline int getSelectedIndex(FourCC command) {
    int idx = CommandList::IndexOf(command);
    if (idx < 0) {
        return 0;
    }
    return idx;
}

inline bool isCommandColumn(int col, int c1, int c2) {
    return col == c1 || col == c2;
}

inline bool isCommandColumn(int col, int c1, int c2, int c3) {
    return col == c1 || col == c2 || col == c3;
}

inline FourCC *getCommandPointerByCol(int col, int c1, FourCC *p1, int c2,
                                      FourCC *p2) {
    if (col == c1) {
        return p1;
    }
    if (col == c2) {
        return p2;
    }
    return 0;
}

inline FourCC *getCommandPointerByCol(int col, int c1, FourCC *p1, int c2,
                                      FourCC *p2, int c3, FourCC *p3) {
    if (col == c1) {
        return p1;
    }
    if (col == c2) {
        return p2;
    }
    if (col == c3) {
        return p3;
    }
    return 0;
}

inline FourCC atIndex(int index) {
    return CommandList::GetAt(wrapIndex(index));
}

inline FourCC stepHorizontal(FourCC current, int delta) {
    int count = getCount();
    if (count <= 0) {
        return I_CMD_NONE;
    }

    int rows = getRows();
    int idx = getSelectedIndex(current);
    int col = idx % kColumns;
    int row = idx / kColumns;

    col += delta;
    if (col < 0) {
        col = kColumns - 1;
    } else if (col >= kColumns) {
        col = 0;
    }

    int candidate = row * kColumns + col;
    while (candidate >= count) {
        col += (delta < 0) ? -1 : 1;
        if (col < 0) {
            col = kColumns - 1;
        } else if (col >= kColumns) {
            col = 0;
        }
        candidate = row * kColumns + col;
    }

    return atIndex(candidate);
}

inline FourCC stepVertical(FourCC current, int rowDelta) {
    int count = getCount();
    if (count <= 0) {
        return I_CMD_NONE;
    }

    int rows = getRows();
    int idx = getSelectedIndex(current);
    int col = idx % kColumns;
    int row = idx / kColumns;

    row += rowDelta;
    if (row < 0) {
        row = rows - 1;
    } else if (row >= rows) {
        row = 0;
    }

    int candidate = row * kColumns + col;
    while (candidate >= count) {
        row += (rowDelta < 0) ? -1 : 1;
        if (row < 0) {
            row = rows - 1;
        } else if (row >= rows) {
            row = 0;
        }
        candidate = row * kColumns + col;
    }

    return atIndex(candidate);
}

inline FourCC stepByDirection(FourCC current, ViewUpdateDirection direction) {
    switch (direction) {
    case VUD_LEFT:
        return stepHorizontal(current, -1);
    case VUD_RIGHT:
        return stepHorizontal(current, 1);
    case VUD_UP:
        return stepVertical(current, -1);
    case VUD_DOWN:
        return stepVertical(current, 1);
    }
    return current;
}

template <typename GetCommandPtrFn>
inline void enterSelector(bool commandColumn, GetCommandPtrFn getCommandPointer,
                          bool &active, FourCC &original, bool &dirty) {
    if (!commandColumn) {
        return;
    }
    FourCC *command = getCommandPointer();
    if (!command) {
        return;
    }
    original = *command;
    active = true;
    dirty = true;
}

template <typename GetCommandPtrFn>
inline void leaveSelector(bool commit, GetCommandPtrFn getCommandPointer,
                          bool &active, FourCC original, int &lastCmd,
                          bool &dirty) {
    if (!active) {
        return;
    }
    if (!commit) {
        FourCC *command = getCommandPointer();
        if (command) {
            *command = original;
            lastCmd = *command;
        }
    }
    active = false;
    dirty = true;
}

template <typename GetCommandPtrFn>
inline void stepSelector(ViewUpdateDirection direction,
                         GetCommandPtrFn getCommandPointer, bool active,
                         int &lastCmd, bool &dirty) {
    if (!active) {
        return;
    }
    FourCC *command = getCommandPointer();
    if (!command) {
        return;
    }
    *command = stepByDirection(*command, direction);
    lastCmd = *command;
    dirty = true;
}

template <typename SetColorFn, typename DrawStringFn>
inline void drawPopup(FourCC selectedCommand, const GUIPoint &anchor,
                      GUITextProperties &props, SetColorFn setColor,
                      DrawStringFn drawString) {
    const int columns = kColumns;
    const int rows = getRows();
    const int count = getCount();
    int selectedIndex = getSelectedIndex(selectedCommand);

    int startY = getPopupStartY(anchor);
    int startX = getPopupStartX();

    GUITextProperties boxProps = props;
    boxProps.invert_ = false;
    setColor(CD_BORDER);

    char borderTop[28];
    char borderMid[28];
    char borderBottom[28];
    borderTop[0] = '+';
    borderMid[0] = '|';
    borderBottom[0] = '+';
    int innerWidth = getPopupInnerWidth();
    for (int i = 1; i <= innerWidth; i++) {
        borderTop[i] = '-';
        borderMid[i] = ' ';
        borderBottom[i] = '-';
    }
    borderTop[innerWidth + 1] = '+';
    borderMid[innerWidth + 1] = '|';
    borderBottom[innerWidth + 1] = '+';
    borderTop[innerWidth + 2] = 0;
    borderMid[innerWidth + 2] = 0;
    borderBottom[innerWidth + 2] = 0;

    drawString(startX - 1, startY - 1, borderTop, boxProps);
    for (int r = 0; r < rows; r++) {
        drawString(startX - 1, startY + r, borderMid, boxProps);
    }
    drawString(startX - 1, startY + rows, borderBottom, boxProps);

    GUITextProperties cellProps = props;
    char cellStr[5];
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < columns; c++) {
            int index = r * columns + c;
            bool isSelected = (index == selectedIndex);

            if (index < count) {
                fourCC2char(CommandList::GetAt(index), cellStr);
                cellStr[4] = 0;
            } else {
                cellStr[0] = ' ';
                cellStr[1] = ' ';
                cellStr[2] = ' ';
                cellStr[3] = ' ';
                cellStr[4] = 0;
            }

            setColor(CD_NORMAL);
            cellProps.invert_ = isSelected;
            drawString(startX + c * kCellPitch, startY + r, cellStr, cellProps);
        }
    }

    setColor(CD_NORMAL);
}

} // namespace CommandSelectorCommon

#endif
