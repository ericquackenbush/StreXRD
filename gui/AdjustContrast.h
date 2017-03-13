#pragma once

#include <QDialog>
#include "ui_AdjustContrast.h"

class AdjustContrast : public QDialog
{
	Q_OBJECT

public:
	AdjustContrast(QWidget *parent = Q_NULLPTR);
	~AdjustContrast();

	int get_contrast();

private slots:

	void on_pushButton_clicked();

	void set_contrast();

private:
	Ui::AdjustContrast ui;
	int contrast;
};
