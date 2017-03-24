#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QGraphicsScene>
#include <QMediaPlayer>
#include <QTimer>
#include <QTreeWidget>
#include <opencv2/opencv.hpp>
#include <image_object.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:

private slots:

	void resizeEvent(QResizeEvent * e);

    void on_actionExit_triggered();

    void on_actionAbout_triggered();

    void on_actionOpen_Image_triggered();

	void on_actionMetal_triggered();

	void on_actionZoom_In_triggered();

	void on_actionZoom_Out_triggered();

	void on_actionIncrease_Brightness_triggered();

	void on_actionDecrease_Brightness_triggered();

	void on_actionIncrease_Contrast_triggered();

	void on_actionDecrease_Contrast_triggered();

	void on_actionOriginal_Image_triggered();

	void on_actionWeak_Peaks_triggered();

	void on_actionFully_Automatic_triggered();

	void on_actionOpen_existing_project_triggered();

    void on_actionAdd_images_to_project_triggered();

	void change_image(QTreeWidgetItem*, int);

private:
    Ui::MainWindow *ui;

	// display an image with Graphics View
	QPixmap image;
	QImage imageObject;
	QGraphicsScene *scene;
	bool imageLoaded;

	// play music file
	QMediaPlayer* player;

	ImageObject new_image;

	QTreeWidgetItem *itm;

	QHash<QString, QString> file_names_locations;
	void update_image(cv::Mat img);
};

#endif // MAINWINDOW_H
