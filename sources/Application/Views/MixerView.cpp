#include "MixerView.h"
#include "Application/Mixer/MixerService.h"
#include "Application/Model/Mixer.h"
#include "Application/Player/Player.h"
#include "Application/Player/PlayerMixer.h"
#include "Application/Utils/char.h"
#include "UIController.h"
#include "VuMeterUtil.h"
#include <iostream>
#include <sstream>
#include <string>

MixerView::MixerView(GUIWindow &w,ViewData *viewData):View(w,viewData) {
	clipboard_.active_=false ;
	clipboard_.data_=0 ;
	invertBatt_=false;
    soloChannel_ = -1;
    mixerRow_ = 1; // start on volume row
    for (int i = 0; i < 8; i++) {
        vuBarHeightsL_[i] = 0;
        vuBarHeightsR_[i] = 0;
    }
}

MixerView::~MixerView() {
} 


void MixerView::onStart() {
	Player *player=Player::GetInstance() ;
	unsigned char from=viewData_->songX_ ;
	unsigned char to=from ;
	//if (clipboard_.active_) {
	//	GUIRect r=getSelectionRect();
	//	from=r.Left() ;
	//	to=r.Right() ;
	//}
	player->OnStartButton(PM_SONG,from,false,to) ;
} ;

void MixerView::onStop() {
	Player *player=Player::GetInstance() ;
	unsigned char from=viewData_->songX_ ;
	unsigned char to=from ;
	player->OnStartButton(PM_SONG,from,true,to) ;
} ;

void MixerView::OnFocus() {
} ;

void MixerView::updateCursor(int dx,int dy) {
    if (dy != 0) {
        // UP/DOWN switches between rows
		mixerRow_ = (mixerRow_ == 1) ? 2 : 1;
		isDirty_ = true;
    }
    if (dx != 0) {
		// LEFT/RIGHT switches between channels
		int x = viewData_->mixerCol_;
		x += dx;
		if (x < 0) x = 0;
		if (x > 7) x = 7;
		viewData_->mixerCol_ = x;
		isDirty_ = true;
	}
}

void MixerView::toggleMute() {
    int col = viewData_->mixerCol_;
    UIController::GetInstance()->ToggleMute(col,col) ;
	isDirty_=true ;
};

void MixerView::toggleSolo() {
	int col=viewData_->mixerCol_ ;
	bool entering=(soloChannel_!=col) ;
	UIController::GetInstance()->SwitchSoloMode(col,col,entering) ;
	soloChannel_=entering ? col : -1 ;
	isDirty_=true ;
} ;

void MixerView::ProcessButtonMask(unsigned short mask,bool pressed) {
	//if (!pressed) {
	//	if (viewMode_==VM_MUTEON) {
	//		if (mask&EPBM_R) {
	//			toggleMute() ;
	//		}
	//	} ;
	//	if (viewMode_==VM_SOLOON) {
	//		if (mask&EPBM_R) {
	//			switchSoloMode() ;
	//		}
	//	} ;
	//	return ;
	//} ;
	//
	
	if (clipboard_.active_) {
		viewMode_=VM_SELECTION ;
	} ;
	// Process selection related keys
	
	if (viewMode_==VM_SELECTION) {
        if (clipboard_.active_==false) {
            clipboard_.active_=true ;
            clipboard_.x_=viewData_->songX_ ;
            clipboard_.y_=viewData_->songY_ ;
            clipboard_.offset_=viewData_->songOffset_ ;
			saveX_=clipboard_.x_ ;
			saveY_=clipboard_.y_ ;
			saveOffset_=clipboard_.offset_ ;
        }
        processSelectionButtonMask(mask) ;
    } else {
	   
       // Switch back to normal mode

        viewMode_=VM_NORMAL ;
        processNormalButtonMask(mask) ;
    }
} ;


/******************************************************
 processNormalButtonMask:
        process button mask in the case there is no
        selection active
 ******************************************************/
 
void MixerView::processNormalButtonMask(unsigned int mask) {

	// B Modifier

	if (mask&EPBM_B) {
        if (mask & EPBM_A) {
            // B + A = cut: reset volume to full
			Mixer::GetInstance()->SetChannelVolume(viewData_->mixerCol_, 0xFF);
			isDirty_=true;
        } else {
            toggleMute();
        }
    } else {

	  // A modifier

	  if (mask&EPBM_A) {
          if (mixerRow_ == 2) {
              // On HPF row: A cycles HPF mode
			  Mixer *m = Mixer::GetInstance();
			  int col = viewData_->mixerCol_;
			  int mode = m->GetChannelHPF(col);
			  mode = (mode + 1) % 3;
			  m->SetChannelHPF(col, mode);
			  isDirty_ = true;
			  
			  const char *modeStr = (mode == 0) ? "OFF" : (mode == 1) ? "20Hz" : "90Hz";
			  std::string notif = std::string("      High Pass Filter: ") + modeStr;
			  SetNotification(notif.c_str());
          } else if (mixerRow_ == 1) {
              // On volume row: A adjusts volume
			  Mixer *mixer = Mixer::GetInstance();
			  int col = viewData_->mixerCol_;
			  int currentVol = mixer->GetChannelVolume(col);
			  int newVol = currentVol;

			  // Fine adjustment (UP/DOWN)
			  if (mask & EPBM_UP) {
				  newVol = currentVol + 1;
			  }
			  if (mask&EPBM_DOWN) {
				  newVol = currentVol - 1;
			  }
			  
			  // Coarse adjustment (RIGHT/LEFT)
			  if (mask&EPBM_RIGHT) {
				  newVol = currentVol + 16;
			  }
			  if (mask&EPBM_LEFT) {
				  newVol = currentVol - 16;
			  }
			  
			  // Clamp to valid range (0-255)
			  if (newVol < 0) newVol = 0;
			  if (newVol > 255) newVol = 255;
			  
			  if (newVol != currentVol) {
				  mixer->SetChannelVolume(col, newVol);
				  isDirty_ = true;
			  }
          }

          if (mask & EPBM_R) {
              toggleSolo();
          }

      } else {
          // R Modifier
          if (mask & EPBM_R) {
              if (mask & EPBM_UP) {
                  ViewType vt = VT_SONG;
                  ViewEvent ve(VET_SWITCH_VIEW, &vt);
                  SetChanged();
                  NotifyObservers(&ve);
              } else if (mask & EPBM_RIGHT) {
                  ViewType vt = VT_TABLE;
                  ViewEvent ve(VET_SWITCH_VIEW, &vt);
                  SetChanged();
                  NotifyObservers(&ve);
              }
              if (mask & EPBM_START) {
                  onStop();
              }
	    	} else {

                // L Modifier / Normal

                // No L modifier - normal cursor movement
                if (mask & EPBM_UP)
                    updateCursor(0, -1);
                if (mask & EPBM_DOWN)
                    updateCursor(0, 1);
                if (mask & EPBM_LEFT)
                    updateCursor(-1, 0);
                if (mask & EPBM_RIGHT)
                    updateCursor(1, 0);

                if (mask & EPBM_START) {
                    onStart();
                }
            }
      }
    }
} ;

/******************************************************
 processSelectionButtonMask:
        process button mask in the case there is a
        selection active
 ******************************************************/
 
void MixerView::processSelectionButtonMask(unsigned int mask) {

	// B Modifier

	if (mask&EPBM_B) {

    } else {

	  // A modifier

	  if (mask&EPBM_A) {

	  } else {

		  // R Modifier

          	if (mask&EPBM_R) {
 				if (mask&EPBM_START) {
				    onStop() ;
                }
	    	} else {

    			// No modifier
	          		if (mask&EPBM_START) {
					   onStart() ;
	    			}
		    }
	  } 
	}
}

void MixerView::DrawView() {

	Clear() ;

	GUITextProperties props ;
	GUIPoint pos=GetTitlePosition() ;
	GUIPoint anchor=GetAnchor() ;
	char hex[3] ;

// Draw title

	SetColor(CD_NORMAL) ;

	Player *player=Player::GetInstance() ;
	
	std::ostringstream os;

	os << ((player->GetSequencerMode()==SM_SONG)?"Song":"Live") ;

    std::string buffer(os.str());

    DrawString(pos._x, pos._y, buffer.c_str(), props);

    // Draw mixer grid with row labels
    pos = anchor;
    short dx = 3;
    Mixer *mixer = Mixer::GetInstance();
    Player *playerInst = Player::GetInstance();
    GUITextProperties rowLabelProps;
    
    // Row 0: bus routing
    if (mixerRow_ == 0) {
        rowLabelProps.invert_ = true;
        SetColor(CD_HILITE2);
    } else {
        rowLabelProps.invert_ = false;
        SetColor(CD_NORMAL);
    }
    DrawString(pos._x - 3, pos._y, " ", rowLabelProps);
    
    for (int i = 0; i < 8; i++) {
        props.invert_ = (i == viewData_->mixerCol_ && mixerRow_ == 0);
        SetColor((i == viewData_->mixerCol_ && mixerRow_ == 0) ? CD_HILITE2 : CD_NORMAL);

        if (soloChannel_ == i) {
            DrawString(pos._x, pos._y, "S ", props);
        } else if (playerInst->IsChannelMuted(i)) {
            DrawString(pos._x, pos._y, "M ", props);
        } else {
            int bus = mixer->GetBus(i);
            hex2char(bus, hex);
            DrawString(pos._x, pos._y, hex, props);
        }
        pos._x += dx;
    }
    
    // Row 1: volumes
    pos = anchor;
    pos._y += 1;
    if (mixerRow_ == 1) {
        rowLabelProps.invert_ = true;
        SetColor(CD_HILITE2);
    } else {
        rowLabelProps.invert_ = false;
        SetColor(CD_NORMAL);
    }
    DrawString(pos._x - 5, pos._y, "VOL", rowLabelProps);
    
    pos._x = anchor._x;
    for (int i = 0; i < 8; i++) {
        props.invert_ = (i == viewData_->mixerCol_ && mixerRow_ == 1);
        SetColor((i == viewData_->mixerCol_ && mixerRow_ == 1) ? CD_HILITE2 : CD_NORMAL);

        int vol = mixer->GetChannelVolume(i);
        hex2char(vol, hex);
        DrawString(pos._x, pos._y, hex, props);
        pos._x += dx;
    }

    // Row 2: HPF mode
    pos = anchor;
    pos._y += 3;
    if (mixerRow_ == 2) {
        rowLabelProps.invert_ = true;
        SetColor(CD_HILITE2);
    } else {
        rowLabelProps.invert_ = false;
        SetColor(CD_NORMAL);
    }
    DrawString(pos._x - 5, pos._y, "HPF", rowLabelProps);
    
    pos._x = anchor._x;
    for (int i = 0; i < 8; i++) {
        props.invert_ = (i == viewData_->mixerCol_ && mixerRow_ == 2);
        SetColor((i == viewData_->mixerCol_ && mixerRow_ == 2) ? CD_HILITE2 : CD_NORMAL);
        
        int hpf = mixer->GetChannelHPF(i);
        char code[4];
        if (hpf == 0) {
            code[0] = 'O'; code[1] = 'F'; code[2] = ' '; code[3] = '\0';
        } else if (hpf == 1) {
            code[0] = '2'; code[1] = '0'; code[2] = ' '; code[3] = '\0';
        } else {
            code[0] = '9'; code[1] = '0'; code[2] = ' '; code[3] = '\0';
        }
        DrawString(pos._x, pos._y, code, props);
        pos._x += dx;
    }

    drawMap() ;
	drawNotes() ;
    EnableNotification();

    if (player->IsRunning()) {
		OnPlayerUpdate(PET_UPDATE) ;
	} ;

    // Draw VU bars at the end so they appear on top of everything
    // They will always show the empty dashes and smoothly decay when paused
    DrawVuBars();
} ;

void MixerView::OnPlayerUpdate(PlayerEventType ,unsigned int tick) {

	Player *player=Player::GetInstance() ;

	// Draw clipping indicator & CPU usage

	GUIPoint anchor=GetAnchor() ;
	GUIPoint pos=anchor ;

	GUITextProperties props ;
	SetColor(CD_NORMAL) ;

    if (View::miniLayout_) {
      pos._y=0 ;
      pos._x=25 ;
    } else {
      pos=anchor ;
      pos._x+=25 ;
    }
    
	if (player->Clipped()) {
           DrawString(pos._x,pos._y,"clip",props); 
    } else {
           DrawString(pos._x,pos._y,"----",props); 
    }
	char strbuffer[10] ;

	pos._y+=1 ;
	sprintf(strbuffer,"%3.3d%%",player->GetPlayedBufferPercentage()) ; 
	DrawString(pos._x,pos._y,strbuffer,props) ;

    System *sys=System::GetInstance() ;
    int batt=sys->GetBatteryLevel() ;
    if (batt>=0) {
		if (batt<90) {
			SetColor(CD_HILITE2) ;
			invertBatt_=!invertBatt_ ;
		} else {
			invertBatt_=false ;
		} ;
		props.invert_=invertBatt_ ;

	    pos._y+=1 ;
    	sprintf(strbuffer,"%3.3d",batt) ; 
	    DrawString(pos._x,pos._y,strbuffer,props) ;
    }
	SetColor(CD_NORMAL) ;
	props.invert_=false ;
    int time=int(player->GetPlayTime()) ;
    int mi=time/60 ;
    int se=time-mi*60 ;
	sprintf(strbuffer,"%2.2d:%2.2d",mi,se) ; 
	pos._y+=1 ;	
	DrawString(pos._x,pos._y,strbuffer,props) ;

    drawNotes() ;

} ;

/******************************************************
 DrawVuBars:
        Draw VU meters below the mixer controls
        Called from both AnimationUpdate and DrawView to ensure
        bars are always visible and never flicker
 ******************************************************/

void MixerView::DrawVuBars() {
    // NOTE: Common VU meter logic extracted to VuMeterUtil
    // Uses: GetVuPeakLevelsStereo for L/R decay, GetVuBarColor for colors, DRAW_VU_BAR_ROW for rendering
    // Draws stereo L/R for each of the 8 channels
    
    Player *player = Player::GetInstance();
    
    GUIPoint vuPos = GetAnchor();
    // VU meter starts at anchor row and grows upward for 8 rows
    vuPos._y += 16;  // Position below the row labels (bus, vol, hpf)
    
    GUITextProperties vuProps;
    vuProps.invert_ = true;
    
    MixerService *ms = MixerService::GetInstance();
    short dx = 3;  // 3 chars per channel (L and R + 1 space)
    
    // When playback stops, immediately clear the bars instead of letting them decay
    if (!player->IsRunning()) {
        for (int i = 0; i < 8; i++) {
            vuBarHeightsL_[i] = 0;
            vuBarHeightsR_[i] = 0;
        }
    }
    
    // Extract L/R peak levels from all channels
    float peakLevelsL[8];
    float peakLevelsR[8];
    for (int i = 0; i < 8; i++) {
        MixBus *bus = ms->GetMixBus(i);
        if (bus) {
            uint32_t level = bus->GetPeakLevel();
            // Extract L and R from packed format: (left_16bits << 16) | right_16bits
            int leftPeak = (level >> 16) & 0xFFFF;
            int rightPeak = level & 0xFFFF;
            // Normalize to 0.0-1.0
            peakLevelsL[i] = (float)leftPeak / 32767.0f;
            peakLevelsR[i] = (float)rightPeak / 32767.0f;
        } else {
            peakLevelsL[i] = 0.0f;
            peakLevelsR[i] = 0.0f;
        }
        // Force to 0 when not playing
        if (!player->IsRunning()) {
            peakLevelsL[i] = 0.0f;
            peakLevelsR[i] = 0.0f;
        }
    }
    
    // Update bar heights with slew rate decay for both L and R
    int displayHeightsL[8];
    int displayHeightsR[8];
    GetVuPeakLevelsStereo(vuBarHeightsL_, vuBarHeightsR_, 
                          displayHeightsL, displayHeightsR,
                          peakLevelsL, peakLevelsR);
    
    // Draw vertical VU bars for L and R channels (two columns per channel)
    // Each bar is 8 rows tall, growing upward
    for (int row = 0; row < VU_METER_HEIGHT; row++) {
        // Set color based on level threshold
        SetColor(GetVuBarColor(row));
        
        // Draw left and right channels for all 8 channels
        for (int i = 0; i < 8; i++) {
            GUIPoint pos = vuPos;
            pos._x += i * dx;  // Each channel gets dx spacing
            pos._y -= row;     // Grow upward from the anchor position
            
            // Draw left channel
            DRAW_VU_BAR_ROW(this, pos, row, displayHeightsL[i], vuProps);
            SetColor(GetVuBarColor(row));
            
            // Draw right channel at x+1 (one character to the right)
            pos._x += 1;
            DRAW_VU_BAR_ROW(this, pos, row, displayHeightsR[i], vuProps);
            SetColor(GetVuBarColor(row));
        }
    }
    
    SetColor(CD_NORMAL);
}

void MixerView::AnimationUpdate() {
    DrawVuBars();
} ;
