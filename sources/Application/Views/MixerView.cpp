#include "MixerView.h"
#include "Application/Mixer/MixerService.h"
#include "Application/Model/Mixer.h"
#include "Application/Player/Player.h"
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
    previousViewType_ = VT_SONG; // Default to SongView
    for (int i = 0; i < 9; i++) {
        vuBarHeightsL_[i] = 0;
        vuBarHeightsR_[i] = 0;
    }
}

MixerView::~MixerView() {}

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
}

// Stay on the same column as SongView
void MixerView::OnFocus() { viewData_->mixerCol_ = viewData_->songX_; }

void MixerView::updateCursor(int dx,int dy) {
    if (dy != 0) {
        // UP/DOWN cycles between rows 1, 2, 3
        if (dy < 0) {
            mixerRow_ = (mixerRow_ == 1) ? 3 : mixerRow_ - 1;
        } else {
            mixerRow_ = (mixerRow_ == 3) ? 1 : mixerRow_ + 1;
        }
		isDirty_ = true;
    }
    if (dx != 0) {
        // LEFT/RIGHT switches between channels (0-7 only, Master is
        // display-only)
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
    UIController::GetInstance()->ToggleMute(col, col);
    isDirty_ = true;
};

void MixerView::toggleSolo() {
	int col=viewData_->mixerCol_ ;
	bool entering=(soloChannel_!=col) ;
	UIController::GetInstance()->SwitchSoloMode(col,col,entering) ;
    soloChannel_ = entering ? col : -1;
    isDirty_ = true;
}

void MixerView::unMuteAll() {
    UIController *controller = UIController::GetInstance();
    controller->UnMuteAll();
    isDirty_ = true;
}

void MixerView::ProcessButtonMask(unsigned short mask, bool pressed) {

    if (clipboard_.active_) {
		viewMode_=VM_SELECTION ;
	} ;
	// Process selection related keys

    if (viewMode_ == VM_SELECTION) {
        if (clipboard_.active_==false) {
            clipboard_.active_=true ;
            clipboard_.x_ = viewData_->songX_;
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
        processNormalButtonMask(mask);
    }
};

/******************************************************
 processNormalButtonMask:
        process button mask in the case there is no
        selection active
 ******************************************************/

void MixerView::processNormalButtonMask(unsigned int mask) {

    // R Modifier
    if (mask & EPBM_R) {
        if (mask & EPBM_B) {
            toggleMute();
        } else if (mask & EPBM_A) {
            toggleSolo();
        } else if (mask & EPBM_L) {
            unMuteAll();
        } else if (mask & EPBM_UP) {
            // R + UP = go back to previous view
            ViewEvent ve(VET_SWITCH_VIEW, &previousViewType_);
            // Go back to the same column as in MixerView
            viewData_->songX_ = viewData_->mixerCol_;
            SetChanged();
            NotifyObservers(&ve);
        } else if (mask & EPBM_RIGHT) {
            ViewType vt = VT_TABLE;
            ViewEvent ve(VET_SWITCH_VIEW, &vt);
            // Go back to the same column as in MixerView
            viewData_->songX_ = viewData_->mixerCol_;
            unsigned char *data = viewData_->GetCurrentSongPointer();
            if (*data != 0xFF) { // Set chain
                viewData_->currentChain_ = *data;
            }
            data = viewData_->GetCurrentChainPointer();
            if (*data != 0xFF) { // Set phrase
                viewData_->currentPhrase_ = *data;
            }
            SetChanged();
            NotifyObservers(&ve);
        } else if (mask & EPBM_START) {
            onStop();
        }
    } else if (mask & EPBM_B) {
        if (mask & EPBM_A) {
            // B + A = cut: reset volume to full
            Mixer::GetInstance()->SetChannelVolume(viewData_->mixerCol_, 0xFF);
            isDirty_ = true;
        }
    } else if (mask & EPBM_A) {
        if (mixerRow_ == 3) {
            // On LPF row: A+Up/Down = coarse ×2/÷2, A+Left/Right = fine
            // ±10Hz A alone (no direction) = toggle off/1000Hz
            Mixer *m = Mixer::GetInstance();
            int col = viewData_->mixerCol_;
            unsigned short freq = m->GetChannelLPF(col);
            unsigned short newFreq = freq;
            if (mask & EPBM_UP) {
                if (freq == 0)
                    newFreq = 20;
                else {
                    unsigned short step = freq / 10 < 1 ? 1 : freq / 10;
                    newFreq =
                        (unsigned short)(freq + step > 20000 ? 0
                                                                : freq + step);
                }
            } else if (mask & EPBM_DOWN) {
                if (freq == 0)
                    newFreq = 20000;
                else {
                    unsigned short step = freq / 10 < 1 ? 1 : freq / 10;
                    newFreq = (freq <= step)
                                    ? 0
                                    : (unsigned short)(freq - step < 20
                                                            ? 0
                                                            : freq - step);
                }
            } else if (mask & EPBM_RIGHT) {
                if (freq == 0)
                    newFreq = 20;
                else
                    newFreq =
                        (unsigned short)(freq + 10 > 20000 ? 0 : freq + 10);
            } else if (mask & EPBM_LEFT) {
                if (freq == 0)
                    newFreq = 20000;
                else
                    newFreq =
                        (freq <= 10)
                            ? 0
                            : (unsigned short)(freq - 10 < 20 ? 0
                                                                : freq - 10);
            }
            if (newFreq != freq) {
                m->SetChannelLPF(col, newFreq);
                isDirty_ = true;
            }
            // show notification
            if (newFreq == 0) {
                SetNotification("       Low Pass Filter: OFF");
            } else {
                char notifBuf[40];
                sprintf(notifBuf, "       Low Pass Filter: %dHz",
                        (int)newFreq);
                SetNotification(notifBuf);
            }
        } else if (mixerRow_ == 2) {
            // On HPF row: A+Right cycles forward, A+Left cycles backward
            if (mask & (EPBM_RIGHT | EPBM_LEFT)) {
                Mixer *m = Mixer::GetInstance();
                int col = viewData_->mixerCol_;
                int mode = m->GetChannelHPF(col);
                if (mask & EPBM_RIGHT) {
                    mode = (mode + 1) % 3;
                } else {
                    mode = (mode + 2) % 3;
                }
                m->SetChannelHPF(col, mode);
                isDirty_ = true;
                const char *modeStr = (mode == 0)   ? "OFF"
                                        : (mode == 1) ? "20Hz"
                                                    : "90Hz";
                std::string notif =
                    std::string("      High Pass Filter: ") + modeStr;
                SetNotification(notif.c_str());
            }
        } else if (mixerRow_ == 1) {
            // On volume row: A adjusts volume
            Mixer *mixer = Mixer::GetInstance();
            int col = viewData_->mixerCol_;
            int currentVol = mixer->GetChannelVolume(col);
            int newVol = currentVol;

            // Coarse adjustment (UP/DOWN)
            if (mask & EPBM_UP) {
                newVol = currentVol + 16;
            }
            if (mask & EPBM_DOWN) {
                newVol = currentVol - 16;
            }

            // Fine adjustment (RIGHT/LEFT)
            if (mask & EPBM_RIGHT) {
                newVol = currentVol + 1;
            }
            if (mask & EPBM_LEFT) {
                newVol = currentVol - 1;
            }

            // Clamp to valid range (0-255)
            if (newVol < 0)
                newVol = 0;
            if (newVol > 255)
                newVol = 255;

            if (newVol != currentVol) {
                mixer->SetChannelVolume(col, newVol);
                isDirty_ = true;
            }
        }
    } else {
        // Normal cursor movement (no modifier)
		// This works everywhere, including Master channel
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

/******************************************************
 processSelectionButtonMask:
        process button mask in the case there is a
        selection active
 ******************************************************/

void MixerView::processSelectionButtonMask(unsigned int mask) {

	// B Modifier

    if (mask & EPBM_B) {

    } else {

        // A modifier

        if (mask & EPBM_A) {

        } else {

            // R Modifier

            if (mask & EPBM_R) {
                if (mask&EPBM_START) {
				    onStop() ;
                }
            } else {

                // No modifier
                if (mask & EPBM_START) {
                    onStart();
                }
            }
        }
    }
}

void MixerView::DrawView() {

    Clear();

    GUITextProperties props;
    GUIPoint pos=GetTitlePosition() ;
	GUIPoint anchor=GetAnchor() ;
	char hex[3] ;

// Draw title

	SetColor(CD_NORMAL) ;

    Player *player = Player::GetInstance();

    std::ostringstream os;

    os << ((player->GetSequencerMode() == SM_SONG) ? "Song" : "Live");

    std::string buffer(os.str());

    DrawString(pos._x, pos._y, buffer.c_str(), props);

    // Draw mixer grid with row labels
    pos = anchor;
    short dx = 3;
    Mixer *mixer = Mixer::GetInstance();
    Player *playerInst = Player::GetInstance();
    GUITextProperties rowLabelProps;

    // Row 0: Track number / Solo / Mute
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
        SetColor((i == viewData_->mixerCol_ && mixerRow_ == 0) ? CD_HILITE2
                                                               : CD_COL_TITLE);

        if (playerInst->IsChannelMuted(i)) {
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
        SetColor((i == viewData_->mixerCol_ && mixerRow_ == 1) ? CD_HILITE2
                                                               : CD_NORMAL);

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
            code[0] = '-';
            code[1] = '-';
            code[2] = ' ';
            code[3] = '\0';
        } else if (hpf == 1) {
            code[0] = '2'; code[1] = '0'; code[2] = ' '; code[3] = '\0';
        } else {
            code[0] = '9'; code[1] = '0'; code[2] = ' '; code[3] = '\0';
        }
        DrawString(pos._x, pos._y, code, props);
        pos._x += dx;
    }

    // Row 3: LPF frequency
    pos = anchor;
    pos._y += 4;
    if (mixerRow_ == 3) {
        rowLabelProps.invert_ = true;
        SetColor(CD_HILITE2);
    } else {
        rowLabelProps.invert_ = false;
        SetColor(CD_NORMAL);
    }
    DrawString(pos._x - 5, pos._y, "LPF", rowLabelProps);

    pos._x = anchor._x;
    for (int i = 0; i < 8; i++) {
        props.invert_ = (i == viewData_->mixerCol_ && mixerRow_ == 3);
        SetColor((i == viewData_->mixerCol_ && mixerRow_ == 3) ? CD_HILITE2 : CD_NORMAL);

        unsigned short lpf = mixer->GetChannelLPF(i);
        char lpfCode[5];
        if (lpf == 0) {
            lpfCode[0] = '-'; lpfCode[1] = '-'; lpfCode[2] = ' '; lpfCode[3] = '\0';
        } else if (lpf < 100) {
            lpfCode[0] = '0' + (lpf / 10);
            lpfCode[1] = '0' + (lpf % 10);
            lpfCode[2] = ' '; lpfCode[3] = '\0';
        } else if (lpf < 1000) {
            lpfCode[0] = '0' + (lpf / 100);
            lpfCode[1] = 'h';
            lpfCode[2] = ' '; lpfCode[3] = '\0';
        } else if (lpf < 10000) {
            lpfCode[0] = '0' + (lpf / 1000);
            lpfCode[1] = 'k';
            lpfCode[2] = ' '; lpfCode[3] = '\0';
        } else {
            lpfCode[0] = '0' + (lpf / 10000);
            lpfCode[1] = '0' + (lpf % 10000) / 1000;
            lpfCode[2] = 'k';
            lpfCode[3] = ' '; lpfCode[4] = '\0';
        }
        DrawString(pos._x, pos._y, lpfCode, props);
        pos._x += dx;
    }

    drawMap() ;
	drawNotes() ;
    EnableNotification();

    if (player->IsRunning()) {
        OnPlayerUpdate(PET_UPDATE);
    }

    // Draw VU bars at the end so they appear on top of everything
    // They will always show the empty dashes and smoothly decay when paused
    DrawVuBars();
};

void MixerView::OnPlayerUpdate(PlayerEventType, unsigned int tick) {

    Player *player = Player::GetInstance();

    // Draw clipping indicator & CPU usage

    GUIPoint anchor = GetAnchor();
    GUIPoint pos=anchor ;

    GUITextProperties props;
    SetColor(CD_NORMAL) ;

    if (View::miniLayout_) {
      pos._y=0 ;
      pos._x=25 ;
    } else {
      pos=anchor ;
      pos._x+=25 ;
    }

    if (player->Clipped()) {
        DrawString(pos._x, pos._y, "clip", props);
    } else {
        DrawString(pos._x, pos._y, "----", props);
    }
    char strbuffer[10];

    pos._y += 1;
    sprintf(strbuffer,"%3.3d%%",player->GetPlayedBufferPercentage()) ;
	DrawString(pos._x,pos._y,strbuffer,props) ;

    System *sys = System::GetInstance();
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
        sprintf(strbuffer, "%3.3d", batt);
        DrawString(pos._x,pos._y,strbuffer,props) ;
    }
    SetColor(CD_NORMAL);
    props.invert_=false ;
    int time=int(player->GetPlayTime()) ;
    int mi=time/60 ;
    int se=time-mi*60 ;
    sprintf(strbuffer, "%2.2d:%2.2d", mi, se);
    pos._y += 1;
    DrawString(pos._x,pos._y,strbuffer,props) ;

    drawNotes() ;
};

/******************************************************
 DrawVuBars:
        Draw VU meters below the mixer controls
        Called from both AnimationUpdate and DrawView to ensure
        bars are always visible and never flicker
 ******************************************************/

void MixerView::DrawVuBars() {
    Player *player = Player::GetInstance();

    GUIPoint vuPos = GetAnchor();
    vuPos._y += 16; // Position below the row labels (bus, vol, hpf, lpf)

    GUITextProperties vuProps;
    vuProps.invert_ = true;

    short dx = 3;             // 3 chars per channel (L and R + 1 space)
    short masterSpacing = 1;  // Extra spacing before Master column (matches grid)

    // Read peak levels for all 9 channels
    float peakLevelsL[9];
    float peakLevelsR[9];

    if (!player->IsRunning()) {
        for (int i = 0; i < 9; i++) {
            peakLevelsL[i] = peakLevelsR[i] = 0.0f;
        }
    } else {
        MixerService *ms = MixerService::GetInstance();
        
        // Channels 0-7: Read pre-volume peaks
        for (int i = 0; i < 8; i++) {
            MixBus *bus = ms->GetMixBus(i);
            if (bus) {
                uint32_t level = bus->GetPreMasterVolumePeakLevel();
                peakLevelsL[i] = (float)((level >> 16) & 0xFFFF) / 32767.0f;
                peakLevelsR[i] = (float)(level & 0xFFFF) / 32767.0f;
            } else {
                peakLevelsL[i] = peakLevelsR[i] = 0.0f;
            }
        }
        
        // Channel 8 (Master): Read post-volume peaks
        uint32_t masterLevel = ms->GetMasterPeakLevel();
        peakLevelsL[8] = (float)((masterLevel >> 16) & 0xFFFF) / 32767.0f;
        peakLevelsR[8] = (float)(masterLevel & 0xFFFF) / 32767.0f;
    }

    // Update bar heights with slew rate decay for all 9 channels using utility
    int displayHeightsL[9];
    int displayHeightsR[9];
    UpdateVuBarHeights(vuBarHeightsL_, displayHeightsL, peakLevelsL, 9);
    UpdateVuBarHeights(vuBarHeightsR_, displayHeightsR, peakLevelsR, 9);

    // Draw vertical VU bars for all 9 channels (L and R for each)
    for (int row = 0; row < VU_METER_HEIGHT; row++) {
        SetColor(GetVuBarColor(row));

        // Draw channels 0-7
        for (int i = 0; i < 8; i++) {
            GUIPoint pos = vuPos;
            pos._x += i * dx;
            pos._y -= row;

            // Left channel
            DrawVuBarRow(this, pos, row, displayHeightsL[i], vuProps, GetVuBarColor(row));

            // Right channel at x+1
            pos._x += 1;
            DrawVuBarRow(this, pos, row, displayHeightsR[i], vuProps,
                         GetVuBarColor(row));
        }

        // Draw Master channel with spacing
        GUIPoint masterPos = vuPos;
        masterPos._x += 8 * dx + masterSpacing;
        masterPos._y -= row;

        // Master left channel
        DrawVuBarRow(this, masterPos, row, displayHeightsL[8], vuProps, GetVuBarColor(row));

        // Master right channel at x+1
        masterPos._x += 1;
        DrawVuBarRow(this, masterPos, row, displayHeightsR[8], vuProps, GetVuBarColor(row));
    }

    SetColor(CD_NORMAL);
}

void MixerView::AnimationUpdate() { DrawVuBars(); }
