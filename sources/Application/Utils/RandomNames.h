#ifndef _RANDOM_NAMES_H_
#define _RANDOM_NAMES_H_

#include <string>
#include <vector>
#include <cstdlib>
#include "time.h"

class RandomNames {
  private:
    static std::vector<std::string> adjectives_;
    static std::vector<std::string> verbs_;
  public:
    RandomNames() { srand(int(time(NULL))); }
    static const std::string getRandomName() {
        std::string adjective = adjectives_[rand() % adjectives_.size()];
        std::string verb = verbs_[rand() % verbs_.size()];
        int maxLength = 12;

        while ((adjective + verb).length() > maxLength) {
            adjective = adjectives_[rand() % adjectives_.size()];
            verb = verbs_[rand() % verbs_.size()];
        }

        return (adjective + verb).c_str();
    }
};

#endif //_RANDOM_NAMES_H_
