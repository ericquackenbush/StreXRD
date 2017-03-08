#ifndef NEWPROJECT_H
#define NEWPROJECT_H

#include <QDialog>

namespace Ui {
class newproject;
}

class newproject : public QDialog
{
    Q_OBJECT

public:
    explicit newproject(QWidget *parent = 0);
    ~newproject();

private:
    Ui::newproject *ui;
};

#endif // NEWPROJECT_H
