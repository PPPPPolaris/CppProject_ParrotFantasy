#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <vector>
#include "Map.h"
namespace Ui {
class game;
}

class game : public QWidget
{
    Q_OBJECT

public:
    explicit game(QWidget *parent = nullptr);
    ~game();
    void paintEvent(QPaintEvent *event);

signals:
     void chooseBack();


private:
    Ui::game *ui;
    Map* map = nullptr;
    //自导入地图
    int myGhostSpeed;
    int MyR;
    int MyC;
    int MyGridSize;
    int MyY0;
    vector<vector<int>> MyAbstractMap;
    int MyStartI;
    int MyStartJ;
    int MyMostWaves;
    void readInFile(const QString& fileName);
};

#endif // GAME_H
