/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionCreate_new_data_set;
    QAction *actionOpen_existing_dataset;
    QAction *actionPreferences;
    QAction *actionExit;
    QAction *actionEnter_calibration_file;
    QAction *actionLoad_standard_diffraction_file;
    QAction *actionSelect_standard_from_database;
    QAction *actionPerform_calibration;
    QAction *actionView_calibration_parameters;
    QAction *actionFind_Center;
    QAction *actionAzimuthal_Integration;
    QAction *actionFit_Circles;
    QAction *actionFit_Ellipses;
    QAction *actionTake_Slices;
    QAction *actionDocumentation;
    QAction *actionAbout;
    QAction *actionMulti_Analysis;
    QAction *actionOpen_Image;
    QAction *actionAdd_images_to_existing_project;
    QAction *actionZoom_In;
    QAction *actionZoom_Out;
    QAction *actionMetal;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QSplitter *splitter;
    QTreeWidget *treeWidget;
    QTabWidget *tabWidget;
    QWidget *tab;
    QHBoxLayout *horizontalLayout_2;
    QSplitter *splitter_2;
    QGraphicsView *graphicsView;
    QGroupBox *groupBox;
    QWidget *tab_2;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuCalibration;
    QMenu *menuAnalysis;
    QMenu *menuHelp;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(809, 496);
        actionCreate_new_data_set = new QAction(MainWindow);
        actionCreate_new_data_set->setObjectName(QStringLiteral("actionCreate_new_data_set"));
        actionOpen_existing_dataset = new QAction(MainWindow);
        actionOpen_existing_dataset->setObjectName(QStringLiteral("actionOpen_existing_dataset"));
        actionPreferences = new QAction(MainWindow);
        actionPreferences->setObjectName(QStringLiteral("actionPreferences"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionEnter_calibration_file = new QAction(MainWindow);
        actionEnter_calibration_file->setObjectName(QStringLiteral("actionEnter_calibration_file"));
        actionLoad_standard_diffraction_file = new QAction(MainWindow);
        actionLoad_standard_diffraction_file->setObjectName(QStringLiteral("actionLoad_standard_diffraction_file"));
        actionSelect_standard_from_database = new QAction(MainWindow);
        actionSelect_standard_from_database->setObjectName(QStringLiteral("actionSelect_standard_from_database"));
        actionPerform_calibration = new QAction(MainWindow);
        actionPerform_calibration->setObjectName(QStringLiteral("actionPerform_calibration"));
        actionView_calibration_parameters = new QAction(MainWindow);
        actionView_calibration_parameters->setObjectName(QStringLiteral("actionView_calibration_parameters"));
        actionFind_Center = new QAction(MainWindow);
        actionFind_Center->setObjectName(QStringLiteral("actionFind_Center"));
        actionAzimuthal_Integration = new QAction(MainWindow);
        actionAzimuthal_Integration->setObjectName(QStringLiteral("actionAzimuthal_Integration"));
        actionFit_Circles = new QAction(MainWindow);
        actionFit_Circles->setObjectName(QStringLiteral("actionFit_Circles"));
        actionFit_Ellipses = new QAction(MainWindow);
        actionFit_Ellipses->setObjectName(QStringLiteral("actionFit_Ellipses"));
        actionTake_Slices = new QAction(MainWindow);
        actionTake_Slices->setObjectName(QStringLiteral("actionTake_Slices"));
        actionDocumentation = new QAction(MainWindow);
        actionDocumentation->setObjectName(QStringLiteral("actionDocumentation"));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionMulti_Analysis = new QAction(MainWindow);
        actionMulti_Analysis->setObjectName(QStringLiteral("actionMulti_Analysis"));
        actionOpen_Image = new QAction(MainWindow);
        actionOpen_Image->setObjectName(QStringLiteral("actionOpen_Image"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/icons/document-open-5.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen_Image->setIcon(icon);
        actionAdd_images_to_existing_project = new QAction(MainWindow);
        actionAdd_images_to_existing_project->setObjectName(QStringLiteral("actionAdd_images_to_existing_project"));
        actionZoom_In = new QAction(MainWindow);
        actionZoom_In->setObjectName(QStringLiteral("actionZoom_In"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icons/icons/zoom-in-5.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionZoom_In->setIcon(icon1);
        actionZoom_Out = new QAction(MainWindow);
        actionZoom_Out->setObjectName(QStringLiteral("actionZoom_Out"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/icons/icons/zoom-out-5.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionZoom_Out->setIcon(icon2);
        actionMetal = new QAction(MainWindow);
        actionMetal->setObjectName(QStringLiteral("actionMetal"));
        actionMetal->setCheckable(false);
        actionMetal->setChecked(false);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QStringLiteral("splitter"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(splitter->sizePolicy().hasHeightForWidth());
        splitter->setSizePolicy(sizePolicy);
        splitter->setMinimumSize(QSize(0, 322));
        splitter->setOrientation(Qt::Horizontal);
        treeWidget = new QTreeWidget(splitter);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(treeWidget->sizePolicy().hasHeightForWidth());
        treeWidget->setSizePolicy(sizePolicy1);
        treeWidget->setMinimumSize(QSize(0, 0));
        treeWidget->setMaximumSize(QSize(250, 16777215));
        splitter->addWidget(treeWidget);
        tabWidget = new QTabWidget(splitter);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        sizePolicy1.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy1);
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        horizontalLayout_2 = new QHBoxLayout(tab);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        splitter_2 = new QSplitter(tab);
        splitter_2->setObjectName(QStringLiteral("splitter_2"));
        splitter_2->setOrientation(Qt::Vertical);
        graphicsView = new QGraphicsView(splitter_2);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        splitter_2->addWidget(graphicsView);
        groupBox = new QGroupBox(splitter_2);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        splitter_2->addWidget(groupBox);

        horizontalLayout_2->addWidget(splitter_2);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        tabWidget->addTab(tab_2, QString());
        splitter->addWidget(tabWidget);

        horizontalLayout->addWidget(splitter);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 809, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuCalibration = new QMenu(menuBar);
        menuCalibration->setObjectName(QStringLiteral("menuCalibration"));
        menuAnalysis = new QMenu(menuBar);
        menuAnalysis->setObjectName(QStringLiteral("menuAnalysis"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuCalibration->menuAction());
        menuBar->addAction(menuAnalysis->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionCreate_new_data_set);
        menuFile->addAction(actionOpen_existing_dataset);
        menuFile->addAction(actionAdd_images_to_existing_project);
        menuFile->addAction(actionPreferences);
        menuFile->addAction(actionMetal);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuCalibration->addAction(actionEnter_calibration_file);
        menuCalibration->addAction(actionLoad_standard_diffraction_file);
        menuCalibration->addAction(actionSelect_standard_from_database);
        menuCalibration->addAction(actionPerform_calibration);
        menuCalibration->addAction(actionView_calibration_parameters);
        menuAnalysis->addAction(actionFind_Center);
        menuAnalysis->addAction(actionAzimuthal_Integration);
        menuAnalysis->addAction(actionFit_Circles);
        menuAnalysis->addAction(actionFit_Ellipses);
        menuAnalysis->addAction(actionTake_Slices);
        menuAnalysis->addSeparator();
        menuAnalysis->addAction(actionMulti_Analysis);
        menuHelp->addAction(actionDocumentation);
        menuHelp->addSeparator();
        menuHelp->addAction(actionAbout);
        mainToolBar->addAction(actionOpen_Image);
        mainToolBar->addAction(actionZoom_In);
        mainToolBar->addAction(actionZoom_Out);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "StreXRD", Q_NULLPTR));
        actionCreate_new_data_set->setText(QApplication::translate("MainWindow", "Create new project", Q_NULLPTR));
        actionOpen_existing_dataset->setText(QApplication::translate("MainWindow", "Open existing project", Q_NULLPTR));
        actionPreferences->setText(QApplication::translate("MainWindow", "Preferences", Q_NULLPTR));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", Q_NULLPTR));
        actionEnter_calibration_file->setText(QApplication::translate("MainWindow", "Open calibration file", Q_NULLPTR));
        actionLoad_standard_diffraction_file->setText(QApplication::translate("MainWindow", "Load standard diffraction file", Q_NULLPTR));
        actionSelect_standard_from_database->setText(QApplication::translate("MainWindow", "Select standard from database", Q_NULLPTR));
        actionPerform_calibration->setText(QApplication::translate("MainWindow", "Perform calibration", Q_NULLPTR));
        actionView_calibration_parameters->setText(QApplication::translate("MainWindow", "View calibration parameters", Q_NULLPTR));
        actionFind_Center->setText(QApplication::translate("MainWindow", "Find Center", Q_NULLPTR));
        actionAzimuthal_Integration->setText(QApplication::translate("MainWindow", "Azimuthal Integration", Q_NULLPTR));
        actionFit_Circles->setText(QApplication::translate("MainWindow", "Fit Circles", Q_NULLPTR));
        actionFit_Ellipses->setText(QApplication::translate("MainWindow", "Fit Ellipses", Q_NULLPTR));
        actionTake_Slices->setText(QApplication::translate("MainWindow", "Take Slices", Q_NULLPTR));
        actionDocumentation->setText(QApplication::translate("MainWindow", "Documentation", Q_NULLPTR));
        actionAbout->setText(QApplication::translate("MainWindow", "About", Q_NULLPTR));
        actionMulti_Analysis->setText(QApplication::translate("MainWindow", "Multi-Analysis", Q_NULLPTR));
        actionOpen_Image->setText(QApplication::translate("MainWindow", "Open Image", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionOpen_Image->setToolTip(QApplication::translate("MainWindow", "Open Image", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionAdd_images_to_existing_project->setText(QApplication::translate("MainWindow", "Add images to existing project", Q_NULLPTR));
        actionZoom_In->setText(QApplication::translate("MainWindow", "Zoom In", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionZoom_In->setToolTip(QApplication::translate("MainWindow", "Zoom In", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionZoom_Out->setText(QApplication::translate("MainWindow", "Zoom Out", Q_NULLPTR));
        actionMetal->setText(QApplication::translate("MainWindow", "Metal Mode", Q_NULLPTR));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("MainWindow", "Project Explorer", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("MainWindow", "View Options", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "Main", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "Tab 2", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", Q_NULLPTR));
        menuCalibration->setTitle(QApplication::translate("MainWindow", "Calibration", Q_NULLPTR));
        menuAnalysis->setTitle(QApplication::translate("MainWindow", "Analysis", Q_NULLPTR));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
