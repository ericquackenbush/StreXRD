#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //ui->splitter->setStretchFactor(0,1);
    QList<int> hSplitSizes;
    hSplitSizes << 200 << 800;
    ui->splitter->setSizes(hSplitSizes);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
     QCoreApplication::exit();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this, "About StreXRD", "A program analyzing stress-strain in powder diffraction images.");
}

void MainWindow::on_actionOpen_Image_triggered()
{
    QString filename = QFileDialog::getOpenFileName(
                this, tr("Open File"), "C://", "All files (*.*);;Image files (*.tiff);;MAR345 files (*.mar3450)");
}
