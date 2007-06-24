#ifndef TXML_H
#define TXML_H

#include <QFile>

#include "info_structs.h"

enum InfoTagName {TAG_NAME, TAG_ID, TAG_SIZE, TAG_LASTMOD, TAG_COMPLETE, TAG_PATH};

class TXml
{
public:		
	static void writeFileInfo(QString sharedpath, QString name, QString id, QString size, 
			QString lastmod, QString complete, QString path);
	static bool getFileInfo(QString path, InfoTagName tagname, QString value, 
			QString &name, QString &id, QString &lastmod, QString &complete, QString &path);
	static bool setFileInfo(QString xmlpath, InfoTagName tagname, QString value, 
			InfoTagName settagname, QString setvalue);
	static QStringList getPathList(QString xmlpath);
	static bool deleteFile(QString xmlpath, InfoTagName tagname, QString value);
	static QList<FileInfo> TXml::getFileList(QString xmlpath);
};

#endif //TXML_H
