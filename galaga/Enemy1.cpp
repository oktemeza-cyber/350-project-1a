#include <vector>
#include <cmath>
#include "Enemy.h"
#include "Bullet.h"
#include "Player.h"
#include "Enemy1.h"

Enemy1::Enemy1(CMPUT350::Point2D loc, const std::vector<CMPUT350::Point2D> &path): Enemy(loc, path){ health = 1;}

void Enemy1::Initialize(CMPUT350::GameContext *context){return;}

void Enemy1::LateUpdate(CMPUT350::GameContext *context){return;}

bool Enemy1::Attack(CMPUT350::GameContext *context, const CMPUT350::Point2D &target){
    if (!inFormation){
        return false;
    }
    
    CMPUT350::Point2D bottom(enemyLoc.x, 900.0f);

    std::vector<CMPUT350::Point2D> pts;

    pts.push_back(enemyLoc);
    pts.push_back(CMPUT350::Point2D(enemyLoc.x, enemyLoc.y + 250.0f));
    pts.push_back(CMPUT350::Point2D(enemyLoc.x, enemyLoc.y + 500.0f));
    pts.push_back(bottom);

    pts.push_back(CMPUT350::Point2D(formationLoc.x, 700.0f));
    pts.push_back(CMPUT350::Point2D(formationLoc.x, 400.0f));
    pts.push_back(formationLoc);

    startPath(pts);
    attacking = true;
    return true;
}

void Enemy1::RenderForeground(CMPUT350::GameContext *context){
    context->ScreenContext->DrawRect(CMPUT350::Rect(CMPUT350::Point2D(0,0), 12.0f), CMPUT350::Colors::red);
}
