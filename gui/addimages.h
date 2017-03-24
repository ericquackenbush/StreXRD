#ifndef ADDIMAGES_H
#define ADDIMAGES_H

#include <QDialog>
#include <QFileSystemModel>
#include <QStringListModel>

namespace Ui {
class addImages;
}

class addImages : public QDialog
{
    Q_OBJECT

public:
    explicit addImages(QWidget *parent = 0);
    ~addImages();
    void set_xml_filename(const QString &path);

private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::addImages *ui;
    QStringList files_to_be_added;
    QFileSystemModel *dirmodel;
    QStringListModel *listmodel;
    QString m_path;
};

#endif // ADDIMAGES_H
