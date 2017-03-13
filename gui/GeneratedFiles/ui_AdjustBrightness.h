/********************************************************************************
** Form generated from reading UI file 'AdjustBrightness.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADJUSTBRIGHTNESS_H
#define UI_ADJUSTBRIGHTNESS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AdjustBrightness
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QSlider *horizontalSlider;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer;

    void setupUi(QDialog *AdjustBrightness)
    {
        if (AdjustBrightness->objectName().isEmpty())
            AdjustBrightness->setObjectName(QStringLiteral("AdjustBrightness"));
        AdjustBrightness->resize(269, 79);
        layoutWidget = new QWidget(AdjustBrightness);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(11, 11, 251, 55));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSlider = new QSlider(layoutWidget);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setMaximum(100);
        horizontalSlider->setValue(0);
        horizontalSlider->setSliderPosition(0);
        horizontalSlider->setOrientation(Qt::Horizontal);

        verticalLayout->addWidget(horizontalSlider);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        pushButton = new QPushButton(layoutWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout->addWidget(pushButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(AdjustBrightness);

        QMetaObject::connectSlotsByName(AdjustBrightness);
    } // setupUi

    void retranslateUi(QDialog *AdjustBrightness)
    {
        AdjustBrightness->setWindowTitle(QApplication::translate("AdjustBrightness", "Adjust Brightness", Q_NULLPTR));
        pushButton->setText(QApplication::translate("AdjustBrightness", "Ok", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AdjustBrightness: public Ui_AdjustBrightness {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADJUSTBRIGHTNESS_H
