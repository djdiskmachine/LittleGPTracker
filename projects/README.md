##### Windows: Use Visual 2008 express edition + will need the DirectX SDK (I'm using March 2008)

##### Mac: Use Xcode 3.x + will need the SDL Framework installed

##### CAANOO: Compile under linux with the caanoo toolchain

##### GP2X: Compile under minsys

##### PSP: Compile under Ubuntu
	Required libs:
		sudo apt install -y build-essential cmake pkgconf \n
		libusb-0.1 libgpgme11 libarchive-tools fakeroot
	Dev kit:
		https://github.com/djdiskmachine/pspdev
	using nano ~/.bashrc, add
		export PSPDEV=~/pspdev
		export PATH=\$PATH:\$PSPDEV/bin
	at the end, then 
		source ~/.bashrc

##### DINGOO: Compile under Linux

##### RASPI: Compile under Linux
	Required libs:
	    sudo apt install -y git gcc libsdl1.2-dev make g++ libjack-dev

