#include <QApplication>
#include "mainwindow.h"
#include "TClient.h"
#include "TServer.h"
#include "TSettings.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QDir dir;
	QString path = dir.homePath() + "/.tractor/";
	dir.mkpath(path);
	TSettings *settings = new TSettings(path);
	MainWindow *mainWindow = new MainWindow(settings);
	TServer *server = new TServer(settings->port, settings->maxconnections, path + "sharedlist.xml");
	TClient *client = new TClient(path + "sharedlist.xml", settings->sharedfiles, settings->temporaryfiles, settings->completedfiles, settings->port);
	client->updateSharedList();
	mainWindow->show();
	return app.exec();
}
