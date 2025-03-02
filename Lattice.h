//
// Created by MR on 25-1-7.
//

#ifndef CPPPROJECT_LATTICE_H
#define CPPPROJECT_LATTICE_H

#include <QRect>
class Character;
class Lattice {
    friend class Map;
    friend class Attacker;
    friend class Ghost;

public:
    enum Type {
        Path,
        TowerPLace,
        Empty
    };
private:
    Character* content;
    Lattice* next;
    bool is_end;
    bool is_start;
    int x, y, w, h;
    int i,j;//在地图中的序号
    Type type;
    QRect rect;

public:
    Lattice(QRect rect, int x=0, int y=0, int w = 0, int h = 0);
    ~Lattice();
    void set_content(Character* content);
    void remove_content();
    QRect getRect() const;
    Type getType() const;
    void setType(Type newType);
};

#endif //CPPPROJECT_LATTICE_H
