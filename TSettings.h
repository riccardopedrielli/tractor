#ifndef TSETTINGS_H
#define TSETTINGS_H

#include <QtCore>

class TSettings
{
public:
	TSettings(QString path);
	qint16 port;
	int maxconnections;
	QString temporaryfiles;
	QString completedfiles;
	QString sharedfiles;
	QString theme;
	void writeSettings();

private:
	QSettings *settingsFile;
};

#endif //TSETTINGS_H
