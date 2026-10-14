#include <vector>
#include <cmath>
#include "Enemy.h"
#include "Bullet.h"
#include "Player.h"
#include "Enemy2.h"

Enemy2::Enemy2(CMPUT350::Point2D loc, const std::vector<CMPUT350::Point2D> &path): Enemy(loc, path){ health = 2;}

void Enemy2::Initialize(CMPUT350::GameContext *context){return;}

void Enemy2::LateUpdate(CMPUT350::GameContext *context){return;}

bool Enemy2::Attack(CMPUT350::GameContext *context, const CMPUT350::Point2D &target){
    if (!inFormation){
        return false;
    }
    
    CMPUT350::Point2D bottom(enemyLoc.x, 750.0f);

    std::vector<CMPUT350::Point2D> pts;

    pts.push_back(enemyLoc);
    pts.push_back(CMPUT350::Point2D(enemyLoc.x + 50.0f, enemyLoc.y + 100.0f));
    pts.push_back(CMPUT350::Point2D(enemyLoc.x, enemyLoc.y + 250.0f));
    pts.push_back(bottom);

    pts.push_back(CMPUT350::Point2D(formationLoc.x - 200.0f, 500.0f));
    pts.push_back(CMPUT350::Point2D(formationLoc.x, 400.0f));
    pts.push_back(formationLoc);

    startPath(pts);
    attacking = true;
    return true;
}

void Enemy2::RenderForeground(CMPUT350::GameContext *context){
    context->ScreenContext->DrawCircle(CMPUT350::Point2D(0,0), 6.0f, CMPUT350::Colors::yellow);
}