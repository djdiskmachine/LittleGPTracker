LittleGPTracker can be configured in a various ways through a series of flags. These flags can either be defined temporarely at the command line or permanently in a config file called 'config.xml'.

Note that values do not necessarely affect all platforms. Also, if one of the values isn't working for you it's possible that you need to get the latest [ghetto build](https://wiki.littlegptracker.com/doku.php?id=lgpt:ghetto_builds "lgpt:ghetto_builds").

##command line example

You can specify any of the options at the command line using

```
./lgpt.exe -OPTION=VALUE
```

EG.:

```
./lgpt.exe -FULLSCREEN=YES
```

This can be handy when trying out the effect of a flag before deciding to keep it permanently in the config.xml

## config.xml example

Create a new file config.xml and place it in the same folder as the platform executable you are using (ie. in windows, it must be in the bin folder with the .exe; on the gp2x it should be in the root with lgpt.gpe, etc.) Open it in a text editor or xml editor. First create a root node like this:

```
<CONFIG>
</CONFIG>
```
Then you can add as many config flags as you like:

```
<CONFIG>
  <FLAGNAME value=' '/>
  <FOO value='BAR'/>
</CONFIG>
```

## Config Values

###Screen Size

For [WIN/DEB/OSX]

To start LittleGPTracker in fullscreen:

```
<CONFIG>
  <FULLSCREEN value='YES'/>
</CONFIG>
```
To have the screen bigger than the original GP2X resolution, use SCREEMULT to set a multiplier value:

  `<SCREENMULT value='2'/>`

Be careful as large values take a lot of juice and interfere with sound playback.

## Screen Colors

LittleGPTracker uses 6 colours to do all the drawing. If you want, you can redefine them using the following parameters:

-   `BACKGROUND`: color of the background
-   `FOREGROUND`: color of the foreground
-   `BORDER`: color of the border in the start screen / dialogs
-   `HICOLOR1`: row count in song screen
-   `HICOLOR2`: cursor color
-   `PLAYCOLOR`: play indicator color
-   `MUTECOLOR`: mute indicator color
-   `SONGVIEW_FE`: color of the chain "FE" in song screen
-   `SONGVIEW_00`: color of the chain "00" in song screen
-   `BLANKSPACE`: color of the blanks in song and phrase screens
-   `ROWCOLOR1`: row count color 1
-   `ROWCOLOR2`: row count color 2
-   `ALTROWNUMBER`: How many rows for each rowcolor
-   `MAJORBEAT`: color of "--" at row 00,04,08,0c in phrase screen

All colors are defined by a set of hexadecimal triplet for RGB. Here's an example:

```
<CONFIG>
    <BACKGROUND   value = "505444" />
    <FOREGROUND   value = "FFFFFF" /> <!-- Text and cursor in cursor -->
    <BORDER       value = "FF00DD" /> <!--Dialog Border-->
    <HICOLOR1     value = "F41B38" /> <!-- Highlight color 1 -->
    <HICOLOR2     value = "FF0000" /> <!-- Highlight color 2 -->
    <SONGVIEW_FE  value = "A55B8F" /> <!-- Color of the chain "FE" in song screen-->
    <SONGVIEW_00  value = "853B6F" /> <!-- Color of the chain "00" in song screen-->
    <BLANKSPACE   value = "FFFFFF" /> <!-- Color of the blanks in song and phrase screens -->
    <CURSORCOLOR  value = "FF00DD" /> <!--Cursor color-->
    <PLAYCOLOR    value = "FF00DD" /> <!--Cursor color-->
    <MUTECOLOR    value = "FFFFFF" /> <!-- Text and cursor in cursor -->
    <ROWCOLOR1    value = "BA28F9" /> <!--Row count color 1 -->
    <ROWCOLOR2    value = "FF00FF" /> <!--Row count color 2-->
    <ALTROWNUMBER value = "4"/>       <!--How many rows of each ROWCOLOR-->
</CONFIG>
```

You can generate this config values [in an awesome web app](http://poyo.co/lgpt/ "http://poyo.co/lgpt/")

## Key and Button Mapping

The buttons (GP2X/PSP) or keys (WIN/DEB/OSX) arrangment that is predefined can be totally reconfigured through the following parameters:

-   `KEY_A`: defines mapping for A
-   `KEY_B`: defines mapping for B
-   `KEY_LEFT`: defines mapping for left arrow
-   `KEY_RIGHT`: defines mapping for right arrow
-   `KEY_UP`: defines mapping for up arrow
-   `KEY_DOWN`: defines mapping for down arrow
-   `KEY_LSHOULDER` : defines mapping for left shoulder
-   `KEY_RSHOULDER` : defines mapping for right shoulder
-   `KEY_START`: defines mapping for start

To connect the button to a keyboard key, it's pretty easy: simply put the key name as defined by the [SDL_KeySym](http://www.libsdl.org/cgi/docwiki.cgi/SDL_Keysym_definitions "http://www.libsdl.org/cgi/docwiki.cgi/SDL_Keysym_definitions") as value. For example:

```
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

to connect or redefine the button arrangment of a GP2X or a PSP, you need to define it using the following syntax:

<KEY_START value="but:0:x" />

where x is the button id of the button. The listing of button id's for PSP & GP2X can be found [here](https://wiki.littlegptracker.com/doku.php?id=lgpt:mapping#psp_key_mapping "lgpt:mapping").

Here's an example that maps the start button to the X on PSP:

```
<CONFIG>
    <KEY_START value="but:0:11" />
</CONFIG>
```

And if you want to configure a USB joypad, you'll have to use the [mapping.xml](https://github.com/djdiskmachine/LittleGPTracker/blob/master/docs/wiki/mapping_xml.md "lgpt:mapping")

## Font

### Built-in Fonts:

You can select one of the three fonts provided with this application.

Set the value of FONTTYPE in the config file to '0', '1', or '2' to apply the corresponding font.
```
<CONFIG>
    <FONTTYPE value='1' />
</CONFIG>
```

### Custom Fonts:
To use a custom font, set the value of the FONTTYPE key to 'CUSTOM'

```
<CONFIG>
    <FONTTYPE value='CUSTOM' />
</CONFIG>
```
and place the custom_font.xml file in the same directory as the application.

The custom_font.xml file is generated by converting a BMP file containing font information using a
[dedicated web tool](https://koisignal.com/toolsforlittlegptracker.html "BMP to XML")
.

The source BMP file can be obtained from the link within the web tool
or from
[here](https://raw.githubusercontent.com/djdiskmachine/LittleGPTracker/master/sources/Resources/original.bmp "original.bmp")

.

You can edit the BMP file using a general image editing software.


## Auto repeat

You can tweak the timing used to repeat the arrows keys and other.

-   KEYDELAY is the time (in msecs) you have to keep the key down before it starts repeating.
-   KEYREPEAT is the time in milliseconds between each repeat

```
<CONFIG>
    <KEYDELAY value="185"/>
    <KEYREPEAT value="40"/>
</CONFIG>
```
## Path

You can tweak two different path:

-   `ROOTFOLDER`: defines where you want the root of all your project to be. This can be useful if you don't want to follow the original installation structure.
-   `SAMPLELIB`: defines where is located the samplelibrary. For example, you may want to place the samplelibrary path at the root of your projects so you can grab samples from existing songs too. In that case, you can use an alias called "root:" representing the folder defined by ROOTFOLDER.

```
<CONFIG>
    <ROOTFOLDER value="c:/files/tracks/"/>
    <SAMPLELIB value="root:"/>
</CONFIG>
```

## Rendering

Additionally to playing the song, LittleGPTracker can be used to render the audio to file. To control file rendering, the variable RENDER can be set to either FILE,FILESPLIT,FILERT,FILESPLITRT. Note that there's a small issue with the speed when using FILE/FILESPLIT so the xxRT seem like the best choice at the moment

```
<CONFIG>
    <RENDER value='FILERT'/>
</CONFIG>
```

##Volume

This setting is for GP2X and Dingoo only. It is used to set the volume of the hardware at startup. In decimal (base 10).

```
<CONFIG>
    <VOLUME value='60'/>
</CONFIG>
```

## Audio Configuration

These settings are used to control various options of the audio configuration. They are currenly mostly used for W32 but might extend in the future to other platforms

-   `AUDIOAPI`: Allows to select the class of drivers to use. Currently can be set to ASIO to enable asio output instead of direct sound.
-   `AUDIODRIVER`: Allows to specify which driver to open. It takes the first drvier whose name matches the beginning of the string. For example, to force using a realtek soundcard instead of the default one, you can just specify "Real"
-   `AUDIOBUFFERSIZE`: Allows to tweak the default buffersize used for the audio. If the piggy glitches, increase this value.
-   `AUDIOPREBUFFERCOUNT`: Even if the computer has the ability to run the piggy full screen, some sound hardware needs nearly instant reply for the couple of first buffers. If you have upped the AUDIOBUFFERSIZE but still get glitches, try putting it back to something decent (like 512) and define AUDIOPREBUFFERCOUNT to be 2,3,... that way, a set of blank buffer will be queued, ready for the soundcard to grab, before the sequencer is actually kicked in.

```
<CONFIG>
    <AUDIODRIVER value='Real'/>
    <AUDIOBUFFERSIZE value='512'/>
</CONFIG>
```

## MIDI Configuration

This setting is also W32 only. It can be use to delay the output of midi by a certain amount of milliseconds. For example, to delay it of 1 msecs:

```
<CONFIG>
    <MIDIDELAY value='1'/>
</CONFIG>
```

also note: Because MIDI on PC is a bitch to synchronise and that the ONLY system that kind of worked was, surprisingly, the old MMSYSTEM audio system, I've re-enabled it as an option. So, if you need good midi sync or good timing, you will NEED to use this. The latency isn't great but in our case it's not a problem. To enable the old ass audio system and enjoy pretty stable midi out, add an entry in the config.xml with <AUDIOAPI value='MMSYSTEM' />

## Log Dumping

Get piggy dumping a log on the terminal or to a .log file, useful for debugging crashes or making a mapping.xml for your usb pad:

```
<CONFIG>
    <DUMPEVENT value='YES'/>
</CONFIG>
```
