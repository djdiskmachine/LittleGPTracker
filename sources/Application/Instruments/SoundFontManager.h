#ifndef _SOUND_FONT_MANAGER_H_
#define _SOUND_FONT_MANAGER_H_

#include "Foundation/T_Singleton.h"
#include "Externals/Soundfont/ENAB.H"
#include <vector>

#define MAX_SOUNDFONTS MAXLOADEDBANKS

enum SFManagerError {
    SF_BANK_TABLE_FULL = 1,
    SF_LOAD_ERROR = 2,
    SF_OPEN_ERROR = 3,
};

class SoundFontManager : public T_Singleton<SoundFontManager> {
  public:
	SoundFontManager() ;
	~SoundFontManager() ;
	void Reset() ;
	sfBankID LoadBank(const char *path) ;
private:
	std::vector<void *> sampleData_ ;
};
#endif
