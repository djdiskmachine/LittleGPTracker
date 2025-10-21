# LGPT Configurations

> *Originally seen as `config.xml`+`mapping.xml` pages from LGPT wiki years ago,*
> *reuploaded here since the wiki is down, and added some touchups:*

## **−Table of Contents**

01. [Command Line Example](#command-line-example)
02. [`config.xml` Example](#configxml-example)
03. [Config Values](#config-values)
04. [Screen Size](#screen-size)
05. [Screen Colors](#screen-colors)
06. [Key and Button Mapping](#key-and-button-mapping)
07. [Auto Repeat](#auto-repeat)
08. [Path](#path)
09. [Rendering](#rendering)
10. [Volume](#volume)
11. [Audio Configuration](#audio-configuration)
12. [MIDI Configuration](#midi-configuration)
13. [Log Dumping](#log-dumping)
----------
14. [`mapping.xml` Guide](#mappingxml-guide)
15. [Mapping a Joystick](#mapping-a-joystick)
16. [`mapping.xml` Example](#mappingxml-example)
17. [Checking Your Work](#checking-your-work)
18. [Recovering X,Y to work on GP2X.](#recovering-xy-to-work-on-gp2x)
19. [MAC OSX mapping](#mac-osx-mapping-howto)
20. [PSP key mapping](#psp-key-mapping)
21. [GP2X key mapping](#gp2x-key-mapping)
22. [Dingoo key mapping](#dingoo-key-mapping)
23. [Caanoo key mapping](#caanoo-key-mapping)
24. [Mapping a Midi Controller](#mapping-a-midi-controller)

----------


LittleGPTracker can be configured in a various ways through a series of flags. These flags can either be defined temporarely at the command line or permanently in a config file called `config.xml`.

NOTE that values do not necessarily affect all platforms. Also, if one of the values isn't working for you it's possible that you need to get the latest ~~[ghetto build](https://github.com/NinjasCL-archive/LittleGPTracker/releases)~~ [build](https://github.com/djdiskmachine/LittleGPTracker/releases/latest).

## Command Line Example

You can specify any of the options at the command line using

```
./lgpt.exe -OPTION=VALUE
```

Example:

```
./lgpt.exe -FULLSCREEN=YES
```

This can be handy when trying out the effect of a flag before deciding to keep it permanently in the `config.xml`.

## `config.xml` Example

Create a new file `config.xml` and place it in the same folder as the platform executable you are using *(ie. in Windows, it must be in the bin folder with the .exe; on the GP2X it should be in the root with lgpt.gpe, etc.)*. Open it in a text editor or xml editor. First create a root node like this:

```xml
<CONFIG>
</CONFIG>
```

Then you can add as many config flags in between as you like:

```xml
<CONFIG>
    <FLAGNAME value=' ' />
    <FOO value='BAR' />
</CONFIG>
```

## Config Values

### Screen Size

For \[**WIN**/**DEB**/**OSX**\]

To start LittleGPTracker in fullscreen:

```xml
<CONFIG>
    <FULLSCREEN value='YES' />
</CONFIG>
```

To have the screen bigger than the original GP2X resolution, use **SCREEMULT** to set a multiplier value:

```xml
<SCREENMULT value='2' />
```

Be careful as large values take a lot of juice and interfere with sound playback.

## Screen Colors

LittleGPTracker uses 6 colours to do all the drawing. If you want, you can redefine them using the following parameters:

- `BACKGROUND`: Color of the background
- `FOREGROUND`: Color of the foreground
- `BORDER`: Color of the border in the start screen / dialogs
- `HICOLOR1`: Row count in song screen
- `HICOLOR2`: Highlight color 2
- `CURSORCOLOR`: Cursor color
- `PLAYCOLOR`: Play indicator color
- `MUTECOLOR`: Mute indicator color
- `SONGVIEW_FE`: Color of the chain "FE" in song screen
- `SONGVIEW_00`: Color of the chain "00" in song screen
- `ROWCOLOR1`: Row count color 1
- `ROWCOLOR2`: Row count color 2
- `MAJORBEAT`: Color of "--" at row 00,04,08,0c in phrase screen
- `ALTROWNUMBER`: How many rows for each `ROWCOLOR`

All colors are defined by a set of hexadecimal triplet for RGB. Here's an example:

```xml
<CONFIG>
    <BACKGROUND   value = "1D0A1F" />
    <FOREGROUND   value = "F5EBFF" /> <!-- Text and cursor in cursor -->
    <BORDER       value = "FF00DD" /> <!-- Dialog Border -->
    <HICOLOR1     value = "B750D1" /> <!-- Highlight color 1 -->
    <HICOLOR2     value = "DB33DB" /> <!-- Highlight color 2 -->
    <CURSORCOLOR  value = "FF00DD" /> <!-- Cursor color -->
    <PLAYCOLOR    value = "FF00DD" /> <!-- Play indicator color -->
    <MUTECOLOR    value = "FFFFFF" /> <!-- Mute indicator color -->
    <SONGVIEW_FE  value = "A55B8F" /> <!-- Color of the chain "FE" in song screen -->
    <SONGVIEW_00  value = "853B6F" /> <!-- Color of the chain "00" in song screen -->
    <ROWCOLOR1    value = "BA28F9" /> <!-- Row count color 1 -->
    <ROWCOLOR2    value = "FF00FF" /> <!-- Row count color 2 -->
    <ALTROWNUMBER value = "4" />      <!-- How many rows of each ROWCOLOR -->
</CONFIG>
```

You can generate this config values [in an awesome web app](http://poyo.co/lgpt/ "http://poyo.co/lgpt/")

## Key and Button Mapping

The buttons (GP2X/PSP) or keys (WIN/DEB/OSX) arrangment that is predefined can be totally reconfigured through the following parameters:

- `KEY_A`: defines mapping for A
- `KEY_B`: defines mapping for B
- `KEY_LEFT`: defines mapping for left arrow
- `KEY_RIGHT`: defines mapping for right arrow
- `KEY_UP`: defines mapping for up arrow
- `KEY_DOWN`: defines mapping for down arrow
- `KEY_LSHOULDER` : defines mapping for left shoulder
- `KEY_RSHOULDER` : defines mapping for right shoulder
- `KEY_START`: defines mapping for start

To connect the button to a keyboard key, it's pretty easy: simply put the key name as defined by the ~~[SDL_KeySym](http://www.libsdl.org/cgi/docwiki.cgi/SDL_Keysym_definitions)~~ [SDL Keysym definitions](https://documentation.help/SDL/sdlkey.html) as value. For example:

```xml
<CONFIG>
    <KEY_A value="f" />
    <KEY_B value="d" />
    <KEY_LEFT value="j" />
    <KEY_RIGHT value="l" />
    <KEY_UP value="i" />
    <KEY_DOWN value="k" />
    <KEY_LSHOULDER value="a" />
    <KEY_RSHOULDER value=";" />
    <KEY_START value="space" />
</CONFIG>
```

to connect or redefine the button arrangement of a GP2X or a PSP, you need to define it using the following syntax:

```xml
<KEY_START value="but:0:x" />
```

where `x` is the `button id` of the button. The listing of button id's for **PSP** & **GP2X** can be found in [`mapping.xml` Guide](#mappingxml-guide).

Here's an example that maps the start button to the X on PSP:

```xml
<CONFIG>
    <KEY_START value="but:0:11" />
</CONFIG>
```

And if you want to configure a USB joypad, you'll have to use the [`mapping.xml`](#mappingxml-guide)

## Font

### Built-in Fonts:

You can select one of the three fonts provided with this application.

Set the value of `FONTTYPE` in the config file to '0', '1', or '2' to apply the corresponding font.

```xml
<CONFIG>
    <FONTTYPE value='1' />
</CONFIG>
```

### Custom Fonts

To use a custom font, set the value of the `FONTTYPE` key to 'CUSTOM'

```xml
<CONFIG>
    <FONTTYPE value='CUSTOM' />
</CONFIG>
```

and place the `custom_font.xml` file in the same directory as the application.

The `custom_font.xml` file is generated by converting a BMP file containing font information using a [dedicated web tool](https://koisignal.com/toolsforlittlegptracker.html "BMP to XML").

The source BMP file can be obtained from the link within the web tool
or from [here](https://raw.githubusercontent.com/djdiskmachine/LittleGPTracker/master/sources/Resources/original.bmp "original.bmp").

You can edit the BMP file using a general image editing software.

## Auto repeat

You can tweak the timing used to repeat the arrows keys and other.

- `KEYDELAY` is the time *(in msecs)* you have to keep the key down before it starts repeating.
- `KEYREPEAT` is the time in milliseconds between each repeat


```xml
<CONFIG>
    <KEYDELAY value="185" />
    <KEYREPEAT value="40" />
</CONFIG>
```

## Path

You can tweak two different path:

- `ROOTFOLDER`: defines where you want the root of all your project to be. This can be useful if you don't want to follow the original installation structure.
- `SAMPLELIB`: defines where sample library is located. For example, you may want to place the samplelibrary path at the root of your projects so you can grab samples from existing songs too. In that case, you can use an alias called “root:” representing the folder defined by `ROOTFOLDER`.


```xml
<CONFIG>
    <ROOTFOLDER value="c:/files/tracks/" />
    <SAMPLELIB value="root:" />
</CONFIG>
```

## Rendering

Additionally to playing the song, LittleGPTracker can be used to render the audio to file. To control file rendering, the variable `RENDER` can be set to either `FILE`,`FILESPLIT`,`FILERT`,`FILESPLITRT`. Note that there's a small issue with the speed when using `FILE`/`FILESPLIT` so the xxRT seem like the best choice at the moment
The xxRT options render in real time
The xSPLITx options render separate files for the channels (stems)

```xml
<CONFIG>
    <RENDER value='FILERT' />
</CONFIG>
```

## Volume

For \[**GP2X**/**Dingoo**\] only

It is used to set the volume of the hardware at startup. In decimal (base 10).

```xml
<CONFIG>
    <VOLUME value='60'/>
</CONFIG>
```

## Audio Configuration

These settings are used to control various options of the audio configuration.
They are currenly _mostly_ used for **W32** but might extend in the future to other platforms:

- `AUDIOAPI`: Allows to select the class of drivers to use. Currently can be set to ASIO to enable asio output instead of direct sound.
- `AUDIODRIVER`: Allows to specify which driver to open. It takes the first drvier whose name matches the beginning of the string. For example, to force using a realtek soundcard instead of the default one, you can just specify “Real”
- `AUDIOBUFFERSIZE`: Allows to tweak the default buffersize used for the audio. If the piggy glitches, increase this value.
- `AUDIOPREBUFFERCOUNT`: Even if the computer has the ability to run the piggy full screen, some sound hardware needs nearly instant reply for the couple of first buffers. If you have upped the `AUDIOBUFFERSIZE` but still get glitches, try putting it back to something decent (like 512) and define `AUDIOPREBUFFERCOUNT` to be 2,3,… that way, a set of blank buffer will be queued, ready for the soundcard to grab, before the sequencer is actually kicked in.


```xml
<CONFIG>
    <AUDIODRIVER value='Real' />
    <AUDIOBUFFERSIZE value='512' />
</CONFIG>
```

## MIDI Configuration

For \[**WIN**\] only

It can be use to delay the output of midi by a certain amount of milliseconds. For example, to delay it of 1 msecs:

```xml
<CONFIG>
    <MIDIDELAY value='1' />
</CONFIG>
```

also note: Because MIDI on PC is a b\*tch to synchronise and that the ONLY system that kind of worked was, surprisingly, the old MMSYSTEM audio system, I've re-enabled it as an option. So, if you need good midi sync or good timing, you will NEED to use this. The latency isn't great but in our case it's not a problem. To enable the old ass audio system and enjoy pretty stable midi out, add an entry in the `config.xml` with `<AUDIOAPI value='MMSYSTEM' />`

## Log Dumping

Get piggy dumping a log on the terminal or to a .log file, useful for debugging crashes or making a `mapping.xml` for your usb pad:

```xml
<CONFIG>
    <DUMPEVENT value='YES' />
</CONFIG>
```

----------

# `mapping.xml` Guide

I realized people are getting confused with the mapping file so I'll try to make it a bit clearer.

First of all, the mapping file is NOT a replacement for key configuration in the `config.xml` file. If you want to change the [default mapping of keys](#key-and-button-mapping), use the `config.xml`. The mapping file is intended to augment/add control ON TOP of the existing basic configuration.

## Mapping a Joystick

The idea here is to add control of the application through a joystick, making the original key combos from the config file still available.

I'm using this joystick:

![](https://web.archive.org/web/20160309150015im_/http://wiki.littlegptracker.com/lib/exe/fetch.php?media=lgpt:joy_front.jpg) ![](https://web.archive.org/web/20160309150015im_/http://wiki.littlegptracker.com/lib/exe/fetch.php?media=lgpt:joy_back.jpg)

Knobs are pretty easy do deal with on this one (since they are all numbered) but the joy axis are a little bit thougher since there's not indication for it. To get their 'ID', we are going to start the pig with a special option:

```
-DUMPEVENT=YES
```

Under Linux, specify it from the command line. Under Windows, you'll have to create a shortcut with the option on. Linux will dump the log in the terminal, while the latest version of the ghetto (as of today) creates it's log in a lgpt.log file, next to the executable.

With that option, the pig will log all event it receives, allowing identification of the various parts of the joystick. Let's first start the pig and press the 1/2/3/4 buttons. The log gives:

```
but(0):0
but(0):1
but(0):2
but(0):3
```

this means button from joystick 0 with ID 0,1,2,3 have been pressed, which is normal since the piggy uses numbers starting from zero while the joystick numbering starts at 1.

Let's press up/down on the 'flat' joystick control I'll use. The log gives:

```
joy(0)::1=-32768 <- up is negative
joy(0)::1=-257
joy(0)::1=32767 <- down is positive
joy(0)::1=-257
```

This means the axis 1 from joystick 0 has been moved, first negatively then positively(ignore the 257 stuff). So up is axis 1- and down is axis 1+. Doing the same with left/right, I'm getting left is axis 0- and right is axis 0+.

So now, we can use these information to build our mapping file. Since I'm using LSDJ type of mode (`<INVERT value=“YES” />` in the `config.xml`), I want (according to the pics)

```
1: -> A
2: -> A
3: -> B
4: -> B
```

for the shoulders, I'll map both button coz I keep pressing randomly one or the other so

```
5:->L_Shoulder
7:->L_Shoulder
6:->R_Shoulder
8:->R_Shoulder
```

and for the axis:

```
0:+ -> right
0: -> left
1:+ -> down
1:- -> up
```

## `mapping.xml` Example

So now, we create a `mapping.xml` file to be located at the same level as the lgpt\_XXX projects with

```xml
<MAPPINGS>
    <MAP src="joy:0:1-" dst="/event/up" />
    <MAP src="joy:0:1+" dst="/event/down" />
    <MAP src="joy:0:0-" dst="/event/left" />
    <MAP src="joy:0:0+" dst="/event/right" />
    <MAP src="but:0:0" dst="/event/a" />
    <MAP src="but:0:3" dst="/event/b" />
    <MAP src="but:0:1" dst="/event/a" />
    <MAP src="but:0:2" dst="/event/b" />
    <MAP src="but:0:5" dst="/event/rshoulder" />
    <MAP src="but:0:7" dst="/event/rshoulder" />
    <MAP src="but:0:4" dst="/event/lshoulder" />
    <MAP src="but:0:6" dst="/event/lshoulder" />
    <MAP src="but:0:8" dst="/event/start" />
    <MAP src="but:0:9" dst="/tempo/tap" />
</MAPPINGS>
```

## checking your work

Starting the pig again, you should see in the log the following (according you didn't mistype or misplace the config file):

```
Attached /event/up to joy:0:1-
Attached /event/down to joy:0:1+
Attached /event/left to joy:0:0-
Attached /event/right to joy:0:0+
Attached /event/a to but:0:0
Attached /event/b to but:0:3
Attached /event/a to but:0:1
Attached /event/b to but:0:2
Attached /event/rshoulder to but:0:5
Attached /event/rshoulder to but:0:7
Attached /event/lshoulder to but:0:4
Attached /event/lshoulder to but:0:6
```

And you're all set !

## Recovering X,Y to work on GP2X.

Since last builds, only A and B are mapped be default and some of you might want to recover the previous behaviour where `X` & `Y` worked like `A`/`B`. On the GP2X, the button mapping is the following:

```
X=14
Y=15
```

so to map `Y` to `B`, and `X` to `A` (regular mode), use the following `mapping.xml`:

```xml
<MAPPINGS>
    <MAP src="but:0:14" dst="/event/a" />
    <MAP src="but:0:15" dst="/event/b" />
</MAPPINGS>
```

or use another configuration that suits you :)

[original post on livejournal](http://community.livejournal.com/littlegptracker/43716.html)

## MAC OSX mapping

1) find values for usb buttons

open up the terminal

go wherever you downloaded piggy and on the command line type:
```
./LittleGPTracker.app/Contents/MacOS/LittleGPTracker -DUMPEVENT=YES
```
hit the buttons you want to use and copy the values piggy spits out

start but(0):9 lt but(0):4 rt but(0):5

up joy(0)::1=-32768 down joy(0)::1=32767 left oy(0)::1=-32768 right joy(0)::0=32767 run but(0):0 jump but(0):1

2) make a mapping file

## PSP key mapping

| button      | value        |
|-------------|--------------|
| SQUARE      | but:0:3      |
| CROSS       | but:0:2      |
| TRIANGLE    | but:0:0      |
| CIRCLE      | but:0:1      |
| START       | but:0:11     |
| SELECT      | but:0:10     |
| L_SHOULDER  | but:0:4      |
| R_SHOULDER  | but:0:5      |
| J_RIGHT     | but:0:9      |
| J_LEFT      | but:0:7      |
| J_DOWN      | but:0:6      |
| J_UP        | but:0:8      |

## GP2X key mapping

| button      | value        |
|-------------|--------------|
| A           | but:0:12     |
| B           | but:0:13     |
| START       | but:0:8      |
| SELECT      | but:0:9      |
| L_SHOULDER  | but:0:10     |
| R_SHOULDER  | but:0:11     |
| J_RIGHT     | but:0:6      |
| J_LEFT      | but:0:2      |
| J_DOWN      | but:0:4      |
| J_UP        | but:0:0      |
| J_STICK     | but:0:18     |
| X           | but:0:14     |
| Y           | but:0:15     |

## Dingoo key mapping

For those of you piggin' on the Dingoo,
you've probably realized the Dingoo doesn't have dedicated volume controls.
Below is the code for mapping the x and y buttons for volume up and down,
respectively.

```xml
<MAPPINGS>
    <MAP src="key:0:space" dst="/mixer/volume/increase" />
    <MAP src="key:0:left shift" dst="/mixer/volume/decrease" />
</MAPPINGS>
```

If you'd like to map volume to other buttons or switch around anything else,
here are is the full key mapping for the Dingoo:

| button        | value                |
|---------------|----------------------|
| X             | key:0:space          |
| Y             | key:0:left shift     |
| A             | key:0:left ctrl      |
| B             | key:0:left alt       |
| START         | key:0:return         |
| SELECT        | key:0:escape         |
| L_SHOULDER    | key:0:tab            |
| R_SHOULDER    | key:0:backspace      |
| RIGHT         | key:0:right          |
| LEFT          | key:0:left           |
| UP            | key:0:up             |
| DOWN          | key:0:down           |

## Caanoo key mapping

| button      | value        |
|-------------|--------------|
| A           | but:0:0      |
| B           | but:0:2      |
| HOME        | but:0:6      |
| HOLD        | but:0:7      |
| HELP1       | but:0:8      |
| HELP2       | but:0:9      |
| L_SHOULDER  | but:0:4      |
| R_SHOULDER  | but:0:5      |
| J_STICK     | but:0:10     |
| X           | but:0:1      |
| Y           | but:0:3      |

## Mapping a Midi Controller

> TODO CLEAN UP THIS EMAIL DUMP

This is an experimental feature that should work on WIN, OSX, NIX, and RASPPI platforms.

You can specify the device to be mapped either from the command line:

```
./lgpt.rpi-exe -MIDICTRLDEVICE=Launchpad -DUMPEVENT=YES
```

When a CTRL device is selected,
you can use your mapping file to add control as you would do with joysticks.
I've tested (rather quickly) with midi CC and and notes and it seems to work as expected.
Here's an example mapping file I used to trigger things from the lauchpad:

```xml
<MAPPINGS>
    <MAP src="midi:all:0:cc:104" dst="/event/up" />
    <MAP src="midi:all:0:cc:105" dst="/event/down" />
    <MAP src="midi:all:0:cc:106" dst="/event/left" />
    <MAP src="midi:all:0:cc:107" dst="/event/right" />
    <MAP src="midi:all:0:note:0" dst="/event/start" />
</MAPPINGS>
```

the rough description of the `src string` is

`midi`: specifies midi (obviously) :D

`all`: listen to all opened midi input. In this case you will only be able to open one but anyways, it work

`ch`: is the midi channel

`note/cc`: selects the midi message

`val`: the value that it's connected to
