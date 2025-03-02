//
// Created by MR on 25-1-7.
//
#include "Attacker.h"
#include "Map.h"
Attacker::Attacker(Map &map):map(map)
{
    attack_dst = nullptr;
    attackTimer = new QTimer(this);
    connect(attackTimer, &QTimer::timeout, this, &Attacker::attack);
    attackEffect = new QLabel(&map);
    attackEffectMoveTimer = new QTimer(this);
    connect(attackEffectMoveTimer, &QTimer::timeout, this, &Attacker::moveAttackEffect);
    isAttacking = false;
}

Attacker::~Attacker(){
    delete attackTimer;
    delete attackEffect;
    delete attackEffectMoveTimer;
    delete towerLabel;
}
Character* Attacker::find_target(Map& map) {
    if (location != nullptr){
        int i = location->i;
        int j = location->j;
        for (int k = 1; k <= attack_distance; ++k) {
            for (int x = i-k; x <= i+k; ++x) {
                for (int y = j-k; y <= j+k ; ++y) {
                    if (x>=0 && y>=0 && x<map.get_r() && y<map.get_c()){
                        if (map[x][y]->content != nullptr && dynamic_cast<Ghost*>(map[x][y]->content) != nullptr){
                            //qDebug() << "Target found at (" << x << "," << y << ")";
                            return map[x][y]->content;
                        }
                    }
                }
            }

        }
    }
    return nullptr;
}

void Attacker::attack() {
    target = find_target(map);
    if (target != nullptr && !isAttacking){
        isAttacking = true;
        attack_dst = target->get_location();

        //初始化攻击效果的位置
        attackEffect->move(location->x, location->y);
        attackEffect_x = location->x;
        attackEffect_y = location->y;
        attackEffect->show();
        //开始移动攻击效果
        attackEffectMoveTimer->start(50);
    }
}


void Attacker::moveAttackEffect()
{
    if(attack_dst != nullptr){
        int speed = 20;
        int dx = attack_dst->x - attackEffect_x;
        int dy = attack_dst->y - attackEffect_y;
        //qDebug() << "attackEffect at (" << attackEffect_x << ',' << attackEffect_y << ')';
        double distance = std::sqrt(dx * dx + dy * dy);
        //qDebug() << "Distance" << distance;
        if(attack_dst->content!=nullptr){
            //阻止目标继续往前走
            target->setInAttack(true);
            if(distance < speed){
                attackEffect->move(attack_dst->x, attack_dst->y);
                attackEffect->hide();
                qDebug()<<"Ghost at (" << target->get_location()->i << ',' << target->get_location()->j << ") is attacked!";
                attackEffectMoveTimer->stop();
                if(target){
                    target->setInAttack(false);
                    qDebug()<<"Ghost at (" << target->get_location()->i << ',' << target->get_location()->j << ") is released!";
                    target->reduce_health(attack_power);
                    isAttacking = false;
                }
            }else{
                attackEffect->move(attackEffect_x + speed*dx/distance, attackEffect_y + speed*dy/distance);
                attackEffect_x += speed*dx/distance;
                attackEffect_y += speed*dy/distance;
            }
        }else{
            attackEffect->hide();
            attackEffectMoveTimer->stop();
            isAttacking = false;
        }
    }
}

QTimer *Attacker::getAttackTimer()
{
    return attackTimer;
}

QTimer *Attacker::getAttackEffectTimer()
{
    return attackEffectMoveTimer;
}

void Attacker::update()
{
    attack_power += 20;
    attack_distance += 1;
}



BasicTower::BasicTower(Map& map):Attacker(map){
    attackTimer->start(1000);
    attack_power = 5;
    attack_distance = 3;//近战攻击距离
    cost = 100;
    attackEffect->setPixmap(QPixmap(":item/img/attack1.png"));
    attackEffect->setFixedSize(44,57);
}

BasicTower::~BasicTower(){
    delete attackTimer;
    delete attackEffect;
    delete attackEffectMoveTimer;
    delete towerLabel;
}

FireTower::FireTower(Map& map):Attacker(map)
{
    attackTimer->start(1000);
    attack_power = 15;
    attack_distance = 5;
    cost = 160;
    attackEffect->setPixmap(QPixmap(":item/img/attack2.png"));
    attackEffect->setFixedSize(32,79);
}
FireTower::~FireTower(){
    delete attackTimer;
    delete attackEffect;
    delete attackEffectMoveTimer;
    delete towerLabel;
}
