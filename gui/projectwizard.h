#ifndef PROJECTWIZARD_H
#define PROJECTWIZARD_H

#include <QWizard>

QT_BEGIN_NAMESPACE
class QCheckBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QRadioButton;
QT_END_NAMESPACE

class ProjectWizard : public QWizard
{
    Q_OBJECT

public:
    ProjectWizard(QWidget *parent = 0);

    void accept() override;
};

class IntroPage : public QWizardPage
{
    Q_OBJECT

public:
    IntroPage(QWidget *parent = 0);

private:
    QLabel *label;
};

class ProjectInfoPage : public QWizardPage
{
    Q_OBJECT

public:
    ProjectInfoPage(QWidget *parent = 0);

private:
    QLabel *projectNameLabel;
    QLabel *projectDirectoryLabel;
    QLineEdit *projectNameLineEdit;
    QLineEdit *projectDirectoryLineEdit;
    QCheckBox *defaultDirectoryCheckBox;
};

class ConclusionPage : public QWizardPage
{
    Q_OBJECT

public:
    ConclusionPage(QWidget *parent = 0);

protected:
    void initializePage() override;

private:
    QLabel *label;
};

#endif
