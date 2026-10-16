#ifndef RANDOM_H
#define RANDOM_H

#include <cstdlib>
#include <ctime>

struct Randomizer {
    int randNum = 0;
    Randomizer() { std::srand(std::time(nullptr));}
    int GetRand() {
        randNum = std::rand();
        return randNum;
    }
};

#endif //RandHelper