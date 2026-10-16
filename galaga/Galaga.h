#ifndef GALAGA_H
#define GALAGA_H

#include "GameObject.h"
#include "MathUtil.h"
#include "RandHelper.h"
#include <string>

enum class GameState { // Basically int, but named!
    LoadStars,
    Waiting,
    Startup,
    WaveStart,
    Playing,
    Respawn,
    GameOver,
    Winning
};

class Galaga : public CMPUT350::GameObject {
    public:
        Galaga(int width, int height);
        void Initialize(CMPUT350::GameContext *context) override;
        void Update(CMPUT350::GameContext *context) override;
        void ReceiveNotification(const std::string &key) override;
    private:
        GameState gameState;
        int lives;
        int score;
        int wave;
        bool coinInserted;
        CMPUT350::Point2D windowSize;
        Randomizer randomizer;

        int respawnTimer;
};

#endif // GALAGA_H