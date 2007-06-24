#include "TSettings.h"

TSettings::TSettings(QString path)
{
	QDir dir;
	settingsFile = new QSettings(path + "settings.ini", QSettings::IniFormat);
	if(!settingsFile->contains("connection/port"))
		settingsFile->setValue("connection/port", 4980);
	if(!settingsFile->contains("connection/maxconnections"))
		settingsFile->setValue("connection/maxconnections", 5);
	if(!settingsFile->contains("directories/temporary"))
	{
		settingsFile->setValue("directories/temporary", path + "temp");
		dir.mkpath(path + "temp");
	}
	if(!settingsFile->contains("directories/completed"))
	{
		settingsFile->setValue("directories/completed", path + "incoming");
		dir.mkpath(path + "incoming");
	}
	if(!settingsFile->contains("directories/shared"))
	{
		settingsFile->setValue("directories/shared", path + "shared");
		dir.mkpath(path + "shared");
	}
	if(!settingsFile->contains("interface/theme"))
		settingsFile->setValue("interface/theme", "greyblue");

	port = settingsFile->value("connection/port").toInt();
	maxconnections = settingsFile->value("connection/maxconnections").toInt();
	temporaryfiles = settingsFile->value("directories/temporary").toString();
	completedfiles = settingsFile->value("directories/completed").toString();
	sharedfiles = settingsFile->value("directories/shared").toString();
	theme = settingsFile->value("interface/theme").toString();
}

void TSettings::writeSettings()
{
	settingsFile->setValue("connection/port", port);
	settingsFile->setValue("connection/maxconnections", maxconnections);
	settingsFile->setValue("directories/temporary", temporaryfiles);
	settingsFile->setValue("directories/completed", completedfiles);
	settingsFile->setValue("directories/shared", sharedfiles);
	settingsFile->setValue("interface/theme", theme);
}
