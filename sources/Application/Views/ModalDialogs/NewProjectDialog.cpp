
#include "NewProjectDialog.h"
#include "Application/Utils/RandomNames.h"

static char *buttonText[BUTTONS_LENGTH] = {
	(char *)"Regen",
	(char *)"Ok",
	(char *)"Cancel"
} ;

#define DIALOG_WIDTH 20

NewProjectDialog::NewProjectDialog(View &view):ModalView(view) {}

NewProjectDialog::~NewProjectDialog() {
}

void NewProjectDialog::DrawView() {

	SetWindow(DIALOG_WIDTH,5) ;

	GUITextProperties props ;

	SetColor(CD_NORMAL) ;

    // Draw string

    int x = 0;

	char buffer[2] ;
	buffer[1]=0 ;
	for (int i=0;i<MAX_NAME_LENGTH;i++) {
		props.invert_=((i==currentChar_)&&(selected_==0)) ;
		buffer[0]=name_[i] ;
        DrawString(x + i, 2, buffer, props);
    }

    // Draw buttons

	SetColor(CD_NORMAL) ;
	props.invert_=false ;


	int offset = DIALOG_WIDTH / (BUTTONS_LENGTH + 1);

	for (int i = 0; i < BUTTONS_LENGTH; i++) {
		const char *text=buttonText[i] ;
		x = offset*(i + 1) - strlen(text) / BUTTONS_LENGTH;
		props.invert_=(selected_==i+1) ;
		DrawString(x,4,text,props) ;
    }
};

void NewProjectDialog::OnPlayerUpdate(PlayerEventType ,unsigned int currentTick) {
};

void NewProjectDialog::OnFocus() {
	selected_=currentChar_=0;
	memset(name_,' ',MAX_NAME_LENGTH+1) ;
	lastChar_ = 'A';
};

void NewProjectDialog::ProcessButtonMask(unsigned short mask,bool pressed) {

	if (!pressed) return ;

	if (mask&EPBM_B) {

	} else {

	  // A modifier
      if (mask & EPBM_A) {
          if (mask == EPBM_A) {
              std::string randomName = getRandomName();
              switch (selected_) {
              case 0:
                  if (name_[currentChar_] == ' ') {
                      name_[currentChar_] = lastChar_;
                  }
                  isDirty_ = true;
                  break;
              case 1:
                  std::fill(name_ + randomName.length(),
                            name_ + sizeof(name_) / sizeof(name_[0]), ' ');
                  strncpy(name_, randomName.c_str(), randomName.length());
                  lastChar_ = currentChar_ = randomName.length() - 1;
                  isDirty_ = true;
                  break;
              case 2:
                  EndModal(1);
                  break;
              case 3:
                  EndModal(0);
                  break;
              }
          }
        if (mask&EPBM_UP) {
			name_[currentChar_]+=1;
			lastChar_=name_[currentChar_] ;
			isDirty_=true ;
		}
		if (mask&EPBM_DOWN) {
			name_[currentChar_]-=1;
			lastChar_=name_[currentChar_] ;
			isDirty_=true ;
		}
      } else {

          // R Modifier

          if (mask & EPBM_R) {
          } else {
              // No modifier
              if (mask == EPBM_UP) {
                  selected_ = (selected_ == 0) ? 1 : 0;
                  isDirty_ = true;
              }
              if (mask == EPBM_DOWN) {
                  selected_ = (selected_ == 0) ? 1 : 0;
                  isDirty_ = true;
              }

              if (mask == EPBM_LEFT) {
                  switch (selected_) {
                  case 0:
                      if (currentChar_ > 0)
                          currentChar_--;
                      break;
                  case 1:
                  case 2:
                  case 3:
                      if (selected_ > 0)
                          selected_--;
                      break;
                  }
                  isDirty_ = true;
              }
              if (mask == EPBM_RIGHT) {
                  switch (selected_) {
                  case 0:
                      if (currentChar_ < MAX_NAME_LENGTH - 1)
                          currentChar_++;
                      else
                          selected_++;
                      break;
                  case 1:
                  case 2:
                  case 3:
                      if (selected_ < BUTTONS_LENGTH)
                          selected_++;
                      break;
                  }
                  isDirty_ = true;
              }
          }
      }
    }
};


std::string NewProjectDialog::GetName() {
    for (int i = MAX_NAME_LENGTH; i >= 0; i--) {
        if (name_[i]==' ') {
            name_[i] = 0;
        } else {
            break;
        }
    }
    std::string name = "lgpt_";
    name += name_;
	return name;
}
