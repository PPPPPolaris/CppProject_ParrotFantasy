//
// Created by MR on 25-1-7.
//
#include "Character.h"
#include "Map.h"
#include "Lattice.h"
Character::Character(){}
Character::Character(int attack_power, int health): attack_power(attack_power), health(health), location(nullptr) {
}
Character::~Character(){}

void Character::set_location(Lattice *location) {
    this->location = location;
    location->set_content(this);
}

Lattice *Character::get_location()
{
    return location;
}

void Character::remove_location() {
    if (location != nullptr){
        location->remove_content();
        location = nullptr;
    }
}

void Character::reduce_health(int damage)
{
    if(health > damage){
        health -= damage;
    }else{
        health = 0;
    }
}
void Character::setInAttack(bool b)
{
    inAttack = b;
}

void Character::update()
{
}

