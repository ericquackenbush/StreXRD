#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QGraphicsScene>
#include <QMediaPlayer>
#include <QTimer>
#include <opencv2/opencv.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

	void resizeEvent(QResizeEvent * e);

    void on_actionExit_triggered();

    void on_actionAbout_triggered();

    void on_actionOpen_Image_triggered();

	void on_actionMetal_triggered();

	void on_actionZoom_In_triggered();

	void on_actionZoom_Out_triggered();

	void on_actionAdjust_Brightness_triggered();

	void on_actionAdjust_Contrast_triggered();

	void transform_image(int, int);

private:
    Ui::MainWindow *ui;

	// display an image with Graphics View
	QPixmap image;
	QGraphicsScene *scene;
	bool imageLoaded;

	// play music file
	QMediaPlayer* player;

};

#endif // MAINWINDOW_H
