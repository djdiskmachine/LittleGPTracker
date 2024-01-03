#include <string>
#include <vector>
#include "time.h"

std::vector<std::string> adjectives =
                                    {
                                        "Red", "Swift", "Spoopy", "Gentle", "Fierce",
                                        "Sparkling", "Magic", "Curious", "Dope", "Hype",
                                        "Rizzy", "Radiant", "Soothing", "Weird", "Haunted",
                                        "Buzzy", "Wild", "Joyful", "Serene", "Wobbly",
                                        "Lively", "Dopy", "Dynamic", "Graceful", "Cool",
                                        "Playful", "Dorky", "Singing", "Clever", "Quirky"
                                    };
std::vector<std::string> verbs =
                                    {
                                        "Jump", "Explore", "Dance", "Whisper", "Roar",
                                        "Run", "Climb", "Song", "Sleep", "Laugh",
                                        "Swimm", "Fly", "Reader", "Build", "Create",
                                        "Hiker", "Cook", "Brows", "Cod", "Dope",
                                        "Glow", "Search", "Dream", "Play", "Wire",
                                        "Holla", "Question", "Rizz", "Plant", "Craft"
                                    };

class RandomNames {
  public:
  RandomNames() { srand(uint8_t(time(NULL))); }
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

