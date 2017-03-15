#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <random>
#include <QMessageBox>
#include <QFileDialog>
#include <QMediaPlayer>
#include <QTimer>
#include <QFile>
#include <opencv2/opencv.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <image_object.h>

double currentScale;
QString filename;

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
    QMessageBox::information(this, "About StreXRD", "A program analyzing for stress-strain in powder diffraction images.");
}

void MainWindow::on_actionOpen_Image_triggered()
{
    filename = QFileDialog::getOpenFileName(
                this, tr("Open File"), "C://", "All files (*.*);;Image files (*.tiff);;MAR345 files (*.mar3450)");

	// set initial scale value
	currentScale = 1.0;

	// this only works on Windows!
	std::string cv_filename = filename.toLocal8Bit().constData();

	new_image.load_file(cv_filename);
	cv::Mat img = new_image.get_input_image();

	// convert cv::Mat to QImage
	QImage imageObject(img.data, img.cols, img.rows, static_cast<int>(img.step), QImage::Format_Grayscale8);

	// display the image
	image = QPixmap::fromImage(imageObject);

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
	double increase_scale = 1.1;
	ui->graphicsView->scale(increase_scale, increase_scale);
	currentScale = currentScale*increase_scale;
}

void MainWindow::on_actionZoom_Out_triggered()
{
	double decrease_scale = 0.9;
	ui->graphicsView->scale(decrease_scale, decrease_scale);
	currentScale = currentScale*decrease_scale;
}

void MainWindow::on_actionIncrease_Brightness_triggered()
{
	if (imageLoaded)
	{
		new_image.adjust_brightness(10);

		cv::Mat img = new_image.get_output_image();

		// convert cv::Mat to QImage
		QImage imageObject(img.data, img.cols, img.rows, static_cast<int>(img.step), QImage::Format_Grayscale8);

		// display the new image
		image = QPixmap::fromImage(imageObject);
		scene->clear();
		scene->addPixmap(image);
		scene->setSceneRect(image.rect());
		ui->graphicsView->setScene(scene);
		ui->graphicsView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
		ui->graphicsView->scale(currentScale, currentScale);
	}
}

void MainWindow::on_actionDecrease_Brightness_triggered()
{
	if (imageLoaded)
	{
		new_image.adjust_brightness(-10);

		cv::Mat img = new_image.get_output_image();

		// convert cv::Mat to QImage
		QImage imageObject(img.data, img.cols, img.rows, static_cast<int>(img.step), QImage::Format_Grayscale8);

		// display the new image
		image = QPixmap::fromImage(imageObject);
		scene->clear();
		scene->addPixmap(image);
		scene->setSceneRect(image.rect());
		ui->graphicsView->setScene(scene);
		ui->graphicsView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
		ui->graphicsView->scale(currentScale, currentScale);
	}
}


void MainWindow::on_actionIncrease_Contrast_triggered()
{
	if (imageLoaded)
	{
		new_image.adjust_contrast(1.1);

		cv::Mat img = new_image.get_output_image();

		// convert cv::Mat to QImage
		QImage imageObject(img.data, img.cols, img.rows, static_cast<int>(img.step), QImage::Format_Grayscale8);

		// display the new image
		image = QPixmap::fromImage(imageObject);
		scene->clear();
		scene->addPixmap(image);
		scene->setSceneRect(image.rect());
		ui->graphicsView->setScene(scene);
		ui->graphicsView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
		ui->graphicsView->scale(currentScale, currentScale);
	}
}

void MainWindow::on_actionDecrease_Contrast_triggered()
{
	if (imageLoaded)
	{
		new_image.adjust_contrast(0.9);

		cv::Mat img = new_image.get_output_image();

		// convert cv::Mat to QImage
		QImage imageObject(img.data, img.cols, img.rows, static_cast<int>(img.step), QImage::Format_Grayscale8);

		// display the new image
		image = QPixmap::fromImage(imageObject);
		scene->clear();
		scene->addPixmap(image);
		scene->setSceneRect(image.rect());
		ui->graphicsView->setScene(scene);
		ui->graphicsView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
		ui->graphicsView->scale(currentScale, currentScale);
	}
}



void MainWindow::on_actionOriginal_Image_triggered()
{
	if (imageLoaded)
	{
		cv::Mat img = new_image.get_input_image();

		// convert cv::Mat to QImage
		QImage imageObject(img.data, img.cols, img.rows, static_cast<int>(img.step), QImage::Format_Grayscale8);

		// display the new image
		image = QPixmap::fromImage(imageObject);
		scene->clear();
		scene->addPixmap(image);
		scene->setSceneRect(image.rect());
		ui->graphicsView->setScene(scene);
		ui->graphicsView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
		ui->graphicsView->scale(1.0, 1.0);
	}
}

void MainWindow::on_actionWeak_Peaks_triggered()
{
	if (imageLoaded)
	{
		if (filename.endsWith(".mar3450"))
		{
			new_image.weak_peaks();

			cv::Mat img = new_image.get_output_image();

			// convert cv::Mat to QImage
			QImage imageObject(img.data, img.cols, img.rows, static_cast<int>(img.step), QImage::Format_Grayscale8);

			// display the new image
			image = QPixmap::fromImage(imageObject);
			scene->clear();
			scene->addPixmap(image);
			scene->setSceneRect(image.rect());
			ui->graphicsView->setScene(scene);
			ui->graphicsView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
			ui->graphicsView->scale(1.0, 1.0);
		}
	}
}

void MainWindow::on_actionFully_Automatic_triggered()
{
	if (imageLoaded)
	{
		new_image.fully_automatic();

		cv::Mat img = new_image.get_output_image();

		// convert cv::Mat to QImage
		QImage imageObject(img.data, img.cols, img.rows, static_cast<int>(img.step), QImage::Format_Grayscale8);

		// display the new image
		image = QPixmap::fromImage(imageObject);
		scene->clear();
		scene->addPixmap(image);
		scene->setSceneRect(image.rect());
		ui->graphicsView->setScene(scene);
		ui->graphicsView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
		ui->graphicsView->scale(1.0, 1.0);
	}
}