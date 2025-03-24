# dfs_midi_cable

![static.flickr.com_49_134601318_bd8935a03c_m.jpg](https://web.archive.org/web/20160325031702im_/http://wiki.littlegptracker.com/lib/exe/fetch.php?tok=769215&media=http%3A%2F%2Fstatic.flickr.com%2F49%2F134601318_bd8935a03c_m.jpg "static.flickr.com_49_134601318_bd8935a03c_m.jpg")

The gp32 can send midi out with this schematic! Small and powered off the gp32's +3V, it can be built into a cable.

![](https://web.archive.org/web/20160325031702im_/http://wiki.littlegptracker.com/lib/exe/fetch.php?tok=934bff&media=http%3A%2F%2Fmp3death.us%2Flgpt%2Fgfx%2FDFS_GP32_MIDI_OUT_V1.PNG)

I would suggest a female din5 rather than a male din5 … the pin # to connect to are the same but the pinout diagram is flipped. This is because male2male midi cables are the standard and you may want to extend the DFS cable.

The MOSFET used is next to fly shit and can be a pain to solder so i'd like to try a transistor design instead sometime.

~~Check [the gp32 EXT port pinout](https://web.archive.org/web/20160325031702/http://wiki.littlegptracker.com/doku.php?id=lgpt:mp3death.us_lgpt_gfx_ioport-en.pdf "lgpt:mp3death.us_lgpt_gfx_ioport-en.pdf") if you're going to hack up a samsung cable. The gp2x wiki has [a good guide on taking phone cables apart and putting them back together](https://web.archive.org/web/20130124153800/http://wiki.gp2x.org/wiki/SamsungE810Cable "http://wiki.gp2x.org/wiki/SamsungE810Cable"), but remember the pinouts there are for the wrong machine!~~

I just added my own Transistor-based Midi-Interface. I limited the Midi-current a bit to take care of battery time. If you get some Midi-Gear not to work you could maybe decrease R1 resistance a bit ( eg. to 390 ). BC846 and BAT46 are very common standard components and so easy to get.

![](https://web.archive.org/web/20160325031702im_/http://wiki.littlegptracker.com/lib/exe/fetch.php?tok=774120&media=http%3A%2F%2Ffirestarter-music.de%2Fgp32_midi.png)

PS: I simply used my GP-Link (wireless)-cable to do the midi-cable, since i will probably never use it. All needed lines are already connected.
