##### Windows: Use Visual 2008 express edition + will need the DirectX SDK (I'm using March 2008)

##### CAANOO: Compile under linux with the caanoo toolchain

##### PSP: Compile under Ubuntu
	Required libs:
		sudo apt install -y build-essential cmake pkgconf \
		libusb-0.1 libgpgme11 libarchive-tools fakeroot python3 python3-pillow

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
	Needs to be built under 32bit architecture for now
	Required libs:
		sudo dpkg --add-architecture i386 && sudo apt update
	    sudo apt install -y git make gcc-multilib g++-multilib libsdl1.2-dev:i386 libasound2-plugins:i386 libjack-dev:i386 python3 python3-pillow

	Build with:
		make PLATFORM=DEB
		
##### X64: Compile under Linux
	Required libs:
	    sudo apt install -y git make libsdl2 libasound2-plugins libjack-dev python3 python3-pillow

	Build with:
		make PLATFORM=X64
		
##### X86: Compile under Linux
	This builds a 32bit version of the X64 SDL2 port
	Required libs:
		sudo dpkg --add-architecture i386 && sudo apt update
	    sudo apt install -y git make gcc-multilib g++-multilib libsdl2-dev:i386 libasound2-plugins:i386 libjack-dev:i386 python3 python3-pillow

	Build with:
		make PLATFORM=X86

##### RASPI: Compile under Linux
	Required libs:
	    sudo apt install -y git gcc libsdl1.2-dev make g++ libjack-dev python3 python3-pillow
	Build with:
		make

##### BITTBOY: Compile under Linux
	Required libs:
		sudo apt install -y libncurses-dev python3 python3-pillow
	Dev kit:
		https://github.com/djdiskmachine/MiyooMini-toolchain/releases/download/toolchain/miyoo-toolchain-v2.0.0-arm-buildroot-linux-musleabi_sdk-buildroot.tar.gz
	    install in /opt/
	Build with:
		make PLATFORM=BITTBOY

##### MIYOOMINI: Compile under Linux
	Required libs:
		sudo apt install -y libncurses-dev python3 python3-pillow
	Dev kit:
		https://github.com/djdiskmachine/miyoomini-toolchain-buildroot/releases/download/1.0.0/miyoomini-toolchain.tar.xz
	    install in /opt/
	Build with:
		make PLATFORM=MIYOO

##### RS97/RG350: Compile under Linux
	Required libs:
		sudo apt install -y libncurses-dev python3 python3-pillow
	Dev kit:
		https://github.com/djdiskmachine/buildroot-rg350-old-kernel/releases/download/toolchain/mipsel-rg350-linux-uclibc_sdk-buildroot.tar.gz
	    install in /opt/ and run relocate-sdk.sh from there
	Build with:
		make PLATFORM=RS97

##### GARLIC: Compile in edemirkans toolchain docker
	For older Anbernic RG35XX Devices running GarlicOS
	clone:
		https://github.com/edemirkan/rg35xx-toolchain
	enter docker:
		make shell
	Build with:
		apt install -y python3 python3-pillow
		make PLATFORM=GARLIC

##### GARLICPLUS: Compile in shauninman toolchain docker
        For newer Anbernic RG35XX Platform devices (SP, Plus, H, 2024)
        running either Garlic or muOS
        clone:
            https://github.com/shauninman/union-rg35xxplus-toolchain
        enter docker:
            make shell
        Build with:
            make PLATFORM=GARLICPLUS
        
##### RG35XXPLUS: Compile with simotek toolchain
        For newer Anbernic RG35XX Platform devices (SP, Plus, H, 2024)
        running the latest 64bit stock or stock modified firmware.
        download:
            https://github.com/simotek/rg35xx-plus-aarch64-SDL2-SDK/releases/download/0.1.0/rg35xx-plus-aarch64-SDL2-SDK-0.1.0.tar.xz
        extract:
            cd /opt
            tar -xf rg35xx-plus-aarch64-SDL2-SDK-0.1.0.tar.xz
        Build with:
            make PLATFORM=RG35XXPLUS
