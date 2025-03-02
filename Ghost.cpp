//
// Created by MR on 25-1-12.
//
#include "Ghost.h"
#include "Map.h"
void Ghost::startFlashing()
{
    if(!isFlashing){
        flashTimer->start();
        isFlashing = true;
        QTimer::singleShot(300, this, [this](){
            flashTimer->stop();
            ghostLabel->show();
            isFlashing=false;
        });
        ghostLabel->hide();
    }
}

void Ghost::toggleFlash()
{
    ghostLabel->setVisible(!ghostLabel->isVisible());
}

void Ghost::attack() {
    map.reduce_carrot_life(attack_power);
}

Ghost::Ghost(Lattice *startPoint, Map& map, int speed):map(map),speed(speed)
{
    health = 100;
    bonus = 50;
    attack_power = 10;
    inAttack = false;
    location = startPoint;
    location->content = this;
    moveTimer = new QTimer(this);
    connect(moveTimer, &QTimer::timeout, this, &Ghost::move);
    moveTimer->start(1000/speed);

    ghostLabel = new QLabel();
    ghostLabel->setParent(&map);
    ghostLabel->setPixmap(QPixmap(":item/img/ghost1.png"));
    ghostLabel->setFixedSize(95,95);
    ghostLabel->move(location->x-35, location->y-50);
    ghostLabel->show();

    flashTimer = new QTimer(this);
    flashTimer->setInterval(100);
    connect(flashTimer, &QTimer::timeout, this, &Ghost::toggleFlash);
    isFlashing = false;
}


void Ghost::reduce_health(int damage) {
    if (health > damage){
        health -= damage;
        startFlashing();
    } else {
        health = 0;
        map.add_money(bonus);
        map.updateMoneyImage();
        moveTimer->stop();
        location->remove_content();
        qDebug()<<"Ghost has died!";
        ghostLabel->hide();
        map.removeGhost(this);
        deleteLater();
    }
}

Ghost::~Ghost(){
    ghostLabel->deleteLater();
    ghostLabel = nullptr;
}

void Ghost::move()
{
    if(location->next != nullptr && !inAttack &&location->next->content == nullptr){
        location->remove_content();
        location = location->next;
        if(location!=nullptr){
            location->content = this;
            ghostLabel->move(location->x-35, location->y - 35);
        }
    }else if(location->next == nullptr){
        moveTimer->stop();
        attack();
        location->remove_content();
        ghostLabel->hide();
        map.removeGhost(this);
        delete this;
    }
}

Lattice *Ghost::getLocation()
{
    return location;
}

QLabel *Ghost::getGhostLabel()
{
    return ghostLabel;
}

QTimer *Ghost::getMoveTimer()
{
    return moveTimer;
}


