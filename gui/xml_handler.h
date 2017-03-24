#pragma once

#include <QDomDocument>
#include <QList>

class XMLHandler {
	QString filename;
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
	void add_files(QList<QString>);
	void create_xml_file(QString, QString, QList<QString>);
	void read_xml_file();
};