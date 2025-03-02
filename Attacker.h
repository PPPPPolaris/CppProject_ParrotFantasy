//
// Created by MR on 25-1-7.
//

#ifndef CPPPROJECT_ATTACKER_H
#define CPPPROJECT_ATTACKER_H

#include "Character.h"
#include "Ghost.h"
#include <iostream>
#include <QTimer>
using namespace std;
class Attacker: public Character{
protected:
    int cost;
    int attack_distance;//攻击距离
    QTimer* attackTimer;
    Map& map;
    QTimer* attackEffectMoveTimer;
    Lattice* attack_dst;
    int attackEffect_x;
    int attackEffect_y;
    Character* target;
    bool isAttacking;
public:
    QLabel* attackEffect;
    QLabel* towerLabel;
    Attacker(Map& map);
    virtual ~Attacker();
    Character* find_target(Map& map);
    virtual void attack();
    void startAttacking();
    void stopAttacking();
    void attackGhost(Ghost* ghost);
    void moveAttackEffect();
    QTimer* getAttackTimer();
    QTimer* getAttackEffectTimer();
    void update();

};

class BasicTower : public Attacker{//近战普通攻击塔
public:
    BasicTower(Map& map);
    ~BasicTower();
};

class FireTower : public Attacker{
public:
    FireTower(Map& map);
    ~FireTower();
};

#endif //CPPPROJECT_ATTACKER_H
