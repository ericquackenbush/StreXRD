#include <QtWidgets>
#include <QDebug>

#include "projectwizard.h"

//! [0] //! [1]
ProjectWizard::ProjectWizard(QWidget *parent)
    : QWizard(parent)
{
    addPage(new IntroPage);
    addPage(new ProjectInfoPage);
    addPage(new ConclusionPage);

    setWindowTitle(tr("Project Wizard"));
}

void ProjectWizard::accept()
{
    qDebug() << "Accepted";

    QDialog::accept();
}

IntroPage::IntroPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Introduction"));

    label = new QLabel(tr("This wizard will generate a StreXRD project file."));
    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);
}

ProjectInfoPage::ProjectInfoPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Project Information"));
    setSubTitle(tr("Specify basic information about the project."));

    projectNameLabel = new QLabel(tr("&Project name:"));
    projectNameLineEdit = new QLineEdit;
    projectNameLabel->setBuddy(projectNameLineEdit);

    projectDirectoryLabel = new QLabel(tr("&Project directory:"));
    projectDirectoryLineEdit = new QLineEdit;
    projectDirectoryLabel->setBuddy(projectDirectoryLineEdit);

    defaultDirectoryCheckBox = new QCheckBox(tr("Default Directory"));

    registerField("projectName*", projectNameLineEdit);
    registerField("projectDirectory", projectDirectoryLineEdit);
    registerField("defaultDirectoryChecked", defaultDirectoryCheckBox);


    QGridLayout *layout = new QGridLayout;
    layout->addWidget(projectNameLabel, 0, 0);
    layout->addWidget(projectNameLineEdit, 0, 1);
    layout->addWidget(projectDirectoryLabel, 1, 0);
    layout->addWidget(projectDirectoryLineEdit, 1, 1);
    layout->addWidget(defaultDirectoryCheckBox, 2, 0, 1, 2);
    setLayout(layout);
}

ConclusionPage::ConclusionPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Conclusion"));

    label = new QLabel;
    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);
}

void ConclusionPage::initializePage()
{
    QString finishText = wizard()->buttonText(QWizard::FinishButton);
    finishText.remove('&');
    label->setText(tr("Click %1 to generate the project file.").arg(finishText));
}