
#include "SelectProjectDialog.h"
#include "NewProjectDialog.h"
#include "System/Console/Trace.h"
#include "Application/Views/ModalDialogs/MessageBox.h"

#include <algorithm>

#define LIST_SIZE 20
#define LIST_WIDTH 32

static char *buttonText[3] = {"Load", "New", "Exit"};

Path SelectProjectDialog::lastFolder_("root:") ;
int SelectProjectDialog::lastProject_ = 0 ;

static void NewProjectCallback(View &v,ModalView &dialog) {

	NewProjectDialog &npd=(NewProjectDialog &)dialog ;
	if (dialog.GetReturnCode()>0) {
		std::string selected=npd.GetName() ;
		SelectProjectDialog &spd=(SelectProjectDialog&)v ;
		Result result = spd.OnNewProject(selected) ;
        if (result.Failed()) {
            Trace::Error(result.GetDescription().c_str());
        }
    }
}

static void DeleteProjectCallback(View &v,ModalView &dialog) {
	SelectProjectDialog &spd=(SelectProjectDialog&)v ;
	if (dialog.GetReturnCode()==MBL_YES) {
		Path projectPath = spd.GetCurrentProjectPath() ;
		Result result = spd.OnDeleteProject(projectPath) ;
		if (result.Failed()) {
			Trace::Error(result.GetDescription().c_str());
		}
	}
}

// Recursive helper to delete directory and all contents
static void RecursiveDeleteDirectory(const Path &dirPath) {
	FileSystem *fs = FileSystem::GetInstance();
	FileType type = fs->GetFileType(dirPath.GetPath().c_str());
	
	if (type == FT_DIR) {
		I_Dir *dir = fs->Open(dirPath.GetPath().c_str());
		if (dir) {
			dir->GetContent("*");
			
			// Collect all items first to avoid iterator invalidation
			T_SimpleList<Path> itemsToDelete(false);
			IteratorPtr<Path> it(dir->GetIterator());
			
			for (it->Begin(); !it->IsDone(); it->Next()) {
				Path itemCopy = it->CurrentItem();
				std::string name = itemCopy.GetName();
				
				// Skip . and .. entries
				if (name != "." && name != "..") {
					Path *ptrCopy = new Path(itemCopy);
					itemsToDelete.Insert(ptrCopy);
				}
			}
			
			delete dir;
			
			// Now delete all collected items
			IteratorPtr<Path> deleteIt(itemsToDelete.GetIterator());
			for (deleteIt->Begin(); !deleteIt->IsDone(); deleteIt->Next()) {
				const Path &item = deleteIt->CurrentItem();
				RecursiveDeleteDirectory(item);
			}
		}
	}
	
	// Delete file or directory
	fs->Delete(dirPath.GetPath().c_str());
}

SelectProjectDialog::SelectProjectDialog(View &view)
    : ModalView(view), content_(true) {}

SelectProjectDialog::~SelectProjectDialog() {
}

void SelectProjectDialog::DrawView() {

	SetWindow(LIST_WIDTH,LIST_SIZE+3) ;

	GUITextProperties props ;

	SetColor(CD_NORMAL) ;
    View::EnableNotification();

    // Draw projects

    int x = 1;
    int y = 1;

    if (currentProject_ < topIndex_) {
        topIndex_ = currentProject_;
    };
    if (currentProject_>=topIndex_+LIST_SIZE) {
		topIndex_=currentProject_-LIST_SIZE+1 ;
	} ;

	IteratorPtr<Path> it(content_.GetIterator()) ;
	int count=0 ;
	char buffer[256] ;
	for(it->Begin();!it->IsDone();it->Next()) {
		if ((count>=topIndex_)&&(count<topIndex_+LIST_SIZE)) {
			Path &current=it->CurrentItem() ;
			std::string p=current.GetName() ;

			std::string firstFourChars = p.substr(0,4);
			std::transform(firstFourChars.begin(), firstFourChars.end(), firstFourChars.begin(), ::tolower);
			if(firstFourChars == "lgpt" && p.size()>4)
      {
        int namestart = 4;
        // skip _ if needed
        if ((!isalnum(p[4])) && (p.size()>4))
        {
          namestart++;
        }
				std::string t=p ;
				p=" " ;
        p+=t.substr(namestart) ;
      }
      else
      {
				std::string t=p ;
				p="[" ;
				p+=t ;
				p+="]" ;
      };

			if (count==currentProject_) {
				SetColor(CD_HILITE2) ;
				props.invert_=true ;
			} else {
				SetColor(CD_NORMAL) ;
				props.invert_=false ;
			}
			strcpy(buffer,p.c_str()) ;
			buffer[LIST_WIDTH-1]=0 ;
			DrawString(x,y,buffer,props) ;
			y+=1 ;
		}
		count++ ;
	} ;

	y=LIST_SIZE+2 ;
	int offset=LIST_WIDTH/4 ;

	SetColor(CD_NORMAL) ;

	for (int i=0;i<3;i++) {
		const char *text=buttonText[i] ;
		x=offset*(i+1)-strlen(text)/2 ;
		props.invert_=(i==selected_)?true:false ;
		DrawString(x,y,text,props) ;
    }
};

void SelectProjectDialog::OnPlayerUpdate(PlayerEventType,
                                         unsigned int currentTick) {};

void SelectProjectDialog::OnFocus() {

	setCurrentFolder(lastFolder_) ;
    currentProject_ = lastProject_;
};

void SelectProjectDialog::ProcessButtonMask(unsigned short mask,bool pressed) {
	if (!pressed) return ;

    if (mask&EPBM_B) {
        // Handle A + B combination for delete
        if (mask & EPBM_A) {
            int count = 0;
            Path *current = 0;

            IteratorPtr<Path> it(content_.GetIterator());
            for (it->Begin(); !it->IsDone(); it->Next()) {
                if (count == currentProject_) {
                    current = &it->CurrentItem();
                    break;
                }
                count++;
            }

            if (current != 0) {
                std::string message =
                    "Delete project '" + current->GetName() + "' ?";
                MessageBox *mb =
                    new MessageBox(*this, message.c_str(), MBBF_YES | MBBF_NO);
                DoModal(mb, DeleteProjectCallback);
                DrawView();
            }
            return;
        }
        if (mask & EPBM_UP)
            warpToNextProject(-LIST_SIZE);
        if (mask&EPBM_DOWN) warpToNextProject(LIST_SIZE) ;
    } else {

        // A modifier
        if (mask & EPBM_A) {
            switch (selected_) {
			case 0: // load
				{
                // locate folder user had selected when they hit a
                int count = 0;
                Path *current = 0;

                IteratorPtr<Path> it(content_.GetIterator());
                for (it->Begin(); !it->IsDone(); it->Next()) {
                    if (count == currentProject_) {
                        current = &it->CurrentItem();
                        break;
                    }
                    count++;
                }

					//check if folder is a project, indicated by 'lgpt' being the first 4 characters of the folder name
					std::string name = current->GetName() ;
					std::string firstFourChars = name.substr(0,4);
					std::transform(firstFourChars.begin(), firstFourChars.end(), firstFourChars.begin(), ::tolower);
					if(firstFourChars == "lgpt"){
						//ugly hack to make the "name" include subdirectories
						//we pass along everything past the root dir
						selection_ = *current ;
						lastFolder_=currentPath_ ;
						lastProject_=currentProject_ ;
						//load the project
						EndModal(1) ;
					} else {
						if (current->GetName() == "..") {
							Path parent=currentPath_.GetParent() ;
							setCurrentFolder(parent) ;
						} else {
							Path newdir=*current ;
							setCurrentFolder(newdir) ;
						}
					}
				break ;
			}
			case 1: // new
			{
                NewProjectDialog *npd =
                    new NewProjectDialog(*this, currentPath_);
                DoModal(npd,NewProjectCallback) ;
				break ;
            }
            case 2: // Exit ;
                EndModal(0) ;
				break ;
		}
        } else {

            // R Modifier

            if (mask & EPBM_R) {
            } else {
                // No modifier
				if (mask==EPBM_UP) warpToNextProject(-1) ;
				if (mask==EPBM_DOWN) warpToNextProject(1) ;
				if (mask==EPBM_LEFT) {
					selected_-- ;
					if (selected_<0) selected_+=3 ;
					isDirty_=true ;
				}
				if (mask==EPBM_RIGHT) {
					selected_=(selected_+1)%3 ;
					isDirty_=true ;
				}
            }
        }
    }
};

void SelectProjectDialog::warpToNextProject(int amount) {

    int offset = currentProject_ - topIndex_;
    int size = content_.Size();
    currentProject_+=amount ;
	if (currentProject_<0) currentProject_+=size ;
	if (currentProject_>=size) currentProject_-=size ;

	if ((amount>1)||(amount<-1)) {
		topIndex_=currentProject_-offset ;
		if (topIndex_<0) {
			topIndex_=0 ;
		} ;
	}
    isDirty_ = true;
}

Path SelectProjectDialog::GetSelection() {
	return selection_ ;
}

Result SelectProjectDialog::OnNewProject(std::string &name) {

    Path path = currentPath_.Descend(name);
    if (path.Exists()) {
        Trace::Log("SelectProjectDialog:OnNewProj","path already exists %s", path.GetPath().c_str());
		std::string res("Name " + name + " busy");
		View::SetNotification(res.c_str(), 0);
        return Result(res);
    }
    Trace::Log("TMP","creating project at %s",path.GetPath().c_str());
	selection_ = path ;
	Result result = FileSystem::GetInstance()->MakeDir(path.GetPath().c_str()) ;
	RETURN_IF_FAILED(result, ("Failed to create project dir for '%s", path.GetPath().c_str()));

	path = path.Descend("samples");
	Trace::Log("TMP","creating samples dir at %s",path.GetPath().c_str());
	result = FileSystem::GetInstance()->MakeDir(path.GetPath().c_str()) ;
	RETURN_IF_FAILED(result, ("Failed to create samples dir for '%s'", path.GetPath().c_str()));

	EndModal(1) ;
  return Result::NoError;
} ;

Result SelectProjectDialog::OnDeleteProject(const Path &projectPath) {

    Trace::Log("SelectProjectDialog:OnDelProj","deleting project at %s", projectPath.GetPath().c_str());
	
	// Make a non-const copy to check existence
	Path pathCopy = projectPath;
	
	// Check if project exists before deletion
    if (!pathCopy.Exists()) {
        std::string errMsg = "Project not found";
        View::SetNotification(errMsg.c_str(), 0);
        return Result("Project not found");
    }

    // Recursively delete the project directory and all contents
	RecursiveDeleteDirectory(projectPath);
	
	// Project deleted successfully, refresh the project list
	std::string successMsg = "Project deleted: " + pathCopy.GetName();
	View::SetNotification(successMsg.c_str(), 0);
	
	// Refresh current folder to update the list while preserving position
	int savedProject = currentProject_;
	int savedTopIndex = topIndex_;
	Path currentPathCopy = currentPath_;
	setCurrentFolder(currentPathCopy);
	
	// Restore position (adjust if necessary if we're at the end of list)
	int listSize = content_.Size();
	if (savedProject >= listSize && listSize > 0) {
		currentProject_ = listSize - 1;
	} else if (listSize > 0) {
		currentProject_ = savedProject;
	}
    topIndex_ = savedTopIndex;
    isDirty_ = true;
		
	return Result::NoError;
};

//copy-paste-mutilate'd from ImportSampleDialog
void SelectProjectDialog::setCurrentFolder(Path &path) {

	//get ready
	selected_=0 ;
	currentPath_=path ;
	content_.Empty() ;
	
	// Let's read all the directory in the root

	I_Dir *dir=FileSystem::GetInstance()->Open(currentPath_.GetPath().c_str()) ;

  if (dir) 
  {

		// Get all lgpt something

		dir->GetContent("*");
    dir->Sort();
      
		IteratorPtr<Path> it(dir->GetIterator()) ;
		for(it->Begin();!it->IsDone();it->Next())
    {
			Path &path=it->CurrentItem() ;

			if (path.IsDirectory())
      {
				std::string name=path.GetName() ;
				if (name[0] != '.' || name[1]== '.')
        {
					Path *p=new Path(path) ;
					content_.Insert(p) ;
				}
      }
		}
		delete (dir) ;
  }

	//reset & redraw screen
	topIndex_=0 ;
	currentProject_=0 ;
    isDirty_ = true;
}

Path SelectProjectDialog::GetCurrentProjectPath() {
	int count = 0;
	IteratorPtr<Path> it(content_.GetIterator());
	for (it->Begin(); !it->IsDone(); it->Next()) {
		if (count == currentProject_) {
			return it->CurrentItem();
		}
		count++;
	}
	return Path();
}
