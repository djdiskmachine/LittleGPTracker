#include <string>
#include <vector>
#include "time.h"

std::vector<std::string> adjectives =
                                    {
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
std::vector<std::string> verbs =
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

class RandomNames {
  public:
  RandomNames() { srand(uint(time(NULL))); }
  std::string getRandomName();
};

// Generate a random name made in the format of: "adjective-verb"
std::string getRandomName() {
    std::string adjective = adjectives[rand() % adjectives.size()];
    std::string verb = verbs[rand() % verbs.size()];
    int maxLength = 12;

    while ((adjective + verb).length() > maxLength) {
        adjective = adjectives[rand() % adjectives.size()];
        verb = verbs[rand() % verbs.size()];
    }

    return (adjective + verb).c_str();
}

