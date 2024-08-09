#---------------------------------------------------------------------------------
# Clear the implicit built in rules
#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

PLATFORM := X64
FONT := original.bmp
include $(PWD)/Makefile.$(PLATFORM)

#---------------------------------------------------------------------------------
# options for FXE
#---------------------------------------------------------------------------------

AUTHOR := n0s
TITLE := lgpt
NAME := LittleGPTracker
ICON := lgpt_icon.bmp

#---------------------------------------------------------------------------------
# Directory definitions
#---------------------------------------------------------------------------------

COMMONDIRS := \
	../sources/System/Console \
	../sources/System/io \
	../sources/Services/Midi \
	../sources/Services/Time \
	../sources/Services/Controllers \
	../sources/System/FileSystem \
	../sources/System/Timer \
	../sources/System/Errors \
	../sources/UIFramework/Framework \
	../sources/UIFramework/BasicDatas \
	../sources/UIFramework/SimpleBaseClasses \
	../sources/UIFramework/Interfaces \
	../sources/Application \
	../sources/Application/Controllers \
	../sources/Application/Commands \
	../sources/Application/Audio \
	../sources/Application/Views \
	../sources/Application/Views/ModalDialogs \
	../sources/Application/Views/BaseClasses \
	../sources/Application/Model \
	../sources/Application/Utils \
	../sources/Application/Player \
	../sources/Application/Mixer \
	../sources/Application/Instruments \
	../sources/Application/Persistency \
	../sources/Foundation \
	../sources/Services/Audio \
	../sources/Foundation/Services \
	../sources/Foundation/Variables \
	../sources/Externals/Compression \
	../sources/Externals/Soundfont \
	../sources/Externals/TinyXML

#---------------------------------------------------------------------------------
# Linux
#---------------------------------------------------------------------------------

LINUXDIRS := \
	../sources/Adapters/LINUX/Main \
        ../sources/Adapters/LINUX/System \
        ../sources/Adapters/Unix/FileSystem \
	../sources/Adapters/Unix/Process \
        ../sources/System/Process
        
DBDIRS := \
	../sources/Adapters/DEB/Main \
        ../sources/Adapters/DEB/System \
        ../sources/Adapters/Unix/FileSystem \
	../sources/Adapters/Unix/Process \
        ../sources/System/Process

DUMMYMIDIDIRS := \
	../sources/Adapters/Dummy/Midi

JACKDIRS := \
	../sources/Adapters/Jack/Audio \
	../sources/Adapters/Jack/Midi \
	../sources/Adapters/Jack/Client

RTAUDIODIRS := \
	../sources/Adapters/RTAudio \
	../sources/Externals/RTAudio

RTMIDIDIRS := \
	../sources/Adapters/RTMidi \
	../sources/Externals/RtMidi

SDLDIRS := \
        ../sources/Adapters/SDL/Timer \
        ../sources/Adapters/SDL/GUI
        
SDL2DIRS := \
        ../sources/Adapters/SDL2/Timer \
        ../sources/Adapters/SDL2/GUI

SDLAUDIODIRS := \
	../sources/Adapters/SDL/Audio
        
SDL2AUDIODIRS := \
        ../sources/Adapters/SDL2/Audio

BEAGLEBOARDDIRS := $(LINUXDIRS) $(DUMMYMIDIDIRS) $(RTAUDIODIRS) $(SDLDIRS)
DEBDIRS := $(DBDIRS) $(JACKDIRS) $(RTAUDIODIRS) $(RTMIDIDIRS) $(SDLDIRS)
RASPIDIRS := $(LINUXDIRS) $(JACKDIRS) $(RTAUDIODIRS) $(RTMIDIDIRS) $(SDLDIRS)
STEAMDIRS := $(LINUXDIRS) $(JACKDIRS) $(RTAUDIODIRS) $(RTMIDIDIRS) $(SDLDIRS)
#X64DIRS := $(LINUXDIRS) $(RTMIDIDIRS) $(SDL2DIRS) $(SDL2AUDIODIRS)
X64DIRS := $(LINUXDIRS) $(DUMMYMIDIDIRS) $(SDL2DIRS) $(SDL2AUDIODIRS)
X86DIRS := $(LINUXDIRS) $(JACKDIRS) $(RTAUDIODIRS) $(RTMIDIDIRS) $(SDLDIRS)

#---------------------------------------------------------------------------------
# Consoles/Embedded
#---------------------------------------------------------------------------------

CAANOODIRS := \
	. \
	../sources/Adapters/CAANOO/Main \
	../sources/Adapters/Unix/FileSystem \
	../sources/System/Process \
	../sources/Adapters/Unix/Process \
	../sources/Adapters/CAANOO/System \
	../sources/Adapters/CAANOO/Midi \
	../sources/Adapters/CAANOO/Audio \
	../sources/Adapters/SDL/GUI \
	../sources/Adapters/SDL/Timer

DINGOODIRS := \
	. \
	../sources/Adapters/DINGOO/Main \
	../sources/Adapters/Unix/FileSystem \
	../sources/System/Process \
	../sources/Adapters/Unix/Process \
	../sources/Adapters/DINGOO/System \
	../sources/Adapters/DINGOO/Midi \
	../sources/Adapters/DINGOO/Audio \
	../sources/Adapters/SDL/Audio \
	../sources/Adapters/SDL/Process \
	../sources/Adapters/SDL/GUI \
	../sources/Adapters/SDL/Timer

RS97DIRS := $(DINGOODIRS)
BITTBOYDIRS := $(DINGOODIRS)
MIYOODIRS := $(DINGOODIRS)
PLUSHDIRS := $(MIYOODIRS)

GP2XDIRS := \
	. \
	../sources/Adapters/GP2X/Main \
	../sources/Adapters/GP2X/FileSystem \
	../sources/System/Process \
	../sources/Adapters/Unix/Process \
	../sources/Adapters/GP2X/System \
	../sources/Adapters/GP2X/Midi \
	../sources/Adapters/GP2X/Audio \
	../sources/Adapters/SDL/GUI \
	../sources/Adapters/SDL/Timer

GP32DIRS := \
	.\
	../sources/Adapters/GP32FileSystem \
	../sources/Adapters/GP32Midi \
	../sources/Adapters/GP32/Main \
	../sources/Adapters/GP32System \
	../sources/Adapters/GP32Timer \
	../sources/Adapters/GP32UI

NDSDIRS := \
	. \
	../sources/Adapters/NDS/FileSystem \
	../sources/Adapters/NDS/System \
	../sources/Adapters/NDS/Main \
	../sources/Adapters/Dummy/Midi \
	../sources/Adapters/NDS/Sound \
	../sources/Adapters/NDS/UI

PSPDIRS := \
	. \
	../sources/Adapters/PSP/Main \
	../sources/Adapters/PSP/FileSystem \
	../sources/System/Process \
	../sources/Adapters/PSP/System \
	../sources/Adapters/Dummy/Midi \
	../sources/Adapters/SDL/Process \
	../sources/Adapters/SDL/Audio \
	../sources/Adapters/SDL/GUI \
	../sources/Adapters/SDL/Timer

#---------------------------------------------------------------------------------
# Windows
#---------------------------------------------------------------------------------

W32DIRS := \
	.\
	../sources/Adapters/W32FileSystem \
	../sources/Adapters/W32Midi \
	../sources/Adapters/W32Sound \
	../sources/Adapters/WSDLSystem \
	../sources/Adapters/WSDLUI \
	../sources/Adapters/SDL/Input \
	../sources/Adapters/W32/Main \
	../sources/Adapters/W32/Process \
	../sources/Adapters/W32/Audio \
	../sources/System/Process

#---------------------------------------------------------------------------------
# files definition
#---------------------------------------------------------------------------------

COMMONFILES := \
	Logger.o Trace.o Result.o Status.o Config.o \
	Timer.o FileSystem.o \
	SysMutex.o TimeService.o \
	MidiOutDevice.o MidiInDevice.o MidiService.o Groove.o \
	MidiChannel.o MidiInMerger.o \
	GUIEvent.o GUIRect.o \
	EventManager.o GUIWindow.o \
	Channel.o Mixer.o \
	ControlRoom.o \
	MultiChannelAdapter.o \
	CommandDispatcher.o \
	ApplicationCommandDispatcher.o \
	EventDispatcher.o \
	ControllerService.o \
	ButtonControllerSource.o \
	KeyboardControllerSource.o \
	JoystickControllerSource.o \
	HatControllerSource.o \
	ControllerSource.o \
	ControlNode.o \
	I_GUIWindowImp.o \
	Application.o AppWindow.o SelectProjectDialog.o \
	NewProjectDialog.o \
	ImportSampleDialog.o \
	MixerView.o NullView.o \
	SongView.o ChainView.o PhraseView.o ViewData.o ConsoleView.o ProjectView.o InstrumentView.o UIController.o \
	View.o ModalView.o FieldView.o UIField.o UIIntField.o \
	UIIntVarOffField.o UIIntVarField.o ViewEvent.o I_Action.o\
	UITempoField.o UIActionField.o \
	MessageBox.o \
	GrooveView.o UINoteVarField.o UIBigHexVarField.o \
	SRPUpdaters.o UIStaticField.o \
	Song.o Chain.o Phrase.o Project.o \
	char.o n_assert.o fixed.o wildcard.o \
	SyncMaster.o TablePlayback.o Player.o \
	Table.o TableView.o\
	InstrumentBank.o WavFileWriter.o WavFile.o MidiInstrument.o Filters.o SampleVariable.o SampleInstrument.o SamplePool.o CommandList.o \
	PersistencyService.o Persistent.o \
	Observable.o SingletonRegistry.o \
	Audio.o AudioMixer.o AudioOutDriver.o AudioDriver.o \
	AudioOut.o \
	DummyAudioOut.o PlayerChannel.o AudioFileStreamer.o \
	MixBus.o \
	MixerService.o PlayerMixer.o \
	Service.o ServiceRegistry.o SubService.o \
	Variable.o VariableContainer.o WatchedVariable.o \
	SoundFontPreset.o SoundFontManager.o SoundFontSample.o \
	ENAB.o HYDRA.o OMEGA.o RIFF.o SFDETECT.o SFLOOKUP.o SFNAV.o SFREADER.o WIN_MEM.o \
	HexBuffers.o lz.o \
	tinyxmlparser.o tinyxml.o tinyxmlerror.o tinystr.o Tiny2NosStub.o

#---------------------------------------------------------------------------------
# Linux
#---------------------------------------------------------------------------------

LINUXFILES := \
	UnixFileSystem.o \
	LINUXMain.o \
	LINUXSystem.o \
	GUIFactory.o \
	SDLGUIWindowImp.o \
	SDLEventManager.o \
	Process.o \
	UnixProcess.o \
	SDLTimer.o
	
DBFILES := \
	UnixFileSystem.o \
	DEBmain.o \
	DEBSystem.o \
	GUIFactory.o \
	SDLGUIWindowImp.o \
	SDLEventManager.o \
	Process.o \
	UnixProcess.o \
	SDLTimer.o

DUMMYMIDIFILES := \
	DummyMidi.o

RTAUDIOFILES := \
	RTAudioDriver.o \
	RTAudioStub.o \
	RtAudio.o

RTMIDIFILES := \
	RtMidi.o \
	RTMidiService.o \
	RTMidiOutDevice.o \
	RTMidiInDevice.o

JACKFILES := \
	JackAudioDriver.o \
	JackClient.o \
	JackAudio.o

SDLAUDIOFILES := \
	SDLAudioDriver.o \
	SDLAudio.o

DEBFILES := $(DBFILES) $(RTAUDIOFILES) $(RTMIDIFILES) $(JACKFILES)
BEAGLEBOARDFILES := $(LINUXFILES) $(DUMMYMIDIFILES)
RASPIFILES := $(LINUXFILES) $(RTAUDIOFILES) $(RTMIDIFILES) $(JACKFILES)
STEAMFILES := $(LINUXFILES) $(RTAUDIOFILES) $(RTMIDIFILES) $(JACKFILES)
#X64FILES := $(LINUXFILES) $(RTMIDIFILES) $(SDLAUDIOFILES)
X64FILES := $(LINUXFILES) $(SDLAUDIOFILES) $(DUMMYMIDIFILES)
X86FILES := $(LINUXFILES) $(RTAUDIOFILES) $(RTMIDIFILES) $(JACKFILES)

#---------------------------------------------------------------------------------
# Consoles/Embedded
#---------------------------------------------------------------------------------

CAANOOFILES := \
	CAANOOSDLMain.o \
	UnixFileSystem.o \
	CAANOOMidiService.o \
	CAANOOSerialMidiDevice.o \
	CAANOOAudio.o \
	Process.o \
	UnixProcess.o \
	CAANOOAudioDriver.o \
	CAANOOSystem.o \
	CAANOOEventQueue.o \
	gpmemcpy.o \
	gpmemset.o \
	SDLTimer.o \
	GUIFactory.o \
	SDLEventManager.o \
	SDLGUIWindowImp.o

GP2XFILES := \
	GPSDLMain.o \
	GP2XFileSystem.o \
	GP2XMidiService.o \
	GP2XSerialMidiDevice.o \
	GP2XAudio.o \
	Process.o \
	UnixProcess.o \
	GP2XAudioDriver.o \
	GP2XSystem.o \
	GP2XEventQueue.o \
	gpmemcpy.o \
	gpmemset.o \
	SDLTimer.o \
	GUIFactory.o \
	SDLEventManager.o \
	SDLGUIWindowImp.o

DINGUXFILES := \
	DINGOOEventQueue.o \
	GPSDLMain.o \
	GUIFactory.o \
	Process.o \
	SDLAudioDriver.o \
	SDLEventManager.o \
	SDLGUIWindowImp.o \
	SDLProcess.o \
	SDLTimer.o \
	UnixFileSystem.o \
	UnixProcess.o

DINGOOFILES := \
	$(DINGUXFILES) \
	DummyMidi.o \
	DINGOOAudio.o \
	DINGOOAudioDriver.o \
	DINGOOSystem.o

RS97FILES := \
	$(DINGUXFILES) \
	DINGOOAudio.o \
	DINGOOAudioDriver.o \
	BITTBOYSystem.o \
	BITTBOYSerialMidiDevice.o \
	BITTBOYMidiService.o

BITTBOYFILES := \
	$(DINGUXFILES) \
	DINGOOAudio.o \
	DINGOOAudioDriver.o \
	BITTBOYSystem.o \
	BITTBOYSerialMidiDevice.o \
	BITTBOYMidiService.o

MIYOOFILES := \
	$(DINGUXFILES) \
	DummyMidi.o \
	MIYOOSystem.o \
	SDLAudio.o

PLUSHFILES = $(MIYOOFILES)

NDSFILES := \
	NDSmain.o \
	NDSFileSystem.o \
	DummyMidi.o \
	NDSSound.o \
	NDSSystem.o \
	GUIFactory.o \
	NDSGUIWindowImp.o

PSPFILES := \
	PSPmain.o \
	PSPFileSystem.o \
	Process.o \
	DummyMidi.o \
	GUIFactory.o \
	SDLGUIWindowImp.o \
	SDLEventManager.o \
	SDLAudioDriver.o \
	SDLAudio.o \
	SDLProcess.o \
	SDLTimer.o \
	PSPSystem.o

#---------------------------------------------------------------------------------
# Windows
#---------------------------------------------------------------------------------

W32FILES := \
	WSDLmain.o \
	W32FileSystem.o \
	W32Midi.o \
	W32AudioDriver.o \
	WSDLSystem.o \
	SDLInput.o \
	Process.o \
	W32Process.o \
	W32Audio.o \
	GUIFactory.o \
	WSDLGUIWindowImp.o

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
#---------------------------------------------------------------------------------

TARGET := $(TITLE)
BUILD := build$(PLATFORM)
SOURCES := $($(PLATFORM)DIRS) $(COMMONDIRS)
INCLUDES :=

#---------------------------------------------------------------------------------
# path to tools - this can be deleted if you set the path in windows
#---------------------------------------------------------------------------------

export PATH := $(DEVKITARM)/bin:$(PATH)

#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project
#---------------------------------------------------------------------------------

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------

#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------

export OUTPUT := $(CURDIR)/$(TARGET)
export VPATH := $(foreach dir,$(SOURCES),$(CURDIR)/$(dir))
export DEPSDIR := $(CURDIR)/$(BUILD)

#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------

export LD := $(CXX)
export OFILES := $(COMMONFILES) $($(PLATFORM)FILES) 
export OBJS := $(OFILES)
export INCLUDE := \
	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
	$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
	-I$(CURDIR)/$(BUILD)
export LIBPATHS := $(foreach dir,$(LIBDIRS),-L$(dir)/lib)

.PHONY: $(BUILD) clean

#---------------------------------------------------------------------------------

$(BUILD):
	python3 ../sources/Resources/mkfont.py ../sources/Resources/$(FONT) ../sources/Resources/font.h
	@[ -d $@ ] || mkdir -p $@
	@make --no-print-directory -C $(BUILD) -f $(PWD)/Makefile

#---------------------------------------------------------------------------------

clean:
	@echo clean ...
	@rm -fr $(OUTPUT).$(EXTENSION) $(BUILD) *.elf *.gxb *.fxe *.gdb

#---------------------------------------------------------------------------------

else

DEPENDS := $(OFILES:.o=.d)

ifeq ($(PLATFORM),PSP)
include $(PSPSDK)/lib/build.mak
endif

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------

all: $(OUTPUT).$(EXTENSION)

$(OUTPUT).exe : $(OFILES)
	@echo "linking result.."
	@$(LD) $(LDFLAGS) -o $@ $(OFILES) $(LIBS)

$(OUTPUT).fxe : $(OUTPUT).gxb

$(OUTPUT).gxb : $(OUTPUT).elf

$(OUTPUT).elf : $(OFILES)

$(OUTPUT).deb-exe : $(OFILES)
	$(LD) $(LDFLAGS) -o $@ $(OFILES) $(LIBS)

$(OUTPUT).beagle-exe : $(OFILES)
	$(LD) $(LDFLAGS) -o $@ $(OFILES) $(LIBS)

-include $(DEPENDS)

#---------------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------------

# Only build non-automated configs
deploy:
#	make PLATFORM=PSP
#	make PLATFORM=DEB
#	make PLATFORM=MIYOO
	./resources/packaging/build_and_copy_from_pi.sh
	./resources/packaging/build_and_copy_from_chip.sh
#	make PLATFORM=BITTBOY
#	make PLATFORM=RS97
#	make PLATFORM=RG35XX
	./resources/packaging/lgpt_package.sh
