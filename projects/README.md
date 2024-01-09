##### Windows: Use Visual 2008 express edition + will need the DirectX SDK (I'm using March 2008)

##### Mac: Use Xcode 3.x + will need the SDL Framework installed

##### CAANOO: Compile under linux with the caanoo toolchain

##### GP2X: Compile under minsys

##### PSP: Compile under Ubuntu
	Required libs:
		sudo apt install -y build-essential cmake pkgconf \
		libusb-0.1 libgpgme11 libarchive-tools fakeroot
	Dev kit:
		https://github.com/djdiskmachine/pspdev
	using nano ~/.bashrc, add
		export PSPDEV=~/pspdev
		export PATH=$PATH:$PSPDEV/bin
	at the end, then 
		source ~/.bashrc
	Build with:
		make PLATFORM=PSP

##### DINGOO: Compile under Linux

##### DEB: Compile under Linux
	Now available for x64!
	Required libs:
	    sudo apt install -y git make gcc g++ libsdl1.2-dev libasound2-plugins libjack-dev

	Build with:
		make PLATFORM=DEB

##### RASPI: Compile under Linux
	Required libs:
	    sudo apt install -y git gcc libsdl1.2-dev make g++ libjack-dev
	Build with:
		make PLATFORM=RASPI

##### BITTBOY: Compile under Linux
	Required libs:
		sudo apt install -y libncurses-dev
	Dev kit:
		https://github.com/djdiskmachine/MiyooMini-toolchain/releases/download/toolchain/miyoo-toolchain-v2.0.0-arm-buildroot-linux-musleabi_sdk-buildroot.tar.gz
	    install in /opt/
	Build with:
		make PLATFORM=BITTBOY

##### MIYOOMINI: Compile under Linux
	Required libs:
		sudo apt install -y libncurses-dev
	Dev kit:
		https://github.com/djdiskmachine/miyoomini-toolchain-buildroot/releases/download/1.0.0/miyoomini-toolchain.tar.xz
	    install in /opt/
	Build with:
		make PLATFORM=MIYOO

##### RS97/RG350: Compile under Linux
	Required libs:
		sudo apt install -y libncurses-dev
	Dev kit:
		https://github.com/djdiskmachine/buildroot-rg350-old-kernel/releases/download/toolchain/mipsel-rg350-linux-uclibc_sdk-buildroot.tar.gz
	    install in /opt/ and run relocate-sdk.sh from there
	Build with:
		make PLATFORM=RS97

##### STEAMDECK: Compile in "scout" docker
	docker run -v /lgpt/repo/path/LittleGPTracker:/LGPT -it registry.gitlab.steamos.cloud/steamrt/scout/sdk/i386:latest bash
	cd LGPT/projects && make PLATFORM=STEAM
