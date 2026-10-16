#include <vector>
#include <cmath>
#include "Enemy.h"
#include "Bullet.h"
#include "Player.h"
#include "Enemy1.h"

Enemy1::Enemy1(CMPUT350::Point2D loc, const std::vector<CMPUT350::Point2D> &path, int rando): Enemy(loc, path, rando){ health = 1;}

void Enemy1::Initialize(CMPUT350::GameContext *context){ 
    localShapes.clear();

    localShapes.emplace_back(CMPUT350::Rect(-6.5f, -6.5f, 12.5f, 12.5f));

    return;
}

void Enemy1::LateUpdate(CMPUT350::GameContext *context){return;}

bool Enemy1::Attack(CMPUT350::GameContext *context, const CMPUT350::Point2D &target, int rando){
    if (!inFormation || attacking || attackDelay > 0){
        attackDelay = rando % 300;
        return false;
    }
    
    std::vector<CMPUT350::Point2D> pts;

    pts.push_back(enemyLoc);
    pts.push_back(CMPUT350::Point2D((enemyLoc.x + target.x)/2, enemyLoc.y + 250.0f));
    pts.push_back(CMPUT350::Point2D((enemyLoc.x + target.x)/2, enemyLoc.y + 500.0f));
    pts.push_back(target);

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
