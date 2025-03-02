//
// Created by MR on 25-1-7.
//

#ifndef CPPPROJECT_MAP_H
#define CPPPROJECT_MAP_H

#include <vector>
#include <QWidget>
#include <QPainter>
#include <QLabel>
#include <QVBoxLayout>
#include <QTimer>
#include <QList>
#include "Attacker.h"
#include <QStackedWidget>
using namespace std;
class Lattice;
namespace Ui {
class Map;
}
class Map: public QWidget{
    Q_OBJECT

    int r;
    int c;
    int gridSize;
    int y0;
    vector<vector<int>> abstract_map;
    vector<vector<Lattice*>> game_map;
    Lattice* start_point;//路径以链表形式存储
    Lattice* end_point;
    int money;
    int carrot_life;
    QPixmap image;
    QTimer* spawnTimer;//定时生成Ghost的定时器
    QList<Ghost*> ghosts;//存储当前所有存活的Ghost
    int currentWave;
    int ghostsInCurrentWave;
    QList<Attacker*>towers;//存储放置的炮塔
    int mostWaves;
    int ghostSpeed;
    bool carrotProtected;
    QLabel* protection;
    bool towerUpdated;
    QLabel* towerUpdateLabel;
public:
    QLabel* money_label;
    QLabel* life_label;
    QHBoxLayout *money_layout;
    QHBoxLayout *life_layout;
    QWidget* money_container;
    QWidget* life_container;
    Map(int ghostSpeed);
    void print_path();
    vector<Lattice*>& operator[] (int i);
    int get_r();
    int get_c();
    int get_carrot_life();
    void reduce_carrot_life(int damage);
    void reduce_money(int reduce_money);
    void add_money(int add_money);
    void loadMap(const QString& imagePath, int r, int c, int gridWidth, int girdHeight, int y0, vector<vector<int>> abstract_map, int start_i, int start_j, int mostWaves);
    //1表示向下，2表示向右，3表示向上，4表示向左，-1表示终点，0表示非路径区域
    void updateImage();
    void updateMoneyImage();
    void updateLifeImage();
    void startSpawningGhosts();
    void spawnGhost();
    void removeGhost(Ghost* ghost);
    void stopGame();
    void gameOver();
    void win();
protected:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);

signals:
    void chooseBack();

private:
    void showTowerSelectWindow(int grid_i, int grid_j, const QPoint& clickPos);
    void placeTower(int grid_i, int grid_j, int towerType);
    void restartGame();
    void writeTextFile();
    void startNextWave();
    void protectCarrot();
    void updateTower(int i, int j);
};

#endif //CPPPROJECT_MAP_H
