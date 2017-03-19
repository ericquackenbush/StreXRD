#pragma once

#include <QtXml>
#include <QList>

class XMLHandler {
	QList<int> file_ID_list;
	QList<QString> file_name_list;
	QList<QString> file_location_list;
	QString project_name;
	QHash<QString, QString> file_names_locations;
public:
	XMLHandler(QString);
	QList<int> get_file_ID();
	QList<QString> get_file_names();
	QList<QString> get_file_locations();
	QString get_project_name();
	QHash<QString, QString> get_hash();
};