#ifndef _MIXER_VIEW_H_
#define _MIXER_VIEW_H_

#include "BaseClasses/View.h"
#include "ViewData.h"

class MixerView: public View {
public:
	MixerView(GUIWindow &w,ViewData *viewData) ;
	~MixerView() ;
	virtual void ProcessButtonMask(unsigned short mask,bool pressed) ;
	virtual void DrawView() ;
	virtual void OnPlayerUpdate(PlayerEventType ,unsigned int tick=0) ;
    virtual void AnimationUpdate();
    virtual void OnFocus() ;
protected:
	void processNormalButtonMask(unsigned int mask) ;
    void processSelectionButtonMask(unsigned int mask) ;
	void onStart() ;
	void onStop() ;
	void updateCursor(int dx,int dy)  ;
    void toggleMute();
    void toggleSolo() ;
private:
	const char *song_ ;
    int soloChannel_;
    int mixerRow_; // 0=bus, 1=volume, 2=hpf, 3=lpf

    struct {                      // .Clipboard structure
        bool active_ ;            // .If currently making a selection
        unsigned char *data_ ;    // .Null if clipboard empty
        int x_ ;                  // .Current selection positions
        int y_ ;                  // .
        int offset_ ;             // .
        int width_ ;              // .Size of selection
        int height_ ;             // .
    } clipboard_;

    int saveX_;
    int saveY_ ;
	int saveOffset_ ;
	bool invertBatt_ ;
    void DrawVuBars();      // Draw VU bars - called from both DrawView and
                            // AnimationUpdate
    int vuBarHeightsL_[8];  // Left channel smoothed bar heights with slew rate decay
    int vuBarHeightsR_[8];  // Right channel smoothed bar heights with slew rate decay
};
#endif
