-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
NOTE!
This build is derivative of the work of the original 
author m-.-n aka Marc Nostromo. The original work and 
releasing the source code has laid the foundation for
everything included in this package.

This build can be considered beta and has thus been tested
extensively. Please report any issues you could find
via Github:
    https://github.com/djdiskmachine/LittleGPTracker/issues

All implemented features have been tested not to break old
projects but make sure to backup your old cherished work
just to be safe <3
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

Changelog Feb 1 2024
1.3o_2
    Adds:
        Alt row count colors, shoutout to https://github.com/edne !
        00 and FE song screen colors, shoutout to https://github.com/koi-ikeno !
        RG35XX build, thanks to https://github.com/jpma29 for help with testing!
    Fixes:
        Root folder detection causing issues with importing samples on some systems (thank you jpma29)
        Name generator causing issues with importing samples on some systems (thanks Dosly)
        Actually random names (fixed random seed, thx Eldsvåda:)
        Save as fixed on MIYOO systems (thx Pret4t)
    Other:
        Refactor help legend

1.3o-1
    First stable release!
    Fixed errors when random naming projects (shoutouts to 256k!)
    Start sample navigation behavior adjusted
    New: When in Phrase or Table view, FX commands have
         a help legend at the top of the screen
    Update license to CC BY-NC-SA 4.0

    Official builds for:
        Miyoo mini
        Win32
        CHIP
        Raspberry Pi
        PSP
        Deb32

1.3o-beta-2
    Random name generator
        New project or Save As project creates a random name
        Courtesy of djdiskmachine

    Preview with start in sample browser
      Browse preview with start + up / down
      Load sample with start + right
      Up one level in folder structure with start + left

      B navigation no longer wraps around but stays at end points

    Skip build for bittboy and RG350 for now, no testers
    Bugs:
      SCREENMULT no longer applies for PSP issue (47)
      Disable default mapping for MIYOO to not conflict with mapping.xml issue (53)
        Shoutouts to fpiesik and Sacaredo for help debugging

1.3o-beta-1
    Pingpong loop now stable
        Courtesy of djdiskmachine 

    Add ability to change font
    USAGE: ./mkfont.py FONT_BMP [OUTPUT_FILE]
    FONT_BMP must be a black/white (#000000, #FFFFFF) 128x64px bmp
    if OUTPUT_FILE is omitted, will print to stdout
        Courtesy of subnixr 

    Adds Miyoo mini build
        Courtesy of Nine-H

    Changes filter attenuation to attenuation
    Adjust volume post scream filter 
    https://github.com/Mdashdotdashn/LittleGPTracker/issues/22

    Missing wiki content dumped to repo
        Courtesy of Peter Swimm 

1.3o-alpha-5
    Adds native SteamOS build
    Adds Bittboy/RS97/RG350 builds
        Merge of Gameblaba, CalebCDE and RafaVicos work
        Shoutouts!    
1.3o-alpha-4
    Adds Save As menu item - save a copy of your project with a new name
        Courtesy of yoyz (https://github.com/yoyz), biggup!
    Working Win32 build
        Requires included SDL.dll file in the same location as the binary
    Including mapping.xml
        Contains macro button examples
    Restored ping pong loop mode
        Know inssue: Starting in reverse is same as regular loop
    Sample song demoing new functionality
    Platforms: Added Win32
1.3o-alpha-3
    Change default colors
    Introduce cursor color
1.3o-alpha-2
    Add filter attenuation (volume post Scream)
    Platforms: Added deb and raspberry
1.3o-alpha-1
    Experimental ping pong loop mode
        not done and removed in following builds
1.3o-alpha-0
    Add Slice loop mode
    Platforms: PSP only
