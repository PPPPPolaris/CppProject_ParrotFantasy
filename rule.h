#ifndef RULE_H
#define RULE_H

#include <QWidget>
#include <QStackedWidget>
#include <QPainter>
#include <QPushButton>

namespace Ui {
class rule;
}

class rule : public QWidget
{
    Q_OBJECT

public:
    explicit rule(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent* event);
    ~rule();

signals:
    void chooseBack();


private:
    Ui::rule *ui;
};

#endif // RULE_H
