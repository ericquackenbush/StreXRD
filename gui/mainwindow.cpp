#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QMediaPlayer>
#include <QTimer>
#include <QFile>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <AdjustBrightness.h>
#include <AdjustContrast.h>

using myType = uint32_t;

cv::Mat inputImage;
cv::Mat new_image;

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

std::string type2str(int type)
{
	std::string r;

	uchar depth = type & CV_MAT_DEPTH_MASK;
	uchar chans = 1 + (type >> CV_CN_SHIFT);

	switch (depth) {
	case CV_8U:  r = "8U"; break;
	case CV_8S:  r = "8S"; break;
	case CV_16U: r = "16U"; break;
	case CV_16S: r = "16S"; break;
	case CV_32S: r = "32S"; break;
	case CV_32F: r = "32F"; break;
	case CV_64F: r = "64F"; break;
	default:     r = "User"; break;
	}

	r += "C";
	r += (chans + '0');

	return r;
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

	// this only works on Windows!
	std::string cv_filename = filename.toLocal8Bit().constData();

	std::ifstream is;
	std::vector<myType> rawfilebuffer;

	is.open(cv_filename, std::ios::binary);
	is.seekg(0, std::ios::end);
	size_t filesize = is.tellg();
	is.seekg(0, std::ios::beg);

	rawfilebuffer.resize(filesize / sizeof(myType));

	is.read((char *)rawfilebuffer.data(), filesize);

	//for (auto const & ui : rawfilebuffer)
	//	std::cout << '[' << ui << ']';

	//std::cout << '\n';

	cv::Mat m = cv::Mat(3450, 3450, CV_32S);
	memcpy(m.data, rawfilebuffer.data(), rawfilebuffer.size() * sizeof(myType));
	m.convertTo(inputImage, CV_8UC1);
	//cv::imshow("Display image", inputImage);

	//qDebug() << "the number should be: " << rawfilebuffer[247115];

	//cv::Mat inputImage = cv::imread(cv_filename);
	//if (inputImage.empty())
	//{
	//	qDebug() << "Failed to read image!";
	//}
	//else
	//{
	//	qDebug() << "Loaded image successfully!";
	//}
	//std::string ty = type2str(inputImage.type());
	//qDebug("Matrix: %s %dx%d \n", ty.c_str(), inputImage.cols, inputImage.rows);
	//cv::imshow("Display image", inputImage);

	// convert cv::Mat to QImage
	QImage imageObject(inputImage.data, inputImage.cols, inputImage.rows, static_cast<int>(inputImage.step), QImage::Format_Grayscale8);
	//imageObject = imageObject.rgbSwapped();

	// display the image
	//imageObject->load(filename);
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
	ui->graphicsView->scale(1.1, 1.1);
}

void MainWindow::on_actionZoom_Out_triggered()
{
	ui->graphicsView->scale(0.9, 0.9);
}

void MainWindow::on_actionAdjust_Brightness_triggered()
{
	if (imageLoaded)
	{
		AdjustBrightness* myDialog1 = new AdjustBrightness(this);
		myDialog1->setModal(true);
		myDialog1->exec();
		int brightness = myDialog1->get_brightness();
		this->transform_image(0, brightness);
	}
}

void MainWindow::on_actionAdjust_Contrast_triggered()
{
	if (imageLoaded)
	{
		AdjustContrast* myDialog2 = new AdjustContrast(this);
		myDialog2->setModal(true);
		myDialog2->exec();
		int contrast = myDialog2->get_contrast();
		this->transform_image(contrast, 0);
	}
}

void MainWindow::transform_image(int a, int b)
{
	int num = a;
	int beta = b;

	double alpha = 2.0*double(num) / 100.0 + 1.0;

	cv::Mat old_image = inputImage;
	new_image = cv::Mat::zeros(old_image.size(), old_image.type());

	for (int y = 0; y < old_image.rows; y++)
	{
		for (int x = 0; x < old_image.cols; x++)
		{
			new_image.at<uchar>(y, x) = cv::saturate_cast<uchar>(alpha*(old_image.at<uchar>(y, x)) + beta);
		}
	}

	// convert cv::Mat to QImage
	QImage imageObject(new_image.data, new_image.cols, new_image.rows, static_cast<int>(new_image.step), QImage::Format_Grayscale8);

	// display the new image
	image = QPixmap::fromImage(imageObject);
	scene->clear();
	scene->addPixmap(image);
	scene->setSceneRect(image.rect());
	ui->graphicsView->setScene(scene);
	ui->graphicsView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}