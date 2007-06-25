#ifndef TSHARED_H
#define TSHARED_H

#include <QDir>
#include <QSettings>

class TShared
{
private:
	QDir dir;

	QString createFileId(QString path);
	void updateShareList(QDir dir, QStringList &duplicatedid, QString sharedlistpath);

public:
	QStringList updateShareList(QString sharepath, QString sharedlistpath);	
	QStringList checkShareList(QString sharedlistpath);
};

#endif //TSHARED_H
