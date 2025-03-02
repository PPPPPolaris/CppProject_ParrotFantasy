//
// Created by MR on 25-1-7.
//

#ifndef CPPPROJECT_GHOST_H
#define CPPPROJECT_GHOST_H

#include "Character.h"
#include "Lattice.h"
#include <QTimer>
#include <QLabel>
class Ghost : public Character {
protected:
    int bonus;
    QTimer* moveTimer;//移动定时器
    int speed;//控制移动速度
    Map& map;
    QLabel* ghostLabel;
    QTimer* flashTimer;
    bool isFlashing;
    void startFlashing();
    void toggleFlash();

public:
    void attack();
    Ghost(Lattice* startPoint, Map& map, int speed);
    void reduce_health(int damage);
    ~Ghost();
    void move();
    Lattice* getLocation();
    QLabel* getGhostLabel();
    QTimer* getMoveTimer();


};
class BasicGhost : public Ghost {
public:
    BasicGhost();
    ~BasicGhost();
};

#endif //CPPPROJECT_GHOST_H
