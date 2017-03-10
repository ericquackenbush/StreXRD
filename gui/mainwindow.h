#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QGraphicsScene>
#include <QMediaPlayer>

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

    void on_actionExit_triggered();

    void on_actionAbout_triggered();

    void on_actionOpen_Image_triggered();

	void on_actionMetal_triggered();

private:
    Ui::MainWindow *ui;

	// display an image with Graphics View
	QPixmap image;
	QImage  *imageObject;
	QGraphicsScene *scene;

	// play music file
	QMediaPlayer* player;

};

#endif // MAINWINDOW_H
