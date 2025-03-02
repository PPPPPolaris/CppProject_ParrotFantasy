#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , player(new QMediaPlayer(this))
    , audioOutput(new QAudioOutput(this))
{
    ui->setupUi(this);
    this->resize(950,630);
    this->setWindowTitle("CarrotFantasy");

    this->setWindowIcon(QPixmap(":/item/img/icon.png"));

    MusicManager::instance().playMusic("qrc:/music/music/BGMusic.mp3");


    game* gameSelect = new game();
    QPushButton *playButton = new QPushButton(this);

    playButton->setStyleSheet("background-image: url(:/button/img/play.png);"
                              "border: node;");

    playButton->setGeometry(375,500,227,66);
    connect(playButton, &QPushButton::clicked, [=](){
        gameSelect->show();
        this->hide();
    });
    connect(gameSelect, &game::chooseBack, this, [=](){
        gameSelect->hide();
        this->show();
    });


    rule* gameRule = new rule();
    QPushButton *ruleButton = new QPushButton(this);
    ruleButton->setStyleSheet("background-image: url(:/button/img/ruleButton.png);"
                              "border: node;");
    ruleButton->setGeometry(10,10,200,58);
    connect(ruleButton, &QPushButton::clicked, [=](){
        gameRule->show();
        this->hide();
        gameRule->resize(950,630);
    });
    connect(gameRule, &rule::chooseBack, this, [=](){
        gameRule->hide();
        this->show();
    });



}


Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap pixmap(":/background/img/first_bg.png");

    if (!pixmap.isNull()) {
        painter.drawPixmap(0, 0, this->width(), this->height(), pixmap);
    } else {
        qDebug() << "Failed to load image!";
    }
}

void Widget::onButtonClicked(){
    qDebug() << "Play button clicked!";
    // game* mainGame = new game();
    // //mainGame->show();
    // this->hide();
}

void Widget::onRuleButtonClicked()
{
    qDebug() << "Rule button clicked!";

}

