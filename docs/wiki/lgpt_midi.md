LGPT supports most of the basic midi commands. It can send note on/off, volume information, continuous controller data and MIDI program change. More information on this can be found in the **[reference manual](./What-is-LittlePiggyTracker.md#commands)**. Platform specific information follows.

## GP2x

The current gp2x firmware doesn't allow you to set arbitrary baud rates for the serial port (as the gp32 does) so piggy sends out serial data at an overclocked 38400bps. a pic is required to convert that high speed data down to midi's 31250 bps (±1%) spec. firestARTer has build such an interface:

[GP2x Midi-Converter by firestARTer](http://www.firestarter-music.de/gp2x "http://www.firestarter-music.de/gp2x")

Additionally you will need to:

1. Disable USB Host Networking

2. To avoid MIDI-data-garbarge being sent by the gp2x out your midi interface, piggy has to be started by a script. To do this you have to write the following into an empty plain text file:

```
./lgpt.gpe > midiout.log 2>&1
```

- save it as "filename.gpe" (for example lgpt_midi.gpe)

- copy it into the folder where your pig lives.

If the converter doesn't work, try to swaping the pin connections of the MIDI-connector.

## GP32

![static.flickr.com_49_134601318_bd8935a03c_m.jpg](https://web.archive.org/web/20180622081958im_/http://wiki.littlegptracker.com/lib/exe/fetch.php?tok=769215&media=http%3A%2F%2Fstatic.flickr.com%2F49%2F134601318_bd8935a03c_m.jpg "static.flickr.com_49_134601318_bd8935a03c_m.jpg")

Please note: Piggy is no longer supported on this platform. ~~You can download the last stable binary for it [here](http://www.littlegptracker.com/lgpt_GP32.zip "http://www.littlegptracker.com/lgpt_GP32.zip")~~ .
Legacy information about the midi adapter can be found **[here](./dfs_midi_cable.md)**_

## Win32

The windows version supports midi without much fuss. Simply navigate to the **Project** menu scroll down to **Midi** and select the midi out port you wish to use. ~~More tips and tricks for windows can be found **[here](https://web.archive.org/web/20160325180418/http://wiki.littlegptracker.com/doku.php?id=lgpt:win_midi "lgpt:win_midi")**.~~

## Linux and Raspbery Pi

Much the same as Windows, if a device is detected by the OS, it will show up in the same place. ~~Power tips can be found **[here](https://web.archive.org/web/20160325231144/http://wiki.littlegptracker.com/doku.php?id=lgpt:unix_midi "lgpt:unix_midi")**.~~

## Midi Input (TODO)
