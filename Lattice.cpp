//
// Created by MR on 25-1-7.
//
#include "Lattice.h"
#include "Character.h"

Lattice::Lattice(QRect rect, int x, int y, int w, int h):x(x), y(y), w(w), h(h){
    content = nullptr;
    this->rect = rect;
}

Lattice::~Lattice()
{
    content = nullptr;
    next = nullptr;
};

void Lattice::set_content(Character *content) {
    this->content = content;
}

void Lattice::remove_content() {
    content = nullptr;
}

QRect Lattice::getRect() const
{
    return QRect(x, y, w, h);
}

Lattice::Type Lattice::getType() const
{
    return type;
}
void Lattice::setType(Type newType){
    type = newType;
}
