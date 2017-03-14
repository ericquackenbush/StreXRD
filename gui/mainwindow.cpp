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
#include <image.h>

using myType = uint32_t;

cv::Mat inputImage;
cv::Mat old_image;
cv::Mat new_image;
double currentScale;

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

	// set initial scale value
	currentScale = 1.0;

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

	if (filename.endsWith(".mar3450"))
	{
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

		cv::Mat m = cv::Mat(3450, 3450, CV_32S);
		memcpy(m.data, rawfilebuffer.data(), rawfilebuffer.size() * sizeof(myType));
		m.convertTo(inputImage, CV_8UC1);
		old_image = inputImage;

		// convert cv::Mat to QImage
		QImage imageObject(inputImage.data, inputImage.cols, inputImage.rows, static_cast<int>(inputImage.step), QImage::Format_Grayscale8);
	}
	else
	{
		QImage imageObject(filename);
	}

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
		adjust_brightness(10);
	}
}

void MainWindow::on_actionDecrease_Brightness_triggered()
{
	if (imageLoaded)
	{
		adjust_brightness(-10);
	}
}


void MainWindow::on_actionIncrease_Contrast_triggered()
{
	if (imageLoaded)
	{
		adjust_contrast(1.1);
	}
}

void MainWindow::on_actionDecrease_Contrast_triggered()
{
	if (imageLoaded)
	{
		adjust_contrast(0.9);
	}

}

void MainWindow::adjust_brightness(int a)
{
	int new_brightness = a;
	cv::Mat new_image = old_image + cv::Scalar(new_brightness);
	old_image = new_image;

	// convert cv::Mat to QImage
	QImage imageObject(new_image.data, new_image.cols, new_image.rows, static_cast<int>(new_image.step), QImage::Format_Grayscale8);

	// display the new image
	image = QPixmap::fromImage(imageObject);
	scene->clear();
	scene->addPixmap(image);
	scene->setSceneRect(image.rect());
	ui->graphicsView->setScene(scene);
	ui->graphicsView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
	ui->graphicsView->scale(currentScale, currentScale);
}

void MainWindow::adjust_contrast(double a)
{
	double new_contrast = a;
	cv::Mat new_image;
	old_image.convertTo(new_image, -1, new_contrast, 0);
	old_image = new_image;

	// convert cv::Mat to QImage
	QImage imageObject(new_image.data, new_image.cols, new_image.rows, static_cast<int>(new_image.step), QImage::Format_Grayscale8);

	// display the new image
	image = QPixmap::fromImage(imageObject);
	scene->clear();
	scene->addPixmap(image);
	scene->setSceneRect(image.rect());
	ui->graphicsView->setScene(scene);
	ui->graphicsView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
	ui->graphicsView->scale(currentScale, currentScale);
}

void MainWindow::on_actionOriginal_Image_triggered()
{
	if (imageLoaded)
	{
		// convert cv::Mat to QImage
		QImage imageObject(inputImage.data, inputImage.cols, inputImage.rows, static_cast<int>(inputImage.step), QImage::Format_Grayscale8);

		// display the new image
		image = QPixmap::fromImage(imageObject);
		scene->clear();
		scene->addPixmap(image);
		scene->setSceneRect(image.rect());
		ui->graphicsView->setScene(scene);
		ui->graphicsView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
	}
}

void MainWindow::on_actionWeak_Peaks_triggered()
{
	if (imageLoaded)
	{
		int width = inputImage.cols;
		int height = inputImage.rows;
		int numPts = 100;
		int radius = 10;
		double minI;
		double maxI;

		std::vector<int> x(numPts);
		std::vector<int> y(numPts);
		std::vector<double> avg(numPts);
		std::vector<double> sd(numPts);

		std::random_device rd;
		std::mt19937_64 generator( rd() );
		std::uniform_int_distribution<> dist( 1, width - 1 );
		for (int i = 0; i < numPts; ++i)
		{
			x[i] = dist(generator);
			y[i] = dist(generator);

		}

		for (int i = 0; i < numPts; ++i)
		{
			std::vector<int> values;

			cv::Mat mask = cv::Mat::zeros(height, width, CV_8UC1);
			cv::Point center = cv::Point(x[i], y[i]);
			cv::circle(mask, center, radius, cv::Scalar(255), -1);
			for (unsigned int ix = 0; ix < height; ++ix)
			{
				for (unsigned int iy = 0; iy < width; ++iy)
				{
					if (mask.at<unsigned char>(iy, ix) > 0)
					{
						values.push_back(inputImage.at<unsigned char>(iy, ix));
					}
				}
			}

			// perform average
			double sum = std::accumulate(values.begin(), values.end(), 0.0);
			avg[i] = sum / values.size();

			// find standard deviation
			double sq_sum = std::inner_product(values.begin(), values.end(), values.begin(), 0.0);
			sd[i] = std::sqrt(sq_sum / values.size() - avg[i] * avg[i]);

			values.clear();
		}

		// find minimum and maximum values and their respective positions
		int min_pos = 0;
		int max_pos = 0;
		for (unsigned i = 0; i < avg.size(); ++i)
		{
			if (avg[i] < avg[min_pos])
				min_pos = i;
			if (avg[i] > avg[max_pos])
				max_pos = i;
		}

		minI = (avg[min_pos] - 3 * sd[min_pos]) / 65535;
		maxI = (avg[max_pos] + 5 * sd[max_pos]) / 65535;

		cv::Mat1b adjusted_image;
		cv::Vec2i in = cv::Vec2i((int)round(minI * 255), ((int)round(maxI * 255)));
		cv::Vec2i out = cv::Vec2i(0, 255);
		int tol = 1;
		Image::imadjust(inputImage, adjusted_image, tol, in, out);

		// convert cv::Mat to QImage
		QImage imageObject(adjusted_image.data, adjusted_image.cols, adjusted_image.rows, static_cast<int>(adjusted_image.step), QImage::Format_Grayscale8);

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

void MainWindow::on_actionFully_Automatic_triggered()
{
	if (imageLoaded)
	{
		double min;
		double max;
		cv::minMaxIdx(inputImage, &min, &max);
		cv::Mat adjMap;
		// Histogram Equalization
		float scale = 255 / (max - min);
		inputImage.convertTo(adjMap, CV_8UC1, scale, -min*scale);

		applyColorMap(adjMap, new_image, cv::COLORMAP_AUTUMN);

		// convert cv::Mat to QImage
		QImage imageObject = QImage((uchar*)new_image.data, new_image.cols, new_image.rows, new_image.step, QImage::Format_RGB888);

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