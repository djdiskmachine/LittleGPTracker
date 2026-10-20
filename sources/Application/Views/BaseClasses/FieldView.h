#ifndef _FIELD_VIEW_H_
#define _FIELD_VIEW_H_

#include "View.h"
#include "Foundation/T_SimpleList.h"
#include "UIField.h"

class FieldView: public View,public T_SimpleList<UIField> {
public:
	FieldView(GUIWindow &w,ViewData *viewData) ;

	virtual void Redraw() ;
	virtual void ProcessButtonMask(unsigned short mask) ;

	void SetFocus(UIField *) ;
	UIField *GetFocus() ;
	void ClearFocus() ;
	int GetFocusIndex() ;
	void SetSize(int size) ;

    virtual void DrawLabel(int x, int y, int index, const char *txt);
    virtual void DrawLabel(int x, int y, int minIndex, int maxIndex,
                                const char *txt);
private:
	T_SimpleList<UIField> fieldList_ ;
	UIField *focus_ ;
} ;

#endif
