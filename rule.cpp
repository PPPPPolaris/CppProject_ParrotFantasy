#include "rule.h"
#include "ui_rule.h"

rule::rule(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::rule)
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
}

void rule::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap pixmap(":/background/img/rule.png");

    if (!pixmap.isNull()) {
        painter.drawPixmap(0, 0, this->width(), this->height(), pixmap);
    } else {
        qDebug() << "Failed to load image!";
    }
}

rule::~rule()
{
    delete ui;
}
