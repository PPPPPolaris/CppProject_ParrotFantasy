//
// Created by MR on 25-1-7.
//
#include "Map.h"
#include "Lattice.h"
#include <iostream>
#include <QColor>
#include <QPixmap>
#include <QMouseEvent>
#include <QDialog>
#include <QHBoxLayout>
#include <QString>
#include <QChar>
#include <QLabel>
#include <QTimer>
#include <QPushButton>
#include <QFont>
#include <QMessageBox>
#include <QDebug>
#include <QFile>

Map::Map(int ghostSpeed):ghostSpeed(ghostSpeed){
    this->setWindowIcon(QPixmap(":/item/img/icon.png"));

    money = 500;
    carrot_life = 100;
    carrotProtected = false;
    protection = new QLabel(this);
    towerUpdated = false;
    towerUpdateLabel = new QLabel(this);

    money_label = new QLabel(QString::number(money), this);
    life_label = new QLabel(QString::number(carrot_life), this);
    money_label->move(60,12);
    life_label->move(730,15);

    QFont font("Comic Sans MS",20);
    money_label->setFont(font);
    life_label->setFont(font);

    spawnTimer = new QTimer(this);
    connect(spawnTimer, &QTimer::timeout, this, &Map::spawnGhost);

    startSpawningGhosts();

    //绘制退出按钮
    QPushButton* quit = new QPushButton(this);
    quit->setStyleSheet("background-image: url(:/button/img/quit.png);"
                        "border: node;");
    quit->setGeometry(870,0,73,70);
    connect(quit, &QPushButton::clicked, this, [=](){
        stopGame();
        emit chooseBack();
    });

    //绘制导出按钮
    QPushButton* exportMap = new QPushButton(this);
    exportMap->setStyleSheet("background-image: url(:/button/img/ExportMap.png);"
                        "border: node;");
    exportMap->setGeometry(350,10,197,57);
    connect(exportMap, &QPushButton::clicked, this, [=](){
        writeTextFile();
        QMessageBox::information(this, "提示", "您已成功导出该地图！", QMessageBox::Ok);
    });

}


void Map::print_path() {
    Lattice* tmp = start_point;
    while (tmp != end_point){
        cout << '(' << tmp->x << ',' << tmp->y << ')';
        tmp = tmp->next;
        cout << "->";
    }
    cout << '(' << tmp->x << ',' << tmp->y << ')' << endl;
}

int Map::get_r() {
    return r;
}

int Map::get_c() {
    return c;
}

int Map::get_carrot_life()
{
    return carrot_life;
}

vector<Lattice*> &Map::operator[](int i) {
    return game_map[i];
}

void Map::reduce_carrot_life(int damage) {
    qDebug() << "Carrot is attacked by";
    if (carrot_life > damage){
        carrot_life -= damage;
        updateLifeImage();
    }else{
        carrot_life = 0;
        updateLifeImage();
        gameOver();
    }
}

void Map::reduce_money(int reduce_money)
{
    money -= reduce_money;
}

void Map::add_money(int add_money)
{
    money += add_money;
}

void Map::loadMap(const QString &imagePath, int r, int c,int gridWidth, int gridHeight, int y0, vector<vector<int>> abstract_map, int start_i, int start_j, int mostWaves)
{
    this->mostWaves = mostWaves;
    this->gridSize = gridWidth;
    this->y0 = y0;
    image.load(imagePath);
    this->r = r;
    this->c = c;
    this->abstract_map = abstract_map;

    game_map.resize(r);
    for (int i = 0; i < r; ++i) {
        game_map[i].resize(c, nullptr);
    }

    if(image.isNull()){
        qDebug() << "Failed to load image!";
        return;
    }
    Lattice* pre = nullptr;//标记路径上的前一个格子
    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            QRect rect(j*gridWidth, y0 + i * gridHeight, gridWidth, gridHeight);
            Lattice* lattice = new Lattice(rect, j*gridWidth, y0 + i * gridHeight, gridWidth, gridHeight);
            if (abstract_map[i][j] > 0){
                lattice->setType(Lattice::Path);
                if(pre == nullptr && i == start_i && j == start_j) {
                    pre = lattice;
                    start_point = lattice;
                }
            }else if(abstract_map[i][j] == 0){
                lattice->setType(Lattice::TowerPLace);
            }else{
                lattice->setType(Lattice::Path);
                end_point = lattice;
            }
            lattice->i = i;
            lattice->j = j;
            game_map[i][j] =lattice;
        }
    }
    while (abstract_map[start_i][start_j] != -1) {
        switch (abstract_map[start_i][start_j]) {
        case 1:
            start_i++;
            break;
        case 2:
            start_j++;
            break;
        case 3:
            start_i--;
            break;
        case 4:
            start_j--;
            break;
        default:
            break;
        }
        pre->next = game_map[start_i][start_j];
        pre = game_map[start_i][start_j];
    }
    end_point->next = nullptr;

    update();
}

void Map::updateMoneyImage()
{
    money_label->setText(QString::number(money));
}

void Map::updateLifeImage()
{
    life_label->setText(QString::number(carrot_life));
}

void Map::startSpawningGhosts()
{

    spawnTimer->start(2000);
    currentWave = 1;
    ghostsInCurrentWave = 0;
}

void Map::spawnGhost()
{
    if(currentWave <= mostWaves){
        if(ghostsInCurrentWave < 5){
            if(start_point&&start_point->content == nullptr){
                Ghost* newGhost = new Ghost(start_point, *this, ghostSpeed);
                ghosts.append(newGhost);
                ghostsInCurrentWave++;
            }
        }else{
            //挺到最后一波时可给萝卜添加护盾
            if(currentWave == mostWaves - 1){
                carrotProtected = true;
                protection->setPixmap(QPixmap(":item/img/protection.png"));
                protection->move(350, 550);
                protection->setFixedSize(75,75);
            }else if(currentWave == mostWaves/2){
                //挺过一半时可以升级一个炮塔
                towerUpdated = true;
                towerUpdateLabel->setPixmap(QPixmap(":item/img/update.png"));
                towerUpdateLabel->move(450, 550);
                towerUpdateLabel->setFixedSize(75,75);
                towerUpdateLabel->raise();
            }


            // 当前波生成完怪物后，停止生成并启动 15 秒的延时
            qDebug() << "Wave " << currentWave << " finished. Pausing for 15 seconds.";
            spawnTimer->stop();
            // 设置15秒后继续生成下一波怪物
            QTimer::singleShot(10000, this, &Map::startNextWave);

            // 停止当前波怪物生成
            currentWave++;
            ghostsInCurrentWave = 0;
        }
    }else{
        spawnTimer->stop();
    }
}

void Map::removeGhost(Ghost *ghost)
{
    ghosts.removeAll(ghost);
    if(currentWave > mostWaves && ghosts.isEmpty()){
        win();
    }
}

void Map::stopGame()
{
    //停止生成怪物
    spawnTimer->stop();
    //停止ghost的移动
    for(Ghost* ghost:ghosts){
        if(ghost){
            //ghost->remove_location();
            ghost->getMoveTimer()->stop();
            //ghost->getGhostLabel()->hide();
            //ghost->deleteLater();
        }
    }
    //ghosts.clear();
    //停止攻击并停止攻击效果的移动
    for(Attacker* tower:towers){
        if(tower){
            //tower->remove_location();
            tower->getAttackEffectTimer()->stop();
            tower->getAttackTimer()->stop();
            tower->attackEffect->hide();
            //tower->towerLabel->hide();
            //tower->deleteLater();
        }
    }
    //towers.clear();
}

void Map::gameOver()
{
    stopGame();
    //显示游戏结束
    QMessageBox::information(this, "Game Over", "没能成功保卫萝卜，游戏结束");
}

void Map::win()
{
    stopGame();
    //显示游戏结束
    QMessageBox::information(this, "Game Won", "恭喜你成功消灭了所有怪物，游戏胜利！");
}

void Map::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if (!image.isNull()) {
        painter.drawPixmap(0, 0, this->width(), this->height(), image);
    } else {
        qDebug() << "Failed to load image!";
    }
    QPixmap grid_img;
    grid_img.load(":/grid/img/grid.png");
    for(int i = 0; i < r; i++){
        for (int j = 0; j < c; ++j) {
            Lattice* tmp = game_map[i][j];
            switch(tmp->getType()){
            case Lattice::Path:
                painter.drawPixmap(tmp->x, tmp->y, tmp->w, tmp->h, grid_img);
                break;
            default:
                break;
            }
        }
    }
    //绘制萝卜
    QPixmap carrot;
    carrot.load(":/item/img/carrot.png");
    painter.drawPixmap(this->end_point->x+10,this->end_point->y - 35, this->end_point->w, this->end_point->h+35, carrot);
    //绘制Ghost
    for (Ghost* ghost : ghosts) {
        if(ghost)
            ghost->getGhostLabel()->move(ghost->getLocation()->x, ghost->getLocation()->y);
    }

}

void Map::mousePressEvent(QMouseEvent *event)
{
    //遍历所有格子，检查点击位置
    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            if(game_map[i][j]->rect.contains(event->pos())){
                if(game_map[i][j]->getType() == Lattice::TowerPLace && game_map[i][j]->content == nullptr)
                    showTowerSelectWindow(i,j,event->pos());
                else if(game_map[i][j] == end_point && carrotProtected){
                    protectCarrot();
                }else if(game_map[i][j]->getType() == Lattice::TowerPLace && game_map[i][j]->content != nullptr){
                    updateTower(i, j);
                }
                return;
            }
        }
    }
}

void Map::showTowerSelectWindow(int grid_i, int grid_j, const QPoint &clickPos)
{
    QDialog *selectWindow = new QDialog(this);
    selectWindow->move(clickPos.x()+100, clickPos.y()+100);

    QHBoxLayout* layout = new QHBoxLayout(selectWindow);
    QPushButton* tower1 = new QPushButton(selectWindow);
    tower1->setFixedSize(80,80);
    tower1->setStyleSheet("background-image: url(:/item/img/tower1.png);"
                          "border: node;");

    QPushButton* tower2 = new QPushButton(selectWindow);
    tower2->setFixedSize(80,80);
    tower2->setStyleSheet("background-image: url(:/item/img/tower2.png);"
                          "border: node;");

    layout->addWidget(tower1);
    layout->addWidget(tower2);

    connect(tower1, &QPushButton::clicked, this, [this, grid_i, grid_j, selectWindow](){
        placeTower(grid_i, grid_j, 1);
        selectWindow->accept();
    });
    connect(tower2, &QPushButton::clicked, this, [this, grid_i, grid_j, selectWindow](){
        placeTower(grid_i, grid_j, 2);
        selectWindow->accept();
    });
    selectWindow->setLayout(layout);
    selectWindow->setFixedSize(200,90);
    selectWindow->exec();
}

void Map::placeTower(int grid_i, int grid_j, int towerType)
{
    int towerCost = 0;

    // 根据炮塔类型设置价格
    if (towerType == 1) {
        towerCost = 100;
    } else if (towerType == 2) {
        towerCost = 160;
    }

    if(money >= towerCost){
        reduce_money(towerCost);
        updateMoneyImage();
        Attacker* newTower;
        QLabel* towerLabel = new QLabel(this);
        switch (towerType) {
        case 1:
            newTower = new BasicTower(*this);
            towerLabel->setPixmap(QPixmap(":item/img/towerSet1.png"));
            towerLabel->move(game_map[grid_i][grid_j]->x, game_map[grid_i][grid_j]->y);
            break;
        case 2:
            newTower = new FireTower(*this);
            towerLabel->setPixmap(QPixmap(":item/img/towerSet2.png"));
            towerLabel->move(game_map[grid_i][grid_j]->x+5, game_map[grid_i][grid_j]->y);
        default:
            break;
        }
        newTower->set_location(game_map[grid_i][grid_j]);
        game_map[grid_i][grid_j]->set_content(newTower);
        towers.append(newTower);
        newTower->towerLabel = towerLabel;
        towerLabel->setFixedSize(75,75);
        towerLabel->show();
        towerLabel->stackUnder(towerUpdateLabel);
    }else{
        QMessageBox::warning(this, "金钱不足", "你没有足够的金钱来放置这个炮塔！");
    }
}

void Map::restartGame()
{
    carrot_life = 100;
    money = 500;
    ghosts.clear();

    updateLifeImage();
    updateMoneyImage();
    update();
}

void Map::writeTextFile()
{
    QFile file("map.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {  // 打开文件进行写入
        qDebug() << "Cannot open file for writing:" << file.errorString();
        return;
    }
    qDebug() << "Writing data: r=" << r << ", c=" << c << ", gridSize=" << gridSize
             << ", y0=" << y0 << ", start_point i=" << start_point->i << ", start_point j=" << start_point->j
             << ", mostWaves=" << mostWaves << ", ghostSpeed=" << ghostSpeed;

    QTextStream out(&file);  // 创建 QTextStream 对象
    out << r << ' ' << c << ' ' << gridSize << ' ' << y0 << ' ' << start_point->i << ' ' << start_point->j << ' ' << mostWaves << ' ' << ghostSpeed << '\n';
    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            out << abstract_map[i][j] <<' ';
        }
        out << '\n';
    }

    out.flush();
    file.close();  // 关闭文件
}

void Map::startNextWave()
{
    qDebug() << "Starting wave " << currentWave;
    spawnTimer->start(2000); // 继续定时生成怪物（每2秒生成一个怪物）
}

void Map::protectCarrot()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认添加护盾", "您确定要给萝卜添加护盾吗？这会消耗您50金币。",
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        qDebug() << "用户选择了添加护盾";
        if(money >= 50){
            protection->move(end_point->x, end_point->y);
            carrot_life += 30;
            carrotProtected = false;
            money -= 50;
            updateMoneyImage();
            updateLifeImage();
        }else{
            QMessageBox::warning(this, "金钱不足", "你没有足够的金钱来给萝卜添加护盾！");
        }
    } else {
        qDebug() << "用户选择了不添加护盾";
    }
}

void Map::updateTower(int i, int j)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认升级炮塔", "您确定升级这个炮塔吗？这会消耗您80金币。",
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        qDebug() << "用户选择了升级炮塔";
        if(money >= 80){
            towerUpdateLabel->move(game_map[i][j]->x, game_map[i][j]->y);
            game_map[i][j]->content->update();
            towerUpdated = false;
            money -= 80;
            updateMoneyImage();
        }else{
            QMessageBox::warning(this, "金钱不足", "你没有足够的金钱来升级这个炮塔！");
        }
    } else {
        qDebug() << "用户选择了不升级炮塔";
    }
}


