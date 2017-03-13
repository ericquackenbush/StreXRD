#pragma once

#include <QDialog>
#include "ui_AdjustBrightness.h"

class AdjustBrightness : public QDialog
{
	Q_OBJECT

public:
	AdjustBrightness(QWidget *parent = Q_NULLPTR);
	~AdjustBrightness();

	int get_brightness();

private slots:
	
	void on_pushButton_clicked();

	void set_brightness();

private:
	Ui::AdjustBrightness ui;
	int brightness;
};
