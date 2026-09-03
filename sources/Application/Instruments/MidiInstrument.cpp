#include "MidiInstrument.h"
#include "CommandList.h"
#include "System/Console/Trace.h"
#include <string.h>
#include <cmath>

MidiService *MidiInstrument::svc_=0 ;

MidiInstrument::MidiInstrument() {

	strcpy(name_,"0") ;
	
	if (svc_==0) {
		svc_=MidiService::GetInstance() ;
	};

    for (int i = 0; i < SONG_CHANNEL_COUNT; i++)
        lastNote_[i] = new T_SimpleList<unsigned char>(true);

    Variable *v = new Variable("channel", MIP_CHANNEL, 0);
    Insert(v) ;
	v=new Variable("note length",MIP_NOTELENGTH,0) ;
	Insert(v) ;
	v=new Variable("volume",MIP_VOLUME,255) ;
	Insert(v) ;
	v=new Variable("table",MIP_TABLE,-1) ;
	Insert(v) ;
	v=new Variable("table automation",MIP_TABLEAUTO,false) ;
	Insert(v) ;
	velocity_  = 127;
}

MidiInstrument::~MidiInstrument() {
} ;

bool MidiInstrument::Init() {
	tableState_.Reset() ;
	return true ;
};

void MidiInstrument::OnStart() {
	tableState_.Reset() ;
} ;

bool MidiInstrument::Start(int c, unsigned char note, int flags) {
    // Take note of requested note regardless of whethere it is sounded.
    rootNote_[c] = note;
  
    bool muted = flags & 2;

    if (muted) {
        muted_[c] = true;
        return true;
    }

    // Not muted, thus the note should be queued.
    unsigned char *last_note = new unsigned char;
    *last_note = note;
    
    lastNote_[c]->Insert(last_note);

    first_[c] = true;
    muted_[c] = false;

    Variable *v = FindVariable(MIP_CHANNEL);
    int channel=v->GetInt() ;

    v = FindVariable(MIP_NOTELENGTH);
    remainingTicks_=v->GetInt() ;
	if (remainingTicks_==0) {
        remainingTicks_=-1 ;
    }

    //	send initial volume for this midi channel

    v = FindVariable(MIP_VOLUME);
    unsigned char volume = floor(static_cast<float>(v->GetInt() + 0.99) / 2);
    SetVolume(channel, volume);

    // store initial velocity
    velocity_ = volume;
    playing_=true ;
	retrig_=false ;

    return true;
};

void MidiInstrument::Stop(int c) {
    if (muted_[c])
        return;

    Variable *v = FindVariable(MIP_CHANNEL);
    int channel = v->GetInt();

    IteratorPtr<unsigned char> it(lastNote_[c]->GetIterator());
    for (it->Begin(); !it->IsDone(); it->Next()) {
        unsigned char note = it->CurrentItem();
        QueueNote(false, channel, note, 0);
    }

    lastNote_[c]->Empty();
    playing_ = false;
} ;

void MidiInstrument::SetChannel(int channel) {
	Variable *v=FindVariable(MIP_CHANNEL) ;
	v->SetInt(channel) ;
} ;

bool MidiInstrument::Render(int channel, fixed *buffer, int size, int flags) {
    bool mute = flags & 2;

    if (mute && !muted_[channel]) {
        Stop(channel);
	    muted_[channel]=true;
        return false;
    } else if (!mute && muted_[channel])
        muted_[channel] = false;

    // We do it here so we have the opportunity to send some command before

    Variable *v = FindVariable(MIP_CHANNEL);
    int mchannel=v->GetInt() ;

    if (first_[channel]) {

        // send note(s)
        IteratorPtr<unsigned char> it(lastNote_[channel]->GetIterator());
        for (it->Begin(); !it->IsDone(); it->Next()) {
            unsigned char note = it->CurrentItem();
            QueueNote(true, mchannel, note, velocity_);
        }

        first_[channel] = false;
    }

    if (remainingTicks_>0) {
        remainingTicks_--;
        if (remainingTicks_ == 0) {
            if (!retrig_) {
                Stop(channel);
            } else {
                remainingTicks_ = retrigLoop_;
                IteratorPtr<unsigned char> it(
                    lastNote_[channel]->GetIterator());
                for (it->Begin(); !it->IsDone(); it->Next()) {
                    unsigned char note = it->CurrentItem();
                    QueueNote(false, mchannel, note, 0);
                    QueueNote(true, mchannel, note, velocity_);
                }
            };
        };
    } ;
    return false;
};

bool MidiInstrument::IsInitialized() {
	return true ; // Always initialised
} ;

void MidiInstrument::ProcessCommand(int channel, FourCC cc, ushort value) {
    // Do not process commands if muted.
    if (muted_[channel])
        return;
  
	Variable *v=FindVariable(MIP_CHANNEL) ;
	int mchannel=v->GetInt() ;

	switch(cc) {

		case I_CMD_RTRG:
            {
				unsigned char loop=(value&0xFF) ; // number of ticks before repeat
                if (loop!=0) {
                    retrig_=true ;
                    retrigLoop_=loop ;
                    remainingTicks_=loop ;
                } else {
                    retrig_=false ;
                }
            }
			break ;
		case I_CMD_MVEL: {
			velocity_ = floor(static_cast<float>(value / 2));
		}; break;

		case I_CMD_VOLM:
			{
            unsigned char volume = floor(static_cast<float>(value / 2));
            SetVolume(mchannel, volume);
        }; break;

        case I_CMD_MCHD: {
            if (muted_[channel])
                break;

            int notes[] = {value & 0xFF, value >> 8};
            for (unsigned int i = 0; i < 2; i++) {
                unsigned char *chord_tone = new unsigned char;
                *chord_tone = (rootNote_[channel] + notes[i]) % 128;

                // Make sure note hasn't already been triggered.
                bool tone_sounded = *chord_tone == rootNote_[channel];
                IteratorPtr<unsigned char> it(
                    lastNote_[channel]->GetIterator());
                for (it->Begin(); !it->IsDone(); it->Next())
                    tone_sounded |= *chord_tone == it->CurrentItem();

                if (!tone_sounded) {
                    QueueNote(true, mchannel, *chord_tone, velocity_);
                    lastNote_[channel]->Insert(chord_tone);
                }
            }
        } break;

        case I_CMD_MDCC: {
            unsigned char id = (value & 0x7F00) >> 8;
            value &= 0x7F;
            SetCC(mchannel, id, value);
        }; break;

        case I_CMD_MDPG: {
            unsigned char id = value & 0x7F;
            SetPRG(mchannel, id);
        }; break;
        }
};

const char *MidiInstrument::GetName() {
    Variable *v = FindVariable(MIP_CHANNEL);
    sprintf(name_, "MIDI CH %2.2d", v->GetInt() + 1);
    return name_ ;
}

int MidiInstrument::GetTable() {
	Variable *v=FindVariable(MIP_TABLE) ;
	return v->GetInt() ;
} ;

bool MidiInstrument::GetTableAutomation() {
	Variable *v=FindVariable(MIP_TABLEAUTO) ;
	return v->GetBool() ;
} ;

void MidiInstrument::GetTableState(TableSaveState &state) {
	memcpy(state.hopCount_,tableState_.hopCount_,sizeof(uchar)*TABLE_STEPS*3) ;
	memcpy(state.position_,tableState_.position_,sizeof(int)*3) ;
} ;

void MidiInstrument::SetTableState(TableSaveState &state) {
	memcpy(tableState_.hopCount_,state.hopCount_,sizeof(uchar)*TABLE_STEPS*3) ;
	memcpy(tableState_.position_,state.position_,sizeof(int)*3) ;
} ;

void MidiInstrument::QueueNote(bool note_on, int channel, unsigned char note, unsigned char velocity)
{
    MidiMessage msg;
    msg.status_ = channel + (note_on ? MIDI_NOTE_ON : MIDI_NOTE_OFF);
    msg.data1_ = note;
    msg.data2_ = note_on * velocity;

    svc_->QueueMessage(msg);
}

void MidiInstrument::SetVolume(int channel, unsigned char volume) {
  SetCC(channel, 7, volume);
}

void MidiInstrument::SetCC(int channel, unsigned char id, unsigned char value) {
  MidiMessage msg;
  
  msg.status_=MIDI_CC+channel ;
	msg.data1_=id ;
	msg.data2_ = value ;
	svc_->QueueMessage(msg) ;
}

void MidiInstrument::SetPRG(int channel, unsigned char id) {
  MidiMessage msg;
  
  msg.status_=MIDI_PRG+channel ;
	msg.data1_=id ;
	msg.data2_ = MidiMessage::UNUSED_BYTE ;
	svc_->QueueMessage(msg) ;
}
