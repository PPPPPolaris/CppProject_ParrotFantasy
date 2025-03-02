/********************************************************************************
** Form generated from reading UI file 'rule.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RULE_H
#define UI_RULE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_rule
{
public:

    void setupUi(QWidget *rule)
    {
        if (rule->objectName().isEmpty())
            rule->setObjectName("rule");
        rule->resize(400, 300);

        retranslateUi(rule);

        QMetaObject::connectSlotsByName(rule);
    } // setupUi

    void retranslateUi(QWidget *rule)
    {
        rule->setWindowTitle(QCoreApplication::translate("rule", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class rule: public Ui_rule {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RULE_H
