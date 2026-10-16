#include <vector>
#include <cmath>
#include "Enemy.h"
#include "Bullet.h"
#include "Player.h"
#include "Enemy3.h"

Enemy3::Enemy3(CMPUT350::Point2D loc, const std::vector<CMPUT350::Point2D> &path, int rando): Enemy(loc, path, rando){ health = 3;}

void Enemy3::Initialize(CMPUT350::GameContext *context){return;}

void Enemy3::LateUpdate(CMPUT350::GameContext *context){return;}

bool Enemy3::Attack(CMPUT350::GameContext *context, const CMPUT350::Point2D &target, int rando){
    if (!inFormation || attacking || attackDelay > 0){
        attackDelay = rando % 300;
        return false;
    }
    
    CMPUT350::Point2D bottom(enemyLoc.x, 750.0f);

    std::vector<CMPUT350::Point2D> pts;

    pts.push_back(enemyLoc);
    pts.push_back(CMPUT350::Point2D(enemyLoc.x + 200.0f, enemyLoc.y));
    pts.push_back(CMPUT350::Point2D(enemyLoc.x + 250.0f, enemyLoc.y + 250.0f));
    pts.push_back(bottom);

    pts.push_back(CMPUT350::Point2D(formationLoc.x - 50.0f, 200.0f));
    pts.push_back(CMPUT350::Point2D(formationLoc.x, 400.0f));
    pts.push_back(formationLoc);

    startPath(pts);
    attacking = true;
    return true;
}

void Enemy3::RenderForeground(CMPUT350::GameContext *context){
    if(health >= 3){
        context->ScreenContext->DrawRect(CMPUT350::Rect(CMPUT350::Point2D(0,0), 8.0f), CMPUT350::Colors::green);
    } else if(health >= 2){
        context->ScreenContext->DrawRect(CMPUT350::Rect(CMPUT350::Point2D(0,0), 8.0f), CMPUT350::Colors::blue);
    } else {
        context->ScreenContext->DrawRect(CMPUT350::Rect(CMPUT350::Point2D(0,0), 8.0f), CMPUT350::Colors::grey);
    }
}