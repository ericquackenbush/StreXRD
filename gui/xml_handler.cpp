#include <QFileDialog>
#include <QDomDocument>
#include <QList>
#include <QDebug>
#include <xml_handler.h>

XMLHandler::XMLHandler(QString filename)
{
	this->filename = filename;
}

QList<int> XMLHandler::get_file_ID()
{
	return file_ID_list;
}

QList<QString> XMLHandler::get_file_names()
{
	return file_name_list;
}

QList<QString> XMLHandler::get_file_locations()
{
	return file_location_list;
}

QString XMLHandler::get_project_name()
{
	return project_name;
}

QHash<QString, QString> XMLHandler::get_hash()
{
	return file_names_locations;
}

void XMLHandler::add_files(QList<QString> file_list)
{
	QDomDocument doc;
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly))
	    return;
	if (!doc.setContent(&file)) {
	    file.close();
	    return;
	}
	file.close();

	QDomElement docElem = doc.documentElement();
	QDomNode n = docElem.firstChild();
	QDomElement files_tag;
	while(!n.isNull()) {
	    QDomElement e = n.toElement();
	    if(!e.isNull() && e.tagName() == "Files") {
	        files_tag = e;
	    }
	    n = n.nextSibling();
	}

	// get file ID of last file element
	QDomElement last_file_element = files_tag.lastChildElement();
	QString last_file_ID_str = last_file_element.attribute("ID");
	int last_file_ID = last_file_ID_str.toInt();

	// TODO: check for duplicate files

	// append files from QList<QString>
	int file_ID= last_file_ID + 1;
	for (int i = 0; i < file_list.size(); ++i)
	{
	    QString path = file_list[i];
	    QString filename = path.section("/",-1,-1);

	    QDomElement file_tag = doc.createElement("File");
	    file_tag.setAttribute("ID", QString::number(file_ID));
	    files_tag.appendChild(file_tag);
	    file_ID++;

	    QDomElement file_name_tag = doc.createElement("Name");
	    file_tag.appendChild(file_name_tag);
	    QDomText t1 = doc.createTextNode(filename);
	    file_name_tag.appendChild(t1);

	    QDomElement location_name_tag = doc.createElement("Location");
	    file_tag.appendChild(location_name_tag);
	    QDomText t2 = doc.createTextNode(path);
	    location_name_tag.appendChild(t2);
    }

    // write to file
    QString xml = doc.toString();

    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug() << "Could not open file for writing!";
    }

    QTextStream out(&file);
    out << xml;
    file.flush();
    file.close();
}

void XMLHandler::read_xml_file()
{
	QDomDocument document;

	// load the file
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "Unable to open file!";
		return;
	}
	else
	{
		if (!document.setContent(&file))
		{
			qDebug() << "Invalid XML file!";
			return;
		}
		file.close();
	}

	// get root element
	QDomElement docElem = document.documentElement();

	// check root tag
	QString rootTag = docElem.tagName();
	if (!rootTag.compare("\"StreXRD_Project\""))
	{
		qDebug() << "Not a StreXRD project file!";
	}

	QDomElement root = document.firstChildElement();


	// get the project name
	QDomNodeList items = root.elementsByTagName("Name");
	QDomNode itemnode = items.at(0);
	QDomElement itemEl = itemnode.toElement();
	project_name = itemEl.text();

	// get file names and locations
	QDomNodeList files = docElem.elementsByTagName("File");
	for (int i = 0; i < files.count(); i++)
	{
		QDomNode filenode = files.at(i);
		if (filenode.isElement())
		{
			QString file_ID = filenode.toElement().attribute("ID");
			int file_ID_number = file_ID.toInt();
			file_ID_list.append(file_ID_number);

			QDomNodeList file_names = filenode.toElement().elementsByTagName("Name");
			QDomNodeList file_locations = filenode.toElement().elementsByTagName("Location");

			QString file_name = file_names.at(0).toElement().text();
			QString file_location =  file_locations.at(0).toElement().text();

			this->file_names_locations.insert(file_name, file_location);

			file_name_list.append(file_name);
			file_location_list.append(file_location);
		}
	}
}

void XMLHandler::create_xml_file(QString project_name, QString project_directory, QList<QString> file_list)
{
    QDomDocument doc;

    // create header
    QDomProcessingInstruction header = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");
    doc.appendChild(header);

    // create root element
    QDomElement root = doc.createElement("StreXRD_Project");
    doc.appendChild(root);

    // create root children elements
    QDomElement name_tag = doc.createElement("Name");
    root.appendChild(name_tag);
    QDomElement directory_tag = doc.createElement("Directory");
    root.appendChild(directory_tag);
    QDomElement calibration_tag = doc.createElement("Calibration");
    root.appendChild(calibration_tag);
    QDomElement files_tag = doc.createElement("Files");
    root.appendChild(files_tag);

    // set project name and directory location
    QDomText t;
    t = doc.createTextNode(project_name);
    name_tag.appendChild(t);
    t = doc.createTextNode(project_directory);
    directory_tag.appendChild(t);

    // create calibration elements
    QDomElement calibration_data_tag = doc.createElement("Calibration_Data");
    calibration_tag.appendChild(calibration_data_tag);
    QDomElement standard_file_tag = doc.createElement("Standard_Data");
    calibration_tag.appendChild(standard_file_tag);
    QDomElement calibrant_tag = doc.createElement("Calibrant");
    calibration_tag.appendChild(calibrant_tag);

    // TODO: set calibration information

    // create file elements from QList<QString>
    for (int i = 0; i < file_list.size(); ++i)
    {
        QString path = file_list[i];
        QString filename = path.section("/",-1,-1);

        QDomElement file_tag = doc.createElement("File");
        file_tag.setAttribute("ID", QString::number(i));
        files_tag.appendChild(file_tag);

        QDomElement file_name_tag = doc.createElement("Name");
        file_tag.appendChild(file_name_tag);
        QDomText t1 = doc.createTextNode(filename);
        file_name_tag.appendChild(t1);

        QDomElement location_name_tag = doc.createElement("Location");
        file_tag.appendChild(location_name_tag);
        QDomText t2 = doc.createTextNode(path);
        location_name_tag.appendChild(t2);
    }

    // write to file
    QString xml = doc.toString();

    QFile file(filename);

    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug() << "Could not open file for writing!";
    }

    QTextStream out(&file);
    out << xml;
    file.flush();
    file.close();
}