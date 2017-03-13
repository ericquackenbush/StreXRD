#include "AdjustBrightness.h"

AdjustBrightness::AdjustBrightness(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(set_brightness()));
}

AdjustBrightness::~AdjustBrightness()
{
}

void AdjustBrightness::on_pushButton_clicked()
{
	this->close();
}

void AdjustBrightness::set_brightness()
{
	this->brightness = ui.horizontalSlider->value();
}

int AdjustBrightness::get_brightness()
{
	return this->brightness;
}