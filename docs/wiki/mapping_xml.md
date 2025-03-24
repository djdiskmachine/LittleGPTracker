I realized people are getting confused with the mapping file so I'll try to make it a bit clearer.

First of all, the mapping file is NOT a replacement for key configuration in the config.xml file. If you want to change the default mapping of keys, use the config.xml (see the wiki for that). The mapping file is intended to augment/add control ON TOP of the existing basic configuration.

# Mapping a Joystick

The idea here is to add control of the application through a joystick, making the original key combos from the config file still available.

I'm using this joystick:

![](https://web.archive.org/web/20160309150015im_/http://wiki.littlegptracker.com/lib/exe/fetch.php?media=lgpt:joy_front.jpg) ![](https://web.archive.org/web/20160309150015im_/http://wiki.littlegptracker.com/lib/exe/fetch.php?media=lgpt:joy_back.jpg)

Knobs are pretty easy do deal with on this one (since they are all numbered) but the joy axis are a little bit thougher since there's not indication for it. To get their 'ID', we are going to start the pig with a special option:

```
-DUMPEVENT=YES
```

Under linux, specify it from the command line, under windows, you'll have to create a shortcut with the option on. Linux will dump the log in the terminal, while the latest version of the ghetto (as of today) creates it's log in a lgpt.log file, next to the executable.

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

This means the axis 1 from joystick 0 has been moved, first negatively then positively (ignore the 257 stuff). So up is axis 1- and down is axis 1+. Doing the same with left/right, I'm getting left is axis 0- and right is axis 0+

So now, we can use these information to build our mapping file. Since I'm using lsdj type of mode (`<INVERT value="YES" />` in the config.xml), I want (according to the pics)

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

and for the axis

```
0:+ -> right
0: -> left
1:+ -> down
1:- -> up
```

## mapping.xml Example

So now, we create a 'mapping.xml' file to be located at the same level as the lgpt_XXX projects with

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

## recovering X,Y to work on GP2X.

Since last builds, only A and B are mapped be default and some of you might want to recover the previous behaviour where X & Y worked like A/B. On the GP2X, the button mapping is the following:

```
X=14
Y=15
```

so to map Y to B and X to A (regular mode), use the following mapping.xml

```xml
<MAPPINGS>
<MAP src="but:0:14" dst="/event/a" />
<MAP src="but:0:15" dst="/event/b" />
</MAPPINGS>
```

or use another configuration that suits you :)

[original post on livejournal](http://community.livejournal.com/littlegptracker/43716.html "http://community.livejournal.com/littlegptracker/43716.html")

## mac osx mapping howto

1) find values for usb buttons

open up the terminal

go wherever you downloaded piggy and on the command line type:

./LittleGPTracker.app/Contents/MacOS/LittleGPTracker -DUMPEVENT=YES

hit the buttons you want to use and copy the values piggy spits out

start but(0):9 lt but(0):4 rt but(0):5

up joy(0)::1=-32768 down joy(0)::1=32767 left oy(0)::1=-32768 right joy(0)::0=32767 run but(0):0 jump but(0):1

2) make a mapping file

## psp key mapping

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

## gp2x key mapping

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


## dingoo key mapping


For those of you piggin' on the Dingoo, you've probably realized the Dingoo doesn't have dedicated volume controls. Below is the code for mapping the x and y buttons for volume up and down, respectively.

```xml
<MAPPINGS>
<MAP src="key:0:space" dst="/mixer/volume/increase" />
<MAP src="key:0:left shift" dst="/mixer/volume/decrease" />
</MAPPINGS>
```

If you'd like to map volume to other buttons or switch around anything else, here are is the full key mapping for the Dingoo:


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


## caanoo key mapping


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

TODO CLEAN UP THIS EMAIL DUMP

This is an experimental feature that should work on WIN, OSX, NIX, and RASPPI platforms.

You can specify the device to be mapped either from the command line:

```
./lgpt.rpi-exe -MIDICTRLDEVICE=Launchpad -DUMPEVENT=YES
```

When a CTRL device is selected, you can use your mapping file to add control as you would do with joysticks. I've tested (rather quickly) with midi CC and and notes and it seems to work as expected. Here's an example mapping file I used to trigger things from the lauchpad

```xml
<MAPPINGS>
<MAP src="midi:all:0:cc:104" dst="/event/up" />
<MAP src="midi:all:0:cc:105" dst="/event/down" />
<MAP src="midi:all:0:cc:106" dst="/event/left" />
<MAP src="midi:all:0:cc:107" dst="/event/right" />
<MAP src="midi:all:0:note:0" dst="/event/start" />
</MAPPINGS>
```

the rough description of the src string is

midi: specifies midi (obviously) :D

all: listen to all opened midi input. In this case you will only be able to open one but anyways, it work

ch: is the midi channel

note/cc: selects the midi message

val: the value that it's connected to
