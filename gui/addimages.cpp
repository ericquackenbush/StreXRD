#include <QFileSystemModel>
#include <QDebug>
#include <QModelIndex>
#include <QStringListModel>
#include <QList>
#include <xml_handler.h>
#include "addimages.h"
#include "ui_addimages.h"

addImages::addImages(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addImages)
{
    ui->setupUi(this);

    QString sPath = "/";
    dirmodel = new QFileSystemModel(this);
    dirmodel->setRootPath(sPath);
    ui->treeView->setModel(dirmodel);
    ui->treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->treeView->expandAll();

    listmodel = new QStringListModel(this);
    listmodel->setStringList(files_to_be_added);
    ui->listView->setModel(listmodel);
    ui->listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
}

addImages::~addImages()
{
    delete ui;
}

void addImages::on_pushButton_3_clicked()
{
    // add images
    QModelIndexList selected = ui->treeView->selectionModel()->selectedIndexes();
    QString filename;
    int n = 0;
    for (int i = 0; i < selected.size(); i+=4)
    {
    	filename = dirmodel->fileInfo(selected[i]).absoluteFilePath();
    	// check for duplicate values
    	if (!files_to_be_added.contains(filename))
    	{
    		files_to_be_added.append(filename);
    		n++;
    	}
    }
    listmodel->setStringList(files_to_be_added);
    ui->treeView->clearSelection();
}

void addImages::on_pushButton_4_clicked()
{
	// delete images
	QModelIndexList selected = ui->listView->selectionModel()->selectedIndexes();
	QString filename;

	for (int i = 0; i < selected.size(); i++)
    {
    	filename = selected[i].data().toString();
    	files_to_be_added.removeAll(filename);
    }

	qSort(selected.begin(), selected.end(), qGreater<QModelIndex>());
	for (QModelIndexList::const_iterator iter = selected.constBegin(); iter != selected.constEnd(); ++iter)
	{
		listmodel->removeRow((*iter).row(), (*iter).parent());
	}
}
void addImages::on_pushButton_clicked()
{
    // OK
    this->accept();

    //write files_to_be_added to xml file
    XMLHandler project_file(m_path);
    project_file.read_xml_file();
    project_file.add_files(files_to_be_added);

    close();
}

void addImages::on_pushButton_2_clicked()
{
    // Cancel
    this->reject();
    close();
}

void addImages::set_xml_filename(const QString &path)
{
    m_path = path;
}
