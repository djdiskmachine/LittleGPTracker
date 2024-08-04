#ifndef _SOUND_FONT_MANAGER_H_
#define _SOUND_FONT_MANAGER_H_

#include <vector>
#include "Externals/Soundfont/ENAB.H"
#include "Foundation/T_Singleton.h"

class SoundFontManager:public T_Singleton<SoundFontManager> {
public:
	SoundFontManager() ;
	~SoundFontManager() ;
	void Reset() ;
	sfBankID LoadBank(const char *path) ;
private:
	std::vector<void *> sampleData_ ;
};
#endif
