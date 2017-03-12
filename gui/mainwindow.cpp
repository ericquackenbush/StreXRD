#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QMediaPlayer>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //ui->splitter->setStretchFactor(0,1);
    QList<int> hSplitSizes;
    hSplitSizes << 200 << 800;
    ui->splitter->setSizes(hSplitSizes);

	player = new QMediaPlayer(this);

	// determines whether image is loaded for window resize
	imageLoaded = false;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *e)
{
	if (imageLoaded)
	{
		ui->graphicsView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
	}
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

	// display the image
	imageObject = new QImage();
	imageObject->load(filename);

	image = QPixmap::fromImage(*imageObject);

	scene = new QGraphicsScene(this);
	scene->addPixmap(image);
	scene->setSceneRect(image.rect());
	ui->graphicsView->setScene(scene);
	ui->graphicsView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
	imageLoaded = true;
}

void MainWindow::on_actionMetal_triggered()
{
	ui->statusBar->showMessage("Metal mode is now activated!", 2000);
	// play music
	player->setMedia(QUrl::fromLocalFile(QFileInfo("music/song.mp3").absoluteFilePath()));
	player->setVolume(50);
	player->play();
	qDebug() << QDir::currentPath();
	qDebug() << player->errorString();
}

void MainWindow::on_actionZoom_In_triggered()
{
	ui->graphicsView->scale(1.1, 1.1);
}

void MainWindow::on_actionZoom_Out_triggered()
{
	ui->graphicsView->scale(0.9, 0.9);
}
