#include "game.h"
#include "ui_game.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QTimer>
#include <QPushButton>
#include <QFile>

game::game(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::game)
{
    ui->setupUi(this);
    this->resize(950,630);
    this->setWindowTitle("CarrotFantasy");
    this->setWindowIcon(QPixmap(":/item/img/icon.png"));

    //绘制退出按钮
    QPushButton* quit = new QPushButton(this);
    quit->setStyleSheet("background-image: url(:/button/img/quit.png);"
                        "border: node;");
    quit->setGeometry(870,0,73,70);
    connect(quit, &QPushButton::clicked, [=](){
        emit this->chooseBack();
    });

    //绘制关卡按钮
    QPushButton* map1 = new QPushButton(this);
    map1->setStyleSheet("background-image: url(:/item/img/num1.png);"
                        "border: node;");
    map1->setGeometry(60,150,40,80);
    QPushButton* map2 = new QPushButton(this);
    map2->setStyleSheet("background-image: url(:/item/img/num2.png);"
                        "border: node;");
    map2->setGeometry(260,150,52,76);
    QPushButton* map3 = new QPushButton(this);
    map3->setStyleSheet("background-image: url(:/item/img/num3.png);"
                        "border: node;");
    map3->setGeometry(460,150,50,76);

    //第一关
    connect(map1, &QPushButton::clicked, [=](){
        map = new Map(1);

        map->loadMap(":/background/img/bg1.jpg", 4, 12, 75, 75, 190, {{0,1,0,0,0,0,0,0,0,0,-1,0},
                                                                      {0,1,0,0,0,0,0,0,0,0,3,0},
                                                                      {0,1,0,0,2,2,2,1,0,0,3,0},
                                                                      {0,2,2,2,3,0,0,2,2,2,3,0}}, 0, 1, 4);
        map->resize(950, 630);
        map->show();
        this->hide();
        connect(map, &Map::chooseBack, this, [=](){
            map->close();
            this->show();
        });
    });

    //第二关
    connect(map2, &QPushButton::clicked, [=](){
        map = new Map(3);

        map->loadMap(":/background/img/bg1.jpg", 5, 12, 75, 75, 115, {{0,0,0,0,0,0,0,2,1,0,0,0},
                                                                      {0,1,0,0,0,0,0,3,2,2,-1,0},
                                                                      {0,1,0,0,0,2,2,3,0,0,0,0},
                                                                      {0,1,0,0,0,3,0,0,0,0,0,0},
                                                                      {0,2,2,2,2,3,0,0,0,0,0,0}},1,1, 5);
        map->resize(950, 630);
        map->show();
        this->hide();
        connect(map, &Map::chooseBack, this, [=](){
            map->close();
            this->show();
        });
    });

    //第三关
    connect(map3, &QPushButton::clicked, [=](){
        map = new Map(3);

        map->loadMap(":/background/img/bg1.jpg", 5, 12, 75, 75, 115, {{0,1,4,4,4,4,4,4,4,4,4,4},
                                                                      {0,2,2,2,2,2,2,2,2,2,1,0},
                                                                      {1,4,4,4,4,4,4,4,4,4,4,0},
                                                                      {2,2,2,2,2,2,2,2,2,2,2,1},
                                                                      {0,0,-1,4,4,4,4,4,4,4,4,4}},0,11, 6);
        map->resize(950, 630);
        map->show();
        this->hide();
        connect(map, &Map::chooseBack, this, [=](){
            map->close();
            this->show();
        });
    });

    //绘制自导入地图按钮
    QPushButton* loadIn = new QPushButton(this);
    loadIn->setStyleSheet("background-image: url(:/item/img/loadIn.png);"
                        "border: node;");
    loadIn->setGeometry(600,160,197,57);
    connect(loadIn, &QPushButton::clicked, [=](){
        readInFile("map.txt");
        map = new Map(myGhostSpeed);
        map->loadMap(":/background/img/bg1.jpg", MyR, MyC, MyGridSize, MyGridSize, MyY0, MyAbstractMap, MyStartI, MyStartJ, MyMostWaves);
        map->resize(950, 630);
        map->show();
        this->hide();
        connect(map, &Map::chooseBack, this, [=](){
            map->close();
            this->show();
        });
    });


}
void game::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap pixmap(":/background/img/select.png");

    if (!pixmap.isNull()) {
        painter.drawPixmap(0, 0, this->width(), this->height(), pixmap);
    } else {
        qDebug() << "Failed to load image!";
    }
}

void game::readInFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Cannot open file for reading:" << file.errorString();
        return;
    }
    QTextStream in(&file);
    int count = 0;
    bool readMap = false;
    int i =0;
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(" ");
        for(const QString& part: parts){
            bool ok;
            int number = part.toInt(&ok);
            if (ok) {
                if(readMap){
                    MyAbstractMap[i].push_back(number);
                    if(MyAbstractMap[i].size() == MyC){
                        i++;
                    }
                }else{
                    if(count == 0){
                        MyR = number;
                        count++;
                    }else if(count == 1){
                        MyC = number;
                        MyAbstractMap.resize(MyR);
                        count++;
                    }else if(count == 2){
                        MyGridSize = number;
                        count++;
                    }else if(count == 3){
                        MyY0 = number;
                        count++;
                    }else if(count == 4){
                        MyStartI = number;
                        count++;
                    }else if(count == 5){
                        MyStartJ = number;
                        count++;
                    }else if(count == 6){
                        MyMostWaves = number;
                        count++;
                    }else if(count == 7){
                        myGhostSpeed = number;
                        readMap = true;
                    }
                }
            } else {
                qDebug() << "Invalid integer:" << part;
            }
        }
    }
    file.close();
}

game::~game()
{
    delete ui;
}
