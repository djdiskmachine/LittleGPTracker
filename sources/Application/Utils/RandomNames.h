#ifndef _RANDOM_NAMES_H_
#define _RANDOM_NAMES_H_

#include <string>
#include <vector>
#include <cstdlib>
#include "time.h"

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

//Wonky assignment because < C++11
std::vector<std::string> adjectives_ (adj, adj + sizeof(adj) / sizeof(adj[0]) );
std::vector<std::string> verbs_ (vrb, vrb + sizeof(vrb) / sizeof(vrb[0]) );


class RandomNames {
  public:
    RandomNames() { srand(uint(time(NULL))); }
    std::string getRandomName();
};

// Generate a random name made in the format of: "adjective-verb"
std::string getRandomName() {
    std::string adjective = adjectives_[rand() % adjectives_.size()];
    std::string verb = verbs_[rand() % verbs_.size()];

    while ((adjective + verb).length() > MAX_NAME_LENGTH) {
        adjective = adjectives_[rand() % adjectives_.size()];
        verb = verbs_[rand() % verbs_.size()];
    }

    return (adjective + verb).c_str();
}

#endif //_RANDOM_NAMES_H_
