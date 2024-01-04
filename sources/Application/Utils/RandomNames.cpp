#include <string>
#include <vector>
#include "RandomNames.h"

static const std::string adj[] = {
        "Red", "Swift", "Spoopy", "Gentle", "Fierce",
        "Sparkling", "Magic", "Curious", "Fast", "Hyped",
        "Rizzy", "Radiant", "Soothing", "Weird", "Haunted",
        "Buzzy", "Wild", "Joyful", "Serene", "Wobbly",
        "Lively", "Dopey", "Dynamic", "Graceful", "Cool",
        "Playful", "Dorky", "Singing", "Clever", "Quirky",
        "Dull", "Fine", "Gold", "Gray", "Huge",
        "Light", "Chocolate", "Ripe", "Sour", "Tart",
        "Tough", "Brisk", "Fresh", "Grand", "Lean",
        "Lush", "Mild", "Pale", "Rich", "Ripe"
    };
static const std::string vrb[] =
    {
        "Jump", "Explore", "Dance", "Whisper", "Roar",
        "Run", "Climb", "Song", "Sleep", "Laugh",
        "Banana", "Fly", "Reader", "Build", "Create",
        "Hiker", "Cook", "Brows", "Cod", "Dope",
        "Glow", "Gyatt", "Dream", "Play", "Wire",
        "Holla", "Question", "Rizz", "Plant", "Craft",
        "Pecker", "Roar", "Purr", "Surfer", "Drum",
        "Kick", "Flip", "Snap", "Clap", "Snap",
        "Bite", "Chew", "Hunt", "Singer", "Draw",
        "Sleeper", "Skier", "Smile", "Yell", "Zoomer"
    };

std::vector<std::string> RandomNames::adjectives_ (adj, adj + sizeof(adj) / sizeof(adj[0]) );
std::vector<std::string> RandomNames::verbs_ (vrb, vrb + sizeof(vrb) / sizeof(vrb[0]) );
