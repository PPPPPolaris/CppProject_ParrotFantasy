//
// Created by MR on 25-1-7.
//

#ifndef CPPPROJECT_CHARACTER_H
#define CPPPROJECT_CHARACTER_H

#include <QObject>
class Map;
class Lattice;
class Character : public QObject{
    Q_OBJECT
protected:
    int attack_power;//攻击力
    int health;//生命值
    Lattice* location;
    bool inAttack;
public:
    Character();
    Character(int attack_power, int health);
    virtual ~Character();
    void set_location(Lattice* location);
    Lattice* get_location();
    void remove_location();
    void attack();
    virtual void reduce_health(int damage);
    void setInAttack(bool b);
    virtual void update();
};

#endif //CPPPROJECT_CHARACTER_H
