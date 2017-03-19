#include <QFileDialog>
#include <QtXml>
#include <QList>
#include <xml_handler.h>

XMLHandler::XMLHandler(QString filename)
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

			file_names_locations.insert(file_name, file_location);

			file_name_list.append(file_name);
			file_location_list.append(file_location);
		}
	}
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