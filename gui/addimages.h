#ifndef ADDIMAGES_H
#define ADDIMAGES_H

#include <QDialog>
#include <QFileSystemModel>
#include <QStringListModel>
#include <QDialogButtonBox>

namespace Ui {
class addImages;
}

class addImages : public QDialog
{
    Q_OBJECT

public:
    explicit addImages(QWidget *parent = 0);
    ~addImages();

private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::addImages *ui;
    QStringList files_to_be_added;
    QFileSystemModel *dirmodel;
    QStringListModel *listmodel;
    QDialogButtonBox *buttonBox;
};

#endif // ADDIMAGES_H
