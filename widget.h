#ifndef WIDGET_H
#define WIDGET_H

#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QPushButton>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QUrl>
#include <QVBoxLayout>
#include <QStackedWidget>
#include "game.h"
#include "musicmanager.h"
#include "rule.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void paintEvent(QPaintEvent* event);
    void onButtonClicked();
    void onRuleButtonClicked();

private:
    Ui::Widget *ui;
    QMediaPlayer *player;
    QAudioOutput *audioOutput;
};
#endif // WIDGET_H
