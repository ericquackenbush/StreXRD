/********************************************************************************
** Form generated from reading UI file 'newproject.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWPROJECT_H
#define UI_NEWPROJECT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_newproject
{
public:

    void setupUi(QDialog *newproject)
    {
        if (newproject->objectName().isEmpty())
            newproject->setObjectName(QStringLiteral("newproject"));
        newproject->resize(400, 300);

        retranslateUi(newproject);

        QMetaObject::connectSlotsByName(newproject);
    } // setupUi

    void retranslateUi(QDialog *newproject)
    {
        newproject->setWindowTitle(QApplication::translate("newproject", "Dialog", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class newproject: public Ui_newproject {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWPROJECT_H
