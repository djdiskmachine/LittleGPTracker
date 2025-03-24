# Quick-Start Guide

## preamble

this quick start guide is intended to be a gentle introduction to LittleGPTracker (lgpt, piggy, the pig, our precious oinker, etc). this guide should be useful if you're running piggy on any platform.

piggy is a [sample tracker](http://en.wikipedia.org/wiki/Tracker) with an interface and controls specifically designed for a hand held game console. along the way there have been many bows to [LSDj](http://www.littlesounddj.com) so if your comfortable with that gameboy tracker, piggy will fit like an old hat.

piggy is under active development with bugs popping up, bugs being squashed, and features being implemented. that doesn't mean piggy isn't ready for you to use, just be aware crashes could happen and report them so they can be eliminated. it's also a good idea to **SAVE & BACK UP YOUR WORK FREQUENTLY** so that if you do experience a crash, you don't lose much data.

also, what you see in this guide may not perfectly match the looks of the piggy you downloaded. however, the text of this guide is updated frequently enough to match current release functionality. so lets get down and dirty with the pig and see what you come up with!

## installation

first grab & extract the zip file for your platform from the [official homepage](https://www.littlegptracker.com/download.php). from there your action will vary by platform.

* **windows:** put the install folder where ever you want to keep it. make a convinient short cut to folder if you like, so that you can edit the samples in your projects and easily find the executable (\\install\\bin\\lgpt.exe).
* **gp32:** copy the lgpt folder and lgpt.fxe into the GPMM directory on your smart media card (SMC).
* **gp2x, caanoo:** copy the lgpt folder and lgpt.gpe to your secure digital card (SD). they can be anywhere (root, a directory, etc) but they both have to be there.
* **psp:** homebrew enable your psp, place the install folder in /GAME


## configuration

there's not much to do w/ the config file for now unless you are running piggy on a gp2x after playing with [LSDj](http://www.littlesounddj.com) … the A/Y/X/B buttons will feel awkward. you can flip the buttons to feel like lsdj in the config file, but you'll have to remember each reference to the A button in this guide is actually to Y/B (and each reference to B is actually to X/A)!

the config file is an [xml](http://en.wikipedia.org/wiki/XML) document which can be edited like any text file. if it doesn't exist piggy has safe default values. however, if you want to use a config.xml file and didn't find one in the install.rar, grab the examples from the [reference manual](What-is-LittlePiggyTracker.md) and save it in the same directory as lgpt.gpe.

## loading samples

before you load any [samples](http://en.wikipedia.org/wiki/Sampling_(music)) they must be in the correct format. piggy loads 8 or 16bit .wav files, stereo or mono, and any sampling rate. there are 7F (128) instrument slots. if you're running on gp32 or gp2x the total size of your samples needs to be less than the RAM limitation of the machine.

unlike traditional trackers, piggy doesn't store sample data in the module file. instead, samples are stored in a directory and loaded each time piggy is run. references to the samples are saved with the project as instruments, and if piggy finds any new .wav files when it is run they will be assigned to free instruments.

regardless of the platform you're running on, the .wav files belong in **\\lgpt\\samples**.

so, pick out some wavs and copy them to that directory. if you want to grab some samples guaranteed to load in piggy, [here's a 500kb kit](http://mp3death.com/piggy/lgpt16bitQuickStartKitByStarpauseAndPepper.zip) including tr909 drum sounds, an acid bass, piano, and some vocal samples from [pepper](https://web.archive.org/web/20190513041654/http://www.myspace.com/p3pper "http://www.myspace.com/p3pper")).

## launching piggy

![](https://web.archive.org/web/20190513041654im_/http://wiki.littlegptracker.com/lib/exe/fetch.php?media=quick:start01_1.1f.png)

ah yes, the dashes! if you're not familiar with the [hexadecimal numbering system](http://en.wikipedia.org/wiki/Hexadecimal) now would be a good time to read up on it (since everything in piggy is counted in hex)!

for those who are coming from a traditional tracker background, you are about to experience a paradigm shift. typically you have an order list which describes the song and is made up of patterns. each pattern contains playback data for several tracks or channels. the channels are visible as columns only when editing a pattern, so it appears the channels are in each pattern.

piggy replaces the order list with a more powerful Song screen, making the channels visible from the top. each channel is still a monophonic sample playback track, only now it is possible to specify what each channel is playing back at a higher level. to make this higher level of control possible patterns also had to evolve.

the traditional pattern (which would store the playback data for all the channels joined together) has been replaced with several Chains (which contain playback data for a single channel). these Chains or sub-patterns can be re-used on any row (position in time), or even any channel.

this “sub-pattern” style has been seen before in [soundtracker 2.6](http://www.pouet.net/prod.php?which=13607), icetracker 1.1, [jeskola buzz](http://en.wikipedia.org/wiki/Jeskola_Buzz), [goattracker](https://sourceforge.net/projects/goattracker2/), and LSDj.

in summary, on piggy's Song screen we see a grid where each column represents a channel and each row represents a position in time. Each square on the grid (initially shown as tow dashes) represents a slot for a Chain.

the green cursor can be moved around with the joystick. it shows where you are, both on the grid and to the left where the channel numbers are indicated. when piggy is playing, these channel indicators will display the notes of the instruments triggering on each channel.

the red numbers to the left of each row indicate a position in the song. like a traditional tracker, time is aligned on the veritcle axis in piggy.

## create a chain, navigate

![](https://web.archive.org/web/20190513041654im_/http://wiki.littlegptracker.com/lib/exe/fetch.php?media=quick:start02_1.1f.png)

time for action! but first a few conventions: key presses will be `monospaced` for easier reading. a `,` between button pushes means to push the button twice (`B,B` would be tap `B` twice). a `+` between buttons means to hold the first button and then hit the second button (`L+B,B,B` would be hold the `L shoulder` and tap `B` three times). also, if you're following along in windows you may want to check on the [control moves list](What-is-LittlePiggyTracker.md#controls--moves) for pc keyboard equivalents of the buttons.

for reals: time for action! hit `A,A` (double tap the A button that is) to create your first Chain. a hex number will pop up to identify creation. it is only possible to have FF (256) Chains in a piggy project.

the Chain you create can be on any row or any column–later, after you've learned to put phrases into the chain, when you hit `start` a play arrow will appear and piggy will simultaneously play all the Chains on the current row (the one your cursor is on). right now you can expect silence after you hit `start`. to get some sound out we'll need to navigate to the Phrase and lay down some wavs.

navigation in piggy is done by holding the right shoulder (`R`) down and navigating a map of screens (think zelda) with the joystick. your current position is displayed in the upper left hand corner–in the screen capture above, the Song screen. here's map of the screens:

```
   _-------_
  |         |       o888                          o8
  |         |        888    oooooooo8 ooooooooo  o888oo
  |         |        888  888    88o   888    888 888
   project--         888   888oo888o   888    888 888
                    o888o 888     888  888ooo88    888o
       ^           ________888ooo888  o888 *screenmap by Sm0hm*
       v          | groove ^|
   _-------_       _-------v       _-------_       _-------_
  |         |     |         |     |         |     |         |
  |         | <-> |         | <-> |         | <-> |         |
  |         |     |         |     |         |     |         |
   song-----       chain----       phrase---       instrument
                                       ^               ^
                                       v               v
                                   _-------_       _-------_
                                  |         |     |         |
                                  |         | <-> |         |
                                  |         |     |         |
                                   table----       table----

```


## edit a chain, create a phrase

![](https://web.archive.org/web/20190513041654im_/http://wiki.littlegptracker.com/lib/exe/fetch.php?media=quick:start03_1.1f.png)

position the cursor over the Chain you created and hit `R`+`joystick:right`. this will work to edit any Chain you create–when you hit `R`+`joystick:right` with your cursor on any number in the song screen, you will be editing that Chain. the changes you make here will apply to all the instances of a Chain which are laid out on the Song screen.

![](https://web.archive.org/web/20190513041654im_/http://wiki.littlegptracker.com/lib/exe/fetch.php?media=quick:start04_1.1f.png)

the Chain screen presents you with more dashes. these blank spaces are to be filled up with Phrases. to create a new Phrase hit `A,A`. the Phrases you create can be used multiple times in the same Chain or even used in different Chains. like Chains, there is a single pool of Phrases for the piggy project, and you are limited to creating FF (256) of them.

if you hit `start` now, there will still be silence. piggy will be playing the Phrase at Chain position 00 in a loop.

edit a phrase
-------------

![](https://web.archive.org/web/20190513041654im_/http://wiki.littlegptracker.com/lib/exe/fetch.php?media=quick:start05_1.1f.png)

the fun begins! if you haven't hit `start` yet you may as well now so you can hear what is happening. it's also fun to watch the play arrow run through the 16 steps that make up each Phrase :)

to lay down a note trigger hit `A` with the cursor in the left most column. if you want to remove a note trigger hit `B+A`. more generally, if you want to cut out a note, Phrase, or Chain position your cursor over it and hit `B+A`. to paste what you have cut at the current cursor location, hold the left shoulder button and hit A, aka `L+A`. these are universal key presses universal key press in piggy.

to change the [pitch](https://en.wikipedia.org/wiki/Pitch_(music)) of the sample playback press `A+joystick`. up and down on the joystick will change the octave, left and right will change half-steps.

![](https://web.archive.org/web/20190513041654im_/http://wiki.littlegptracker.com/lib/exe/fetch.php?media=quick:start06_1.1f.png)

likewise, if you want to change the instrument that is being triggered on a row/step, position the cursor over the instrument number (as above) and hit `A+joystick`. again, while you have `A` held, left and right on the joystick will edit the least significant bit and up and down will edit the most significant bit. this key combo will also work to edit Phrases in the Chain screen and Chains in the song screen!

notice the instrument names at the top of the screen as you change the instrument under your cursor. these instruments were created by piggy when it found new .wav in the sample directory at boot time. the screen for editing instruments is accessed the same as Chains and Phrases: hold `R+joystick:right` with the cursor. if you feel like dicking around with values here feel free but i'll leave an explanation of each for a later guide. hold `R+joystick:left` in the Instrument screen to return to the Phrase edit screen.

## execute a command

![](https://web.archive.org/web/20190513041654im_/http://wiki.littlegptracker.com/lib/exe/fetch.php?media=quick:start07_1.1f.png)

there are four columns on the right are for triggering commands. each pair of columns is for specifying a command and the parameters for it's execution.

commands are placed (press `A`) and removed (`B+A`) much the same as notes. some commands work best when used on the same row as a sample trigger, some show their true colors when triggered on a row of their own or on a row with another command. it's up to you to have fun and see what will come out, but technical descriptions of all the commands is available in the [reference manual](What-is-LittlePiggyTracker.md).

changing the parameters for commands is different. `A+joystick:left/right` will move the parameter editing cursor back and forth over the four digits. `A+joystick:up/down` will change the value for the digits. the four parameter digits effect each command differently, again i must refer you to the [reference manual](What-is-LittlePiggyTracker.md) if you are interested in details.

## selections

![](https://web.archive.org/web/20190513041654im_/http://wiki.littlegptracker.com/lib/exe/fetch.php?media=quick:start09_1.1f.png)

make a selection, hit `L+B` once. you can then change the selection with the `joystick`. once a selection is made you can tap `B` to copy it or `L+A` to cut. be careful when pasting large selections, it's easy to overwrite data and there is no undo!

![](https://web.archive.org/web/20190513041654im_/http://wiki.littlegptracker.com/lib/exe/fetch.php?media=quick:start08_1.1f.png)

if you have a row full of note, instrument, and command data which you want to copy, it's quick and easy to select the whole thing. hold the `L shoulder` button and tap `B` tapping `B` again will select the entire screen. after the selection is made and you let go of the `L shoulder` button, you can copy/cut as with any other selection.

## eight channels of sound

![](https://web.archive.org/web/20190513041654im_/http://wiki.littlegptracker.com/lib/exe/fetch.php?media=quick:start12_1.1f.png)

when you want to play more than one sound at once you simply add Chains to the other channels on the row where you're pressing `start`. any Chains that don't have a Phrase in them will be silent and you won't see a play arrow on them. if you're writing a song and want a channel to be silent, create a dummy Chain with no sample triggers in it.

![](https://web.archive.org/web/20190513041654im_/http://wiki.littlegptracker.com/lib/exe/fetch.php?media=quick:start10_1.1f.png)

if you want to play one Chain after another, you just put one directly under the other. when piggy reaches the end of a Chain (after you hit `start` in the song screen) it will look to the next row to see if there is another Chain to play. if so, the play arrow proceeds to the next row. if piggy encounters a blank space, it will return to the top of the current chunk of Chains it is playing through for the channel.

![](https://web.archive.org/web/20190513041654im_/http://wiki.littlegptracker.com/lib/exe/fetch.php?media=quick:start11_1.1f.png)

inside of each Chain behavior is similar: piggy always starts playing at position 00 and plays until it hits blank space. when piggy reaches the end of the Phrases, it will start playing position 00 of the next Chain. if you want your Chains to play simultaneously and loop together, you should put the same number of Phrases in all of them. you can also get interesting results and permutations in your composition by using different numbers of Phrases in each Chain :)

if this explanation was too clumsy for you i suggest just pushing play on the Song screen, Chain screen, and Phrase screen. pressing play in each place will play at whatever level you're at and following the play arrow will tell you what's going on.

## saving your work

![](https://web.archive.org/web/20190513041654im_/http://wiki.littlegptracker.com/lib/exe/fetch.php?media=quick:start13_1.1f.png)

saving should be done frequently, especially when working with ghetto builds! if i work out a hot Chain or two i'll typically take a break and save. another good time to save is when your batteries are getting low. if you're running piggy on the gp2x you'll have fair warning–the background will start to draw as grey and then black as the juice dries up in your cells.

to save, piggy playback must be stopped (so hit `start` if need be). navigate to the Project screen (from the Song screen hit `R+joystick:up`). hit `joystick:down` until the cursor is on `Save Song`. the cursor will disappear while the song is saving. don't turn your machine off until the cursor returns or your save won't complete! you can navigate back to the Song screen by pressing `R+joystick:down`.

don't forget to come back later and keep butchering the pig :)

## live mode & muting

![](https://web.archive.org/web/20190513041654im_/http://wiki.littlegptracker.com/lib/exe/fetch.php?media=quick:start14_1.1f.png)

live mode allows you to take full advantage of the grid based song screen in piggy. pressing `start` in live mode won't cause all of the Chains across a row to start at once–it only plays that chain in that channel. the rule is still only one Chain playing on each channel at a time, but you can have each channel playing back a Chain on a different row. instant remix bliss!

to enter live mode, press `B+joystick:right` in the Song screen. you should see Live in the upper left hand corner of the screen. getting back to Song mode is as easy as hitting `B+joystick:right` again.

when you hit `start` with the cursor on a Chain in live mode a play arrow will begin to blink, indicating that the Chain is queued up. queued Chains will begin to play when a Chain currently playing on any reaches it's end (regardless of if the currently playing Chain will loop at it's end or another Chain is queued on the same channel).

you can hit `start+start` to activate an urgent start for a Chain. this will make the play arrow blink faster and the Chain will being when the currently playing Phrase ends.

to stop a Chain from playing hit `R+start` (or `R+start+start` if you're feeling urgent). the play arrow will change to a blinking underscore and the Chain will stop playing when it or another Chain reaches an end (which ever comes first).

![](https://web.archive.org/web/20190513041654im_/http://wiki.littlegptracker.com/lib/exe/fetch.php?media=quick:start15_1.1f.png)

muting allows you to stop sound from coming out of a channel which is still playing. to toggle muting on and off hit `R+B`. you can hold down `R` while jamming on `B` for some performance fun. also, if you hit `R+B` and then release `R` with `B` still held, the mute will stick. the same moves work for un-muting a muted channel.

in the same way, you can solo a channel with `R+A`. soloing causes only the current channel to be audiable–all other channels are muted.

mute and solo effect an entire channel, rather than a particular Chain on a channel, so you can have your cursor on any row of the channel when you perform them. it's also possible to perform mute and solo across more than one channel at once by creating a selection across the channels you want to control (as above).

## further reading

if you made it this far and are still thirsting for more knowledge, you may want to page through the [reference manual](What-is-LittlePiggyTracker.md)
<!-- or the [rest of this wiki](https://web.archive.org/web/20190513041654/http://wiki.littlegptracker.com/doku.php?id=start "start"). -->

<!-- if you want to stay on the cutting edge, subscribe to the [lgpt mailing list](https://web.archive.org/web/20190513041654/http://launch.groups.yahoo.com/group/littlegptracker/ "http://launch.groups.yahoo.com/group/littlegptracker/"). -->

if you want a quick reference guide to carry anywhere (useful for piggin' while traveling), check this little summary that Larsby made [here](http://www.larsby.com/johan/?p=106)
