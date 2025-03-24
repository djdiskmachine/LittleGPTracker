# General Installation

**These instruction are for the stable release. Currently piggy is only built for 32-bit OS's. The ghetto release only includes the binary executable so the installation procedure will be different. Also note you must have libjack installed. These instructions also work for a stock raspberry pi running [Rasbian](http://www.raspbian.org/ "http://www.raspbian.org/") or [Arch](http://archlinuxarm.org/platforms/armv6/raspberry-pi "http://archlinuxarm.org/platforms/armv6/raspberry-pi") using the appropriate PI compiled binary from the Ghetto section. It probably runs on other linux arm variants, but YRMV .**

Before beginning, check that you have installed the major dependencies:

```sh
sudo apt-get install libjack0:i386
sudo apt-get install libsdl1.2debian:i386
sudo apt-get install libasound2-plugins:i386
```

Once these have been installed, you can install:

1. Download and extract the [stable release](http://littlegptracker.com/download.php "http://littlegptracker.com/download.php").
2. Make the `lgpt.deb-exe` file in `DEB/bin/` folder executable. This can be done in most desktop environments by right clicking the file, going to properties, and then permissions. Alternatively, you can do this from the command line by using "`chmod +x lgpt.deb-exe`" inside the `DEB/bin/` folder.
3. Place all projects, your config.xml and mapping.xml (if you have them) in the `DEB/` directory. All samples go in the `DEB/samplelib/` directory unless otherwise specified in your config.xml.
4. run the `lgpt.deb-exe` in the `DEB/bin/` folder.

**The "lgpt.deb-exe" file inside DEB/bin/ is the file that MUST be executed for LGPT to locate your config files, projects, and samplelib properly. Any hard or symbolic links to this file will not work correctly. The following steps are not necessary, but may make your linux piggin' life easier.**

If you want to place an executable file somewhere besides `DEB/bin/`, (for example, inside the `DEB` folder) you can do so with the following script:

```sh
#!/bin/bash

cd [path to DEB/bin/]
./lgpt.deb-exe
```

where `[path to DEB/bin/]` is the absolute location (starting from your root directory) of DEB/bin. Example: "`cd /home/pigger/DEB/bin`"\
Save this file as `lgpt.sh` in your `DEB/bin/` directory and make it executable.

Also, if you want to be able to run lgpt from any xterm window by simply typing "lgpt", you may do the following:

```sh
sudo ln -s [path to lgpt.sh] /usr/bin/lgpt
```

Again, where `[path to lgpt.sh]` is the absolute path.

Please remember:

- if you change the name or location of the `DEB/` directory, please also change it in your `lgpt.sh` file.
- if you change the name or location of `lgpt.sh`, please also change the symbolic link in your `/usr/bin/` directory. You may use the same command as above, just with the new `[path to lgpt.sh]`.

## Tips and Tricks

{EDIT COMING SOON] Once you get piggy working, there are a number of things you can trick out your piggy installation to do:

##### Run LGPT at Boot using a boot script

##### Mount and load your projects from a USB Flash Drive to share easily with OSX/WIN

##### Use your Raspberry PI as a USB Midi to DIN Midi Converter

##### Control LGPT using your Midi device or sequencer

##### Use Piggy as your Midi Sequencer

##### Script piggy to run in render mode from a simple xterm command
