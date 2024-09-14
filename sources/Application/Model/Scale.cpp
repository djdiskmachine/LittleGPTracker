// from: https://github.com/xiphonics/picoTracker
#include "Scale.h"

// Source of scales in original release:
// https://upload.wikimedia.org/wikipedia/commons/thumb/3/35/PitchConstellations.svg/1280px-PitchConstellations.svg.png

// Additional Scales
// https://pianoencyclopedia.com/scales/

const char *scaleNotes[scaleNoteCount] = {
    "C","C#","D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"
};

const char *scaleNames[scaleCount] = {"None (Chromatic)",
                                     "Acoustic",
                                     "Adonal malakh",
                                     "Aeolian mode (minor)",
                                     "Algerian",
                                     "Altered",
                                     "Augmented",
                                     "Bebop dominant",
                                     "Blues",
                                     "Dorian",
                                     "Double harmonic",
                                     "Enigmatic",
                                     "Flamenco",
                                     "Gypsy",
                                     "Half diminished",
                                     "Harmonic major",
                                     "Harmonic minor",
                                     "Hirajoshi",
                                     "Hungarian gypsy",
                                     "Hungarian minor",
                                     "Insen",
                                     "Ionian mode (major)",
                                     "Istrian",
                                     "Iwato",
                                     "Locrian",
                                     "Lydian augmented",
                                     "Lydian",
                                     "Major bebop",
                                     "Major locran",
                                     "Major pentatonic",
                                     "Melodic minor",
                                     "Melodic minor (asc)",
                                     "Minor pentatonic",
                                     "Mixolydian",
                                     "Neapolitan major",
                                     "Neapolitan minor",
                                     "Octatonic",
                                     "Persian",
                                     "Phrygian dominant",
                                     "Phrygian",
                                     "Prometheus",
                                     "Ryukyu",
                                     "Tritone",
                                     "Tercera Alta",
                                     "Ukranian",
                                     "Whole tone",
                                    };

const bool scaleSteps[scaleCount][scaleNoteCount] = {
    // "C","C#","D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"
    // "None (Chromatic)"
    {true, true, true, true, true, true, true, true, true, true, true, true},
    // "Acoustic"
    {true, false, true, false, true, false, true, true, false, true, false,
     false},
    // "Adonal malakh"
    {true, false, true, false, true, true, false, true, true, false, true,
     false},
    // "Aeolian mode (minor)"
    {true, false, true, true, false, true, false, true, true, false, true,
     false},
    // "Algerian"
    {true, false, true, true, false, false, true, true, true, false, false,
     true},
    // "Altered"
    {true, true, false, true, true, false, true, false, true, false, true,
     false},
    // "Augmented"
    {true, false, false, true, true, false, false, true, true, false, false,
     true},
    // "Bebop dominant"
    {true, false, true, false, true, true, false, true, false, true, true,
     true},
    // "Blues"
    {true, false, false, true, false, true, true, true, false, false, true,
     false},
    // "Dorian"
    {true, false, true, true, false, true, false, true, false, true, true,
     false},
    // "Double harmonic"
    {true, true, false, false, true, true, false, true, true, false, false,
     true},
    // "Enigmatic"
    {true, true, false, false, true, false, true, false, true, false, true,
     true},
    // "Flamenco"
    {true, true, false, false, true, true, false, true, true, false, false,
     true},
    // "Gypsy"
    {true, false, true, true, false, false, true, true, true, false, true,
     false},
    // "Half diminished"
    {true, false, true, true, false, true, true, false, true, false, true,
     false},
    // "Harmonic major"
    {true, false, true, false, true, true, false, true, true, false, false,
     true},
    // "Harmonic minor"
    {true, false, true, true, false, true, false, true, true, false, false,
     true},
    // "Hirajoshi"
    {true, false, true, true, false, false, false, true, true, false, false,
     false},
    // "Hungarian gypsy"
    {true, false, true, true, false, false, true, true, true, false, false,
     true},
    //  "Hungarian minor"
    {true, false, true, true, false, false, true, true, true, false, false,
     true},
    // "Insen"
    {true, true, false, false, false, true, false, true, false, false, true,
     false},
    // "Ionian mode (major)"
    {true, false, true, false, true, true, false, true, false, true, false,
     true},
    // "Istrian"
    {true, true, false, true, true, false, true, true, false, false, false,
     false},
    // "Iwato"
    {true, true, false, false, false, true, true, false, false, false, true,
     false},
    // "Locrian"
    {true, true, false, true, false, true, true, false, true, false, true,
     false},
    // "Lydian augmented"
    {true, false, true, false, true, false, true, false, true, true, false,
     true},
    // "Lydian"
    {true, false, true, false, true, false, true, true, false, true, false,
     true},
    // "Major bebop"
    {true, false, true, false, true, true, false, true, true, true, false,
     true},
    // "Major locran"
    {true, false, true, false, true, true, true, false, true, false, true,
     false},
    // "Major pentatonic"
    {true, false, true, false, true, false, false, true, false, true, false,
     false},
    // "Melodic minor"
    {true, false, true, true, false, true, false, true, true, true, true, true},
    // "Melodic minor (asc)"
    {true, false, true, true, false, true, false, true, false, true, false,
     true},
    // "Minor pentatonic"
    {true, false, false, true, false, true, false, true, false, false, true,
     false},
    // "Mixolydian"
    {true, false, true, false, true, true, false, true, false, true, true,
     false},
    // "Neapolitan major"
    {true, true, false, true, false, true, false, true, false, true, false,
     true},
    // "Neapolitan minor"
    {true, true, false, true, false, true, false, true, true, false, false,
     true},
    // "Octatonic"
    {true, false, true, true, false, true, true, false, true, true, false,
     true},
    // "Persian"
    {true, true, false, false, true, true, true, false, true, false, false,
     true},
    // "Phrygian dominant"
    {true, true, false, false, true, true, false, true, true, false, true,
     false},
    // "Phrygian"
    {true, true, false, true, false, true, false, true, true, false, true,
     false},
    // "Prometheus"
    {true, false, true, false, true, false, true, false, false, true, true,
     false},
    // Ryukyu
    {true, false, false, false, true, true, false, true, false, false, false, true},
    // "Tritone"
    {true, true, false, false, true, false, true, true, false, false, true,
     false},
    // "Tercera Alta"
    {false, true, true, false, false, false, true, false, false, true, false, false},
    // "Ukranian"
    {true, false, true, true, false, false, true, true, false, true, true,
     false},
    // "Whole tone"
    {true, false, true, false, true, false, true, false, true, false, true,
     false}
    
};
