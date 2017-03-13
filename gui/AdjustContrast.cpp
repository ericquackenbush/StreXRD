#include "AdjustContrast.h"

AdjustContrast::AdjustContrast(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(set_contrast()));
}

AdjustContrast::~AdjustContrast()
{
}

void AdjustContrast::on_pushButton_clicked()
{
	this->close();
}

void AdjustContrast::set_contrast()
{
	this->contrast = ui.horizontalSlider->value();
}

int AdjustContrast::get_contrast()
{
	return this->contrast;
}