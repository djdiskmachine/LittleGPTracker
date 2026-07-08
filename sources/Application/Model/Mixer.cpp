
#include "Mixer.h"
#include "Application/Utils/HexBuffers.h"

Mixer::Mixer():Persistent("MIXER")  {
	Clear() ;
} ;

Mixer::~Mixer() {
} ;

void Mixer::Clear() {

	for (int i=0;i<SONG_CHANNEL_COUNT;i++) {
		channelBus_[i]=i ;
        channelVolume_[i] = 0xFF;
        channelHPF_[i] = 0; // 0=OFF, 1=20Hz, 2=90Hz
    }
} ;

void Mixer::SaveContent(TiXmlNode *node) {
    saveHexBuffer(node, "BUS", (unsigned char *)channelBus_,
                  SONG_CHANNEL_COUNT);
    saveHexBuffer(node, "VOL", channelVolume_, SONG_CHANNEL_COUNT);
    saveHexBuffer(node, "HPF", channelHPF_, SONG_CHANNEL_COUNT);
} ;

void Mixer::RestoreContent(TiXmlElement *element) {
    TiXmlElement *current = element->FirstChildElement();
    while (current) {
        const char *value = current->Value();
        if (!strcmp("BUS", value)) {
            restoreHexBuffer(current, (unsigned char *)channelBus_);
        } else if (!strcmp("VOL", value)) {
            restoreHexBuffer(current, channelVolume_);
        } else if (!strcmp("HPF", value)) {
            restoreHexBuffer(current, channelHPF_);
        }
        current = current->NextSiblingElement();
    }
}
